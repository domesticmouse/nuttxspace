#!/usr/bin/env python3
"""Static site builder for the Seeed Studio XIAO RP2350 Bar Graph tutorial series."""

from __future__ import annotations

import argparse
import html
import json
import re
import shutil
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import yaml
from markdown_it import MarkdownIt


FRONTMATTER_RE = re.compile(r"\A---\s*\n(.*?\n)---\s*\n", re.DOTALL)
ATTR_RE = re.compile(r'([a-zA-Z0-9_-]+)="([^"]*)"')
HEADING_ID_RE = re.compile(
    r"(<h([1-6])>)((?:(?!</h\2>).)*?)\s*\{#([a-zA-Z0-9_-]+)\}\s*(</h\2>)",
    re.DOTALL,
)
TABLE_CAPTION_RE = re.compile(
    r"<p>Table:\s*(.*?)</p>\s*<table>",
    re.DOTALL,
)
BLANK_TOKEN_RE = re.compile(r"\{\{blank:([^}]+)\}\}")
SIDENOTE_RE = re.compile(r"\{\{sidenote:((?:[^{}]|\{[^{}]*\})+)\}\}")
WIDGET_RE = re.compile(r"\{\{widget:([a-zA-Z0-9_-]+)\}\}")
MD_LINK_RE = re.compile(r'\]\(([^)]+\.md(?:#[^)]+)?)\)')
PRE_CODE_CHECK_RE = re.compile(r"<pre><code[^>]*>(.*?)</code></pre>", re.DOTALL)
FORBIDDEN_IN_CODE_RE = re.compile(r"<(?:p|h[1-6]|pre)\b", re.IGNORECASE)
STRIP_CODE_AND_PRE_RE = re.compile(r"<pre\b[^>]*>.*?</pre>|<code\b[^>]*>.*?</code>", re.DOTALL | re.IGNORECASE)
MALFORMED_TAG_RE = re.compile(r"<[0-9]\b|\{#[a-zA-Z0-9_-]+\}")
UNRENDERED_TEX_RE = re.compile(r"\$[^$\n]+\$|\\(?:approx|pm|sim|Omega|text|le|ge|in)\b")


@dataclass
class PageMeta:
    src_path: Path
    rel_src: Path
    out_rel: Path
    title: str
    page_title: str
    nav_id: str
    badge: str
    subtitle: str
    nav_label: str | None
    card_title: str | None
    card_summary: str | None
    body_md: str
    kind: str  # "index", "lesson", "reference"
    lesson_num: str | None = None


def parse_attrs(attr_str: str) -> dict[str, str]:
    if "{" in attr_str or "}" in attr_str:
        raise ValueError(f"Curly-brace attributes are forbidden: {attr_str!r}")
    return {m.group(1): m.group(2) for m in ATTR_RE.finditer(attr_str)}


def load_page(src_path: Path, root_dir: Path, kind: str) -> PageMeta:
    raw = src_path.read_text(encoding="utf-8")
    m = FRONTMATTER_RE.match(raw)
    if not m:
        raise ValueError(f"{src_path}: missing YAML frontmatter")
    fm = yaml.safe_load(m.group(1)) or {}
    body_md = raw[m.end() :]

    for req in ("title", "page_title", "nav_id", "badge", "subtitle"):
        if not fm.get(req):
            raise ValueError(f"{src_path}: missing required frontmatter key {req!r}")

    rel_src = src_path.relative_to(root_dir)
    if kind == "index":
        out_rel = Path("index.html")
    elif kind == "lesson":
        out_rel = Path("lessons") / f"{src_path.stem}.html"
    elif kind == "reference":
        out_name = "glossary.html" if src_path.name == "GLOSSARY.md" else f"{src_path.stem}.html"
        out_rel = Path("reference") / out_name
    else:
        raise ValueError(f"Unknown page kind: {kind}")

    card = fm.get("card") or {}
    lesson_num = None
    if kind == "lesson":
        num_match = re.match(r"^(\d{4})-", src_path.stem)
        if num_match:
            lesson_num = num_match.group(1)

    return PageMeta(
        src_path=src_path,
        rel_src=rel_src,
        out_rel=out_rel,
        title=str(fm["title"]),
        page_title=str(fm["page_title"]),
        nav_id=str(fm["nav_id"]),
        badge=str(fm["badge"]),
        subtitle=str(fm["subtitle"]),
        nav_label=str(fm["nav_label"]) if fm.get("nav_label") else None,
        card_title=str(card["title"]) if card.get("title") else None,
        card_summary=str(card["summary"]) if card.get("summary") else None,
        body_md=body_md,
        kind=kind,
        lesson_num=lesson_num,
    )


def rewrite_md_links(
    text: str,
    page: PageMeta,
    root_dir: Path,
    errors: list[str],
) -> str:
    def _replace(match: re.Match[str]) -> str:
        raw_target = match.group(1)
        if raw_target.startswith(("http://", "https://", "mailto:")):
            return match.group(0)
        if "#" in raw_target:
            path_part, frag = raw_target.split("#", 1)
            frag_suffix = f"#{frag}"
        else:
            path_part = raw_target
            frag_suffix = ""

        resolved = (page.src_path.parent / path_part).resolve()
        if not resolved.is_file():
            errors.append(
                f"{page.rel_src}: broken internal .md link {raw_target!r} (resolved to {resolved})"
            )
            return match.group(0)

        try:
            rel_to_root = resolved.relative_to(root_dir)
        except ValueError:
            errors.append(
                f"{page.rel_src}: external relative link outside tutorial root is forbidden: {raw_target!r}"
            )
            return match.group(0)

        if rel_to_root.name == "GLOSSARY.md":
            html_name = "glossary.html"
        else:
            html_name = f"{rel_to_root.stem}.html"

        parts = list(Path(path_part).parts)
        parts[-1] = html_name
        rewritten = "/".join(parts) + frag_suffix
        return f"]({rewritten})"

    return MD_LINK_RE.sub(_replace, text)


def render_inline_md(md: MarkdownIt, text: str) -> str:
    return md.renderInline(text.strip())


def render_blank_tokens(code_escaped: str, ex_id: str, errors: list[str], page: PageMeta) -> tuple[str, int]:
    blank_idx = 0

    def _sub(match: re.Match[str]) -> str:
        nonlocal blank_idx
        blank_idx += 1
        spec = html.unescape(match.group(1))
        parts = spec.split("|")
        if len(parts) < 4:
            errors.append(
                f"{page.rel_src}: exercise {ex_id} blank #{blank_idx} must have answer|hint|placeholder|width"
            )
            while len(parts) < 4:
                parts.append("")
        width_str = parts[-1].strip()
        placeholder = parts[-2].strip()
        error_hint = parts[-3].strip()
        answer = "|".join(parts[:-3]).strip()
        try:
            width_ch = max(int(width_str), 4)
        except ValueError:
            width_ch = 12
        input_id = f"{ex_id}-blank-{blank_idx}"
        return (
            f'<input type="text" class="code-blank" id="{html.escape(input_id)}" '
            f'data-answer="{html.escape(answer)}" '
            f'data-hint="{html.escape(error_hint)}" '
            f'placeholder="{html.escape(placeholder)}" '
            f'aria-label="{html.escape(f"Code blank {blank_idx}: {error_hint}")}" '
            f'autocomplete="off" spellcheck="false" '
            f'style="width: {width_ch + 2}ch;">'
        )

    rendered = BLANK_TOKEN_RE.sub(_sub, code_escaped)
    return rendered, blank_idx


def build_code_figure_html(
    lang: str,
    attrs_str: str,
    code_body: str,
    page: PageMeta,
    errors: list[str],
    ex_id: str | None = None,
) -> tuple[str, int]:
    try:
        attrs = parse_attrs(attrs_str)
    except ValueError as exc:
        errors.append(f"{page.rel_src}: {exc}")
        attrs = {}

    caption = attrs.get("caption", "")
    clean_lang = lang.strip() or "text"

    if clean_lang == "mermaid":
        fig_cap = (
            f'<figcaption class="mermaid-caption">{html.escape(caption)}</figcaption>'
            if caption
            else ""
        )
        return (
            f'<figure class="mermaid-figure">'
            f'<pre class="mermaid">{html.escape(code_body.strip())}</pre>'
            f"{fig_cap}"
            f"</figure>",
            0,
        )

    escaped_code = html.escape(code_body.rstrip("\n"))
    blank_count = 0
    keep_markup_class = ""
    if ex_id is not None:
        escaped_code, blank_count = render_blank_tokens(escaped_code, ex_id, errors, page)
        keep_markup_class = " keep-markup"

    cap_html = (
        f'<div class="code-caption"><span class="code-lang-tag">{html.escape(clean_lang)}</span>'
        f'<span class="code-caption-text">{html.escape(caption)}</span></div>'
        if caption
        else f'<div class="code-caption"><span class="code-lang-tag">{html.escape(clean_lang)}</span></div>'
    )
    return (
        f'<div class="code-figure">'
        f"{cap_html}"
        f'<pre class="language-{html.escape(clean_lang)}{keep_markup_class}">'
        f'<code class="language-{html.escape(clean_lang)}{keep_markup_class}">{escaped_code}</code>'
        f"</pre>"
        f"</div>",
        blank_count,
    )


def render_exercise_block(
    attr_str: str,
    inner_md: str,
    md: MarkdownIt,
    page: PageMeta,
    errors: list[str],
) -> str:
    try:
        attrs = parse_attrs(attr_str)
    except ValueError as exc:
        errors.append(f"{page.rel_src}: {exc}")
        attrs = {}

    ex_id = attrs.get("id", "ex-unknown")
    title = attrs.get("title", "Interactive Exercise")
    badge = attrs.get("badge", "Skill Practice")

    prompts_re = re.compile(r"^:::prompts\s*\n(.*?\n)^:::\s*$", re.MULTILINE | re.DOTALL)
    pm = prompts_re.search(inner_md)
    if not pm:
        errors.append(f"{page.rel_src}: :::exercise {ex_id} missing nested :::prompts YAML block")
        prompts_data: dict[str, Any] = {}
        content_md = inner_md
    else:
        prompts_data = yaml.safe_load(pm.group(1)) or {}
        if "hint" not in prompts_data or "success" not in prompts_data:
            errors.append(
                f"{page.rel_src}: :::exercise {ex_id} :::prompts must contain 'hint' and 'success'"
            )
        content_md = inner_md[: pm.start()] + inner_md[pm.end() :]

    code_placeholders: dict[str, str] = {}
    total_blanks = 0
    fence_re = re.compile(r"^```([a-zA-Z0-9_+-]*)([^\n]*)\n(.*?)^```\s*$", re.MULTILINE | re.DOTALL)

    def _extract_code(m: re.Match[str]) -> str:
        nonlocal total_blanks
        idx = len(code_placeholders)
        key = f"<!--EX_CODE_PLACEHOLDER_{idx}-->"
        fig_html, b_count = build_code_figure_html(
            m.group(1), m.group(2), m.group(3), page, errors, ex_id=ex_id
        )
        total_blanks += b_count
        code_placeholders[key] = fig_html
        return key

    staged_md = fence_re.sub(_extract_code, content_md)
    if total_blanks == 0:
        errors.append(f"{page.rel_src}: :::exercise {ex_id} contains zero {{blank:...}} tokens")

    rendered_inner = md.render(staged_md)
    for k, v in code_placeholders.items():
        rendered_inner = rendered_inner.replace(f"<p>{k}</p>", v).replace(k, v)

    hint_str = str(prompts_data.get("hint", "Review the code block above."))
    success_str = str(prompts_data.get("success", "All blanks verified!"))

    return (
        f'<section class="interactive-card exercise-card" id="{html.escape(ex_id)}" '
        f'data-hint="{html.escape(hint_str)}" data-success="{html.escape(success_str)}">'
        f'<header class="card-header">'
        f'<div class="card-title-group">'
        f'<span class="card-eyebrow">INTERACTIVE EXERCISE</span>'
        f'<h3 class="card-title">{html.escape(title)}</h3>'
        f"</div>"
        f'<span class="card-badge">{html.escape(badge)}</span>'
        f"</header>"
        f'<div class="card-body">{rendered_inner}</div>'
        f'<div class="card-actions">'
        f'<button type="button" class="btn-primary exercise-check-btn" data-target="{html.escape(ex_id)}">'
        f"Check Answers</button>"
        f'<button type="button" class="btn-secondary exercise-hint-btn" data-target="{html.escape(ex_id)}">'
        f"Show Hint</button>"
        f'<button type="button" class="btn-ghost exercise-reset-btn" data-target="{html.escape(ex_id)}">'
        f"Reset</button>"
        f"</div>"
        f'<div class="exercise-feedback" id="{html.escape(ex_id)}-feedback" aria-live="polite" hidden></div>'
        f"</section>"
    )


def render_quiz_block(
    attr_str: str,
    inner_yaml: str,
    md: MarkdownIt,
    page: PageMeta,
    errors: list[str],
) -> str:
    try:
        attrs = parse_attrs(attr_str)
    except ValueError as exc:
        errors.append(f"{page.rel_src}: {exc}")
        attrs = {}

    quiz_id = attrs.get("id", "quiz-unknown")
    title = attrs.get("title", "Retrieval Practice")
    badge = attrs.get("badge", "3 Questions • Equal-Length Options")

    try:
        questions = yaml.safe_load(inner_yaml) or []
    except yaml.YAMLError as exc:
        errors.append(f"{page.rel_src}: :::quiz {quiz_id} invalid YAML: {exc}")
        questions = []

    if not isinstance(questions, list) or not questions:
        errors.append(f"{page.rel_src}: :::quiz {quiz_id} must contain a YAML list of questions")
        return ""

    q_html_parts: list[str] = []
    for q_idx, q in enumerate(questions, start=1):
        q_text = str(q.get("question", ""))
        wrong_hint = str(q.get("wrong_hint", ""))
        options = q.get("options") or []
        if len(options) != 4:
            errors.append(
                f"{page.rel_src}: :::quiz {quiz_id} Q{q_idx} must have exactly 4 options (found {len(options)})"
            )

        word_counts: list[int] = []
        correct_count = 0
        opt_buttons: list[str] = []

        for opt in options:
            letter = str(opt.get("letter", ""))
            text = str(opt.get("text", "")).strip()
            is_correct = bool(opt.get("correct", False))
            explain = str(opt.get("explain", ""))
            wc = len(text.split())
            word_counts.append(wc)
            if is_correct:
                correct_count += 1
            opt_buttons.append(
                f'<button type="button" class="quiz-option" '
                f'data-letter="{html.escape(letter)}" '
                f'data-correct="{"true" if is_correct else "false"}" '
                f'data-explain="{html.escape(explain)}">'
                f'<span class="quiz-option-letter">{html.escape(letter)}</span>'
                f'<span class="quiz-option-text">{render_inline_md(md, text)}</span>'
                f"</button>"
            )

        if len(set(word_counts)) > 1:
            errors.append(
                f"{page.rel_src}: :::quiz {quiz_id} Q{q_idx} option word counts are not equal: {word_counts}"
            )
        if correct_count != 1:
            errors.append(
                f"{page.rel_src}: :::quiz {quiz_id} Q{q_idx} must have exactly 1 correct option (found {correct_count})"
            )

        q_dom_id = f"{quiz_id}-q{q_idx}"
        q_html_parts.append(
            f'<div class="quiz-question" id="{html.escape(q_dom_id)}" '
            f'data-wrong-hint="{html.escape(wrong_hint)}">'
            f'<p class="quiz-q-prompt"><strong>Q{q_idx}.</strong> {render_inline_md(md, q_text)}</p>'
            f'<div class="quiz-options" role="group" aria-label="Question {q_idx} options">'
            + "".join(opt_buttons)
            + "</div>"
            f'<div class="quiz-q-feedback" id="{html.escape(q_dom_id)}-feedback" aria-live="polite" hidden></div>'
            f"</div>"
        )

    return (
        f'<section class="interactive-card quiz-card" id="{html.escape(quiz_id)}">'
        f'<header class="card-header">'
        f'<div class="card-title-group">'
        f'<span class="card-eyebrow">RETRIEVAL PRACTICE</span>'
        f'<h3 class="card-title">{html.escape(title)}</h3>'
        f"</div>"
        f'<span class="card-badge">{html.escape(badge)}</span>'
        f"</header>"
        f'<div class="card-body">' + "".join(q_html_parts) + "</div>"
        f"</section>"
    )


def render_verify_block(
    attr_str: str,
    inner_md: str,
    md: MarkdownIt,
    page: PageMeta,
    errors: list[str],
) -> str:
    try:
        attrs = parse_attrs(attr_str)
    except ValueError as exc:
        errors.append(f"{page.rel_src}: {exc}")
        attrs = {}

    verify_id = attrs.get("id", "verify-unknown")
    title = attrs.get("title", "Verification Lab")
    badge = attrs.get("badge", "Terminal Gate")

    code_placeholders: dict[str, str] = {}
    fence_re = re.compile(r"^```([a-zA-Z0-9_+-]*)([^\n]*)\n(.*?)^```\s*$", re.MULTILINE | re.DOTALL)

    def _extract_code(m: re.Match[str]) -> str:
        idx = len(code_placeholders)
        key = f"<!--VERIFY_CODE_PLACEHOLDER_{idx}-->"
        fig_html, _ = build_code_figure_html(
            m.group(1), m.group(2), m.group(3), page, errors
        )
        code_placeholders[key] = fig_html
        return key

    staged_md = fence_re.sub(_extract_code, inner_md)

    checklist_items: list[str] = []

    def _extract_check(m: re.Match[str]) -> str:
        checklist_items.append(m.group(1).strip())
        return ""

    staged_md = re.sub(r"^- \[ \]\s+(.+)$", _extract_check, staged_md, flags=re.MULTILINE)
    rendered_body = md.render(staged_md)
    for k, v in code_placeholders.items():
        rendered_body = rendered_body.replace(f"<p>{k}</p>", v).replace(k, v)

    checks_html = ""
    if checklist_items:
        items_html = []
        for idx, item_md in enumerate(checklist_items, start=1):
            chk_id = f"{verify_id}-chk-{idx}"
            items_html.append(
                f'<label class="verify-check-item" for="{html.escape(chk_id)}">'
                f'<input type="checkbox" id="{html.escape(chk_id)}" class="verify-checkbox" '
                f'data-verify-id="{html.escape(chk_id)}">'
                f'<span class="verify-check-label">{render_inline_md(md, item_md)}</span>'
                f"</label>"
            )
        checks_html = f'<div class="verify-checklist">' + "".join(items_html) + "</div>"

    return (
        f'<section class="interactive-card verify-card" id="{html.escape(verify_id)}">'
        f'<header class="card-header">'
        f'<div class="card-title-group">'
        f'<span class="card-eyebrow">VERIFICATION GATE</span>'
        f'<h3 class="card-title">{html.escape(title)}</h3>'
        f"</div>"
        f'<span class="card-badge">{html.escape(badge)}</span>'
        f"</header>"
        f'<div class="card-body">{rendered_body}{checks_html}</div>'
        f"</section>"
    )


def render_callout_block(
    directive_name: str,
    attr_str: str,
    inner_md: str,
    md: MarkdownIt,
    page: PageMeta,
    errors: list[str],
) -> str:
    try:
        attrs = parse_attrs(attr_str)
    except ValueError as exc:
        errors.append(f"{page.rel_src}: {exc}")
        attrs = {}

    title = attrs.get("title", "Note")
    variant_class = "callout-box"
    eyebrow = "NOTE"
    if directive_name == "callout-warn":
        variant_class = "callout-box callout-warn"
        eyebrow = "HARDWARE & DRIVER TRAP"
    elif directive_name == "callout-success":
        variant_class = "callout-box callout-success"
        eyebrow = "VERIFIED INVARIANT"
    elif directive_name == "teacher-prompt":
        variant_class = "callout-box teacher-prompt"
        eyebrow = "AI TEACHER PROMPT"

    code_placeholders: dict[str, str] = {}
    fence_re = re.compile(r"^```([a-zA-Z0-9_+-]*)([^\n]*)\n(.*?)^```\s*$", re.MULTILINE | re.DOTALL)

    def _extract_code(m: re.Match[str]) -> str:
        idx = len(code_placeholders)
        key = f"<!--CALLOUT_CODE_PLACEHOLDER_{idx}-->"
        fig_html, _ = build_code_figure_html(
            m.group(1), m.group(2), m.group(3), page, errors
        )
        code_placeholders[key] = fig_html
        return key

    staged_md = fence_re.sub(_extract_code, inner_md)
    rendered_inner = md.render(staged_md)
    for k, v in code_placeholders.items():
        rendered_inner = rendered_inner.replace(f"<p>{k}</p>", v).replace(k, v)

    return (
        f'<aside class="{variant_class}" aria-label="{html.escape(title)}">'
        f'<div class="callout-header">'
        f'<span class="callout-eyebrow">{html.escape(eyebrow)}</span>'
        f'<strong class="callout-title">{html.escape(title)}</strong>'
        f"</div>"
        f'<div class="callout-body">{rendered_inner}</div>'
        f"</aside>"
    )


def build_lesson_cards_html(lessons: list[PageMeta], md: MarkdownIt) -> str:
    cards: list[str] = []
    for lesson in lessons:
        num = lesson.lesson_num or "0000"
        cleaned_badge = re.sub(r"^Lesson\s+\d+\s*•\s*", "", lesson.badge, flags=re.IGNORECASE).strip()
        meta_label = f"LESSON {num} • {cleaned_badge}" if cleaned_badge else f"LESSON {num}"
        card_title_html = render_inline_md(md, lesson.card_title or lesson.title)
        card_summary_html = render_inline_md(md, lesson.card_summary or lesson.subtitle)
        href = f"lessons/{lesson.src_path.stem}.html"
        cards.append(
            f'<a class="lesson-card" href="{html.escape(href)}">'
            f'<div class="lesson-card-meta">{html.escape(meta_label)}</div>'
            f'<h3 class="lesson-card-title">{card_title_html}</h3>'
            f'<p class="lesson-card-summary">{card_summary_html}</p>'
            f'<span class="lesson-card-cta">Start Lesson {html.escape(num)} →</span>'
            f"</a>"
        )
    return '<div class="lesson-cards-grid">' + "".join(cards) + "</div>"


def build_reference_cards_html(refs: list[PageMeta], md: MarkdownIt) -> str:
    rows: list[str] = []
    for ref in refs:
        out_name = ref.out_rel.name
        href = f"reference/{out_name}"
        title_html = render_inline_md(md, ref.card_title or ref.title)
        summary_html = render_inline_md(md, ref.card_summary or ref.subtitle)
        rows.append(
            f"<tr>"
            f'<th scope="row"><a href="{html.escape(href)}">{title_html}</a></th>'
            f"<td>{summary_html}</td>"
            f'<td><a class="ref-sheet-badge" href="{html.escape(href)}">Open Reference →</a></td>'
            f"</tr>"
        )
    return (
        '<div class="table-scroll">'
        '<table class="ref-table">'
        "<caption>Course Reference Sheets &amp; Printable Engineering Guides</caption>"
        "<thead><tr>"
        '<th scope="col">Reference Sheet</th>'
        '<th scope="col">Coverage &amp; Purpose</th>'
        '<th scope="col">Quick Link</th>'
        "</tr></thead>"
        f'<tbody>{"".join(rows)}</tbody>'
        "</table>"
        "</div>"
    )


def parse_directives(
    text: str,
    md: MarkdownIt,
    page: PageMeta,
    errors: list[str],
    placeholders: dict[str, str],
) -> str:
    lines = text.splitlines(keepends=True)
    out_lines: list[str] = []
    i = 0
    n = len(lines)

    start_re = re.compile(
        r"^:::(exercise|quiz|verify|callout-warn|callout-success|callout|teacher-prompt)(?:\s+(.*))?$"
    )

    while i < n:
        line_stripped = lines[i].rstrip("\r\n")
        m = start_re.match(line_stripped)
        if not m:
            out_lines.append(lines[i])
            i += 1
            continue

        directive = m.group(1)
        attr_str = (m.group(2) or "").strip()
        i += 1
        inner_lines: list[str] = []
        depth = 1

        while i < n:
            cur = lines[i].rstrip("\r\n")
            if cur.startswith(":::") and cur != ":::":
                depth += 1
                inner_lines.append(lines[i])
            elif cur == ":::":
                depth -= 1
                if depth == 0:
                    i += 1
                    break
                inner_lines.append(lines[i])
            else:
                inner_lines.append(lines[i])
            i += 1

        if depth != 0:
            errors.append(f"{page.rel_src}: unclosed :::{directive} block")

        inner_content = "".join(inner_lines)
        if directive == "exercise":
            block_html = render_exercise_block(attr_str, inner_content, md, page, errors)
        elif directive == "quiz":
            block_html = render_quiz_block(attr_str, inner_content, md, page, errors)
        elif directive == "verify":
            block_html = render_verify_block(attr_str, inner_content, md, page, errors)
        else:
            block_html = render_callout_block(directive, attr_str, inner_content, md, page, errors)

        ph_key = f"<!--DIRECTIVE_PLACEHOLDER_{len(placeholders)}-->"
        placeholders[ph_key] = block_html
        out_lines.append(f"\n{ph_key}\n")

    return "".join(out_lines)


def render_page_body(
    page: PageMeta,
    lessons: list[PageMeta],
    refs: list[PageMeta],
    root_dir: Path,
    md: MarkdownIt,
    errors: list[str],
) -> str:
    text = page.body_md
    placeholders: dict[str, str] = {}

    # 1. Extract directives first (they handle their own inner code blocks)
    text = parse_directives(text, md, page, errors, placeholders)

    # 2. Extract all remaining fenced code blocks into single-line placeholders BEFORE md.render(),
    #    link rewriting, or sidenote expansion.
    fence_re = re.compile(r"^```([a-zA-Z0-9_+-]*)([^\n]*)\n(.*?)^```\s*$", re.MULTILINE | re.DOTALL)

    def _extract_top_code(m: re.Match[str]) -> str:
        fig_html, _ = build_code_figure_html(
            m.group(1), m.group(2), m.group(3), page, errors
        )
        ph_key = f"<!--FENCED_CODE_PLACEHOLDER_{len(placeholders)}-->"
        placeholders[ph_key] = fig_html
        return ph_key

    text = fence_re.sub(_extract_top_code, text)

    # 3. Rewrite internal .md links to .html
    text = rewrite_md_links(text, page, root_dir, errors)

    # 4. Expand Tufte sidenotes {{sidenote:...}}
    def _expand_sidenote(m: re.Match[str]) -> str:
        note_html = render_inline_md(md, m.group(1))
        ph_key = f"<!--SIDENOTE_PLACEHOLDER_{len(placeholders)}-->"
        placeholders[ph_key] = (
            f'<span class="sidenote-wrapper">'
            f'<span class="sidenote-number" tabindex="0" role="button" aria-expanded="false" '
            f'aria-label="Toggle margin note"></span>'
            f'<span class="sidenote">{note_html}</span>'
            f"</span>"
        )
        return ph_key

    text = SIDENOTE_RE.sub(_expand_sidenote, text)

    # 5. Expand widgets {{widget:...}}
    def _expand_widget(m: re.Match[str]) -> str:
        w_id = m.group(1)
        if w_id == "lesson-cards":
            w_html = build_lesson_cards_html(lessons, md)
        elif w_id == "reference-cards":
            w_html = build_reference_cards_html(refs, md)
        else:
            w_html = (
                f'<div class="interactive-simulator" id="sim-{html.escape(w_id)}" '
                f'data-simulator="{html.escape(w_id)}"></div>'
            )
        ph_key = f"<!--WIDGET_PLACEHOLDER_{len(placeholders)}-->"
        placeholders[ph_key] = w_html
        return ph_key

    text = WIDGET_RE.sub(_expand_widget, text)

    # 6. Render Markdown to HTML
    rendered = md.render(text)

    # 7. Convert heading {#anchor-id} suffixes
    rendered = HEADING_ID_RE.sub(r'<h\2 id="\4">\3\5', rendered)

    # 8. Style tables with Table: caption and .ref-table + scope="col"
    def _table_caption_sub(m: re.Match[str]) -> str:
        cap_inner = m.group(1).strip()
        return f'<table><caption>{cap_inner}</caption>'

    rendered = TABLE_CAPTION_RE.sub(_table_caption_sub, rendered)
    rendered = rendered.replace("<table>", '<div class="table-scroll"><table class="ref-table">')
    rendered = rendered.replace("</table>", "</table></div>")
    rendered = re.sub(r"<thead>\s*<tr>(.*?)</tr>\s*</thead>", lambda m: "<thead><tr>" + m.group(1).replace("<th>", '<th scope="col">') + "</tr></thead>", rendered, flags=re.DOTALL)

    # 9. Restore placeholders in reverse insertion order so nested placeholders resolve cleanly
    for ph_key, html_val in reversed(list(placeholders.items())):
        rendered = rendered.replace(f"<p>{ph_key}</p>", html_val).replace(ph_key, html_val)

    # 10. Validate no <pre><code> contains <p>, <h1..6>, or nested <pre>
    for code_m in PRE_CODE_CHECK_RE.finditer(rendered):
        inner_code = code_m.group(1)
        if FORBIDDEN_IN_CODE_RE.search(inner_code):
            errors.append(
                f"{page.rel_src}: <pre><code> block contains corrupted HTML tags (<p>, <h1..6>, or <pre>)"
            )

    # 11. Validate no malformed heading tags (<2 id=...>) or unrendered TeX markup outside <code>/<pre>
    prose_only = STRIP_CODE_AND_PRE_RE.sub("", rendered)
    for bad_tag in MALFORMED_TAG_RE.finditer(prose_only):
        errors.append(
            f"{page.rel_src}: malformed HTML tag or unparsed heading anchor {bad_tag.group(0)!r} in rendered output"
        )
    for bad_tex in UNRENDERED_TEX_RE.finditer(prose_only):
        errors.append(
            f"{page.rel_src}: unrendered TeX markup {bad_tex.group(0)!r} in rendered HTML"
        )

    return rendered


def build_nav_html(
    page: PageMeta,
    lessons: list[PageMeta],
    refs: list[PageMeta],
) -> str:
    prefix = "" if page.kind == "index" else "../"
    index_href = f"{prefix}index.html"

    # Left: contextual breadcrumbs
    if page.kind == "index":
        breadcrumbs_html = (
            '<div class="nav-breadcrumbs">'
            '<span class="nav-crumb-active" aria-current="page">Index</span>'
            "</div>"
        )
    elif page.kind == "lesson":
        num = page.lesson_num or "0000"
        breadcrumbs_html = (
            '<div class="nav-breadcrumbs">'
            f'<a class="nav-crumb-link" href="{html.escape(index_href)}">Index</a>'
            '<span class="nav-crumb-sep" aria-hidden="true">›</span>'
            f'<span class="nav-crumb-active" aria-current="page">Lesson {html.escape(num)}</span>'
            "</div>"
        )
    else:
        sheet_label = page.nav_label or page.title
        breadcrumbs_html = (
            '<div class="nav-breadcrumbs">'
            f'<a class="nav-crumb-link" href="{html.escape(index_href)}">Index</a>'
            '<span class="nav-crumb-sep" aria-hidden="true">›</span>'
            f'<span class="nav-crumb-active" aria-current="page">{html.escape(sheet_label)}</span>'
            "</div>"
        )

    # Right: controls
    pager_pill_html = ""
    if page.kind == "lesson":
        idx = next((i for i, l in enumerate(lessons) if l.nav_id == page.nav_id), -1)
        prev_lesson = lessons[idx - 1] if idx > 0 else None
        next_lesson = lessons[idx + 1] if 0 <= idx < len(lessons) - 1 else None

        if prev_lesson:
            prev_btn = (
                f'<a class="nav-pager-link" href="{html.escape(prev_lesson.src_path.stem)}.html" '
                f'title="Lesson {html.escape(prev_lesson.lesson_num or "")}: {html.escape(prev_lesson.title)}">'
                f"← Prev</a>"
            )
        else:
            prev_btn = '<span class="nav-pager-disabled" aria-disabled="true">← Prev</span>'

        if next_lesson:
            next_btn = (
                f'<a class="nav-pager-link" href="{html.escape(next_lesson.src_path.stem)}.html" '
                f'title="Lesson {html.escape(next_lesson.lesson_num or "")}: {html.escape(next_lesson.title)}">'
                f"Next →</a>"
            )
        else:
            next_btn = '<span class="nav-pager-disabled" aria-disabled="true">Next →</span>'

        pager_pill_html = (
            f'<div class="nav-pager-group" role="group" aria-label="Adjacent lessons">'
            f"{prev_btn}"
            f'<span class="nav-pager-dot" aria-hidden="true">·</span>'
            f"{next_btn}"
            f"</div>"
            f'<span class="nav-divider" aria-hidden="true">|</span>'
        )

    ref_links: list[str] = []
    for ref in refs:
        label = ref.nav_label or ref.title
        href = f"{prefix}reference/{ref.out_rel.name}"
        is_active = page.nav_id == ref.nav_id
        current_attr = ' aria-current="page"' if is_active else ""
        active_cls = " nav-ref-link-active" if is_active else ""
        ref_links.append(
            f'<a class="nav-ref-link{active_cls}" href="{html.escape(href)}"{current_attr}>'
            f"{html.escape(label)}</a>"
        )

    return (
        '<nav class="course-nav" aria-label="Course">'
        f"{breadcrumbs_html}"
        '<div class="nav-controls">'
        f"{pager_pill_html}"
        f'<div class="nav-ref-group">{"".join(ref_links)}</div>'
        '<span class="nav-divider" aria-hidden="true">|</span>'
        '<span class="mission-badge">XIAO RP2350 • NuttX RTOS</span>'
        "</div>"
        "</nav>"
    )


def build_lesson_pager_html(page: PageMeta, lessons: list[PageMeta]) -> str:
    if page.kind != "lesson":
        return ""
    idx = next((i for i, l in enumerate(lessons) if l.nav_id == page.nav_id), -1)
    if idx < 0:
        return ""
    prev_lesson = lessons[idx - 1] if idx > 0 else None
    next_lesson = lessons[idx + 1] if idx < len(lessons) - 1 else None

    if prev_lesson:
        prev_card = (
            f'<a class="pager-card pager-prev" href="{html.escape(prev_lesson.src_path.stem)}.html">'
            f'<span class="pager-eyebrow">← Previous • Lesson {html.escape(prev_lesson.lesson_num or "")}</span>'
            f'<span class="pager-title">{html.escape(prev_lesson.title)}</span>'
            f"</a>"
        )
    else:
        prev_card = (
            '<a class="pager-card pager-prev" href="../index.html">'
            '<span class="pager-eyebrow">← Course Index</span>'
            '<span class="pager-title">Return to Curriculum Overview</span>'
            "</a>"
        )

    if next_lesson:
        next_card = (
            f'<a class="pager-card pager-next" href="{html.escape(next_lesson.src_path.stem)}.html">'
            f'<span class="pager-eyebrow">Next • Lesson {html.escape(next_lesson.lesson_num or "")} →</span>'
            f'<span class="pager-title">{html.escape(next_lesson.title)}</span>'
            f"</a>"
        )
    else:
        next_card = (
            '<a class="pager-card pager-next" href="../reference/glossary.html">'
            '<span class="pager-eyebrow">Course Complete • Reference Sheets →</span>'
            '<span class="pager-title">Review Glossary &amp; Hardware Cheat Sheets</span>'
            "</a>"
        )

    return (
        '<nav class="lesson-pager" aria-label="Lesson pagination">'
        f"{prev_card}{next_card}"
        "</nav>"
    )


def wrap_document_html(
    page: PageMeta,
    nav_html: str,
    body_html: str,
    pager_html: str,
) -> str:
    prefix = "" if page.kind == "index" else "../"
    css_href = f"{prefix}assets/style.css"
    js_src = f"{prefix}assets/course.js"

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="color-scheme" content="dark">
  <title>{html.escape(page.page_title)}</title>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/themes/prism-tomorrow.min.css">
  <link rel="stylesheet" href="{html.escape(css_href)}">
</head>
<body data-page-id="{html.escape(page.nav_id)}">
  <a href="#main-heading" class="skip-link visually-hidden">Skip to main content</a>
  <header class="site-header">
    {nav_html}
  </header>
  <main class="site-main" id="main-content">
    <article class="tufte-article">
      <header class="article-header">
        <div class="article-badge">{html.escape(page.badge)}</div>
        <h1 class="article-title" id="main-heading" tabindex="-1">{html.escape(page.title)}</h1>
        <p class="article-subtitle">{html.escape(page.subtitle)}</p>
      </header>
      <div class="article-body">
        {body_html}
      </div>
      {pager_html}
    </article>
  </main>
  <footer class="site-footer">
    <div class="footer-inner">
      <p>Out-of-Tree Seeed Studio XIAO RP2350 6-Segment PWM LED Bar Graph — Apache NuttX Interactive Engineering Course</p>
    </div>
  </footer>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/prism.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/components/prism-c.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/components/prism-bash.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/components/prism-makefile.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/components/prism-yaml.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/plugins/keep-markup/prism-keep-markup.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.min.js"></script>
  <script src="{html.escape(js_src)}"></script>
</body>
</html>
"""


def main() -> int:
    parser = argparse.ArgumentParser(description="Compile tutorial Markdown sources to HTML.")
    parser.add_argument(
        "--out-dir",
        type=Path,
        default=None,
        help="Optional output directory (e.g. _site). Defaults to compiling in-place in the tutorial root.",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Run strict schema, link, quiz option word-count, and code-block corruption checks.",
    )
    args = parser.parse_args()

    root_dir = Path(__file__).resolve().parent
    out_dir = args.out_dir.resolve() if args.out_dir else root_dir

    md = MarkdownIt("commonmark", {"html": True, "typographer": False}).enable("table")
    errors: list[str] = []

    index_src = root_dir / "index.md"
    if not index_src.is_file():
        print(f"[ERROR] Missing {index_src}", file=sys.stderr)
        return 1

    lesson_srcs = sorted((root_dir / "lessons").glob("[0-9][0-9][0-9][0-9]-*.md"))
    ref_srcs = sorted((root_dir / "reference").glob("*.md"))
    # Put GLOSSARY.md first in reference nav
    ref_srcs.sort(key=lambda p: (0 if p.name == "GLOSSARY.md" else 1, p.name))

    try:
        index_page = load_page(index_src, root_dir, "index")
        lessons = [load_page(p, root_dir, "lesson") for p in lesson_srcs]
        refs = [load_page(p, root_dir, "reference") for p in ref_srcs]
    except ValueError as exc:
        print(f"[ERROR] {exc}", file=sys.stderr)
        return 1

    all_pages = [index_page, *lessons, *refs]

    for page in all_pages:
        body_html = render_page_body(page, lessons, refs, root_dir, md, errors)
        nav_html = build_nav_html(page, lessons, refs)
        pager_html = build_lesson_pager_html(page, lessons)
        full_html = wrap_document_html(page, nav_html, body_html, pager_html)

        dest_path = out_dir / page.out_rel
        dest_path.parent.mkdir(parents=True, exist_ok=True)
        dest_path.write_text(full_html, encoding="utf-8")

    if out_dir != root_dir:
        (out_dir / ".nojekyll").write_text("", encoding="utf-8")
        assets_src = root_dir / "assets"
        assets_dst = out_dir / "assets"
        if assets_src.is_dir():
            if assets_dst.exists():
                shutil.rmtree(assets_dst)
            shutil.copytree(assets_src, assets_dst)

    if errors:
        for err in errors:
            print(f"[CHECK ERROR] {err}", file=sys.stderr)
        return 1

    print(
        f"[OK] Compiled {len(all_pages)} pages ({len(lessons)} lessons, {len(refs)} reference sheets) -> {out_dir}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
