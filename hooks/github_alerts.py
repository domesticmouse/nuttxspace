"""MkDocs hook for GitHub alerts, experiment artifacts, tutorial compilation, and link rewriting."""

from __future__ import annotations

import pathlib
import posixpath
import re
import shutil
import subprocess
import sys
import tempfile

import yaml
from markdown.extensions.toc import slugify
from mkdocs.structure.files import File, InclusionLevel

REPO_ROOT = pathlib.Path(__file__).resolve().parent.parent
DOCS_ROOT = REPO_ROOT / "docs"

SOURCE_DIRS = (
    "vanilla/xiao-rp2350-bargraph",
    "egm/xiao-rp2350-bargraph",
    "egm/tutorial/apps",
)
EXTRA_SOURCE_FILES = (
    "egm/tutorial/build.py",
    "egm/tutorial/.gitignore",
)

GITHUB_OWNER_REPO = "domesticmouse/nuttxspace"
GITHUB_NUTTX_REPO = "domesticmouse/nuttx"
GITHUB_APPS_REPO = "domesticmouse/nuttx-apps"

ALERT_MAP = {
    "NOTE": "note",
    "TIP": "tip",
    "IMPORTANT": "important",
    "WARNING": "warning",
    "CAUTION": "danger",
}

ALERT_RE = re.compile(
    r"^(\s*)>\s*\[!(NOTE|TIP|IMPORTANT|WARNING|CAUTION)\](?:\s+(.*))?$"
)
FENCE_RE = re.compile(r"^\s*(`{3,}|~{3,})(.*)$")
HEADING_RE = re.compile(r"^(#{1,6})\s+(.*?)(?:\s*\{#([a-zA-Z0-9_-]+)\})?\s*$")
ATTR_RE = re.compile(r'([a-zA-Z0-9_-]+)="([^"]*)"')
CAPTION_RE = re.compile(r'^([a-zA-Z0-9_-]+)\s+caption="([^"]*)"\s*$')
WIDGET_LINE_RE = re.compile(r"^\s*\{\{widget:([a-zA-Z0-9_-]+)\}\}\s*$")
BLANK_TOKEN_RE = re.compile(r"\{\{blank:([^|}]+)(?:\|[^}]*)?\}\}")
SIDENOTE_RE = re.compile(r"\{\{sidenote:((?:[^{}]|\{[^{}]*\})+)\}\}")
LINK_OR_CODE_RE = re.compile(
    r"(!?\[(?:`[^`\n]*`|[^`\]\n])+\]\()([^)\s]+)(\))|(`+)[^`\n]*?\4"
)
LINE_FRAG_RE = re.compile(r"^L(\d+)(?:-L\d+)?$")

_WRAPPED_SOURCES: set[str] = set()
_WRAPPED_SOURCE_MD_URIS: set[str] = set()
_REFERENCED_LINE_ANCHORS: dict[str, set[str]] = {}
_HEADING_CACHE: dict[pathlib.Path, list[tuple[int, str]]] = {}


def _collect_wrapped_sources() -> list[str]:
    """Collect captured non-Markdown, non-SVG source paths under docs/."""
    sources: list[str] = []
    for sub in SOURCE_DIRS:
        base = DOCS_ROOT / sub
        if base.is_dir():
            for path in sorted(base.rglob("*")):
                if path.is_file() and path.suffix not in (".md", ".svg"):
                    sources.append(path.relative_to(DOCS_ROOT).as_posix())
    for extra in EXTRA_SOURCE_FILES:
        if (DOCS_ROOT / extra).is_file():
            sources.append(extra)
    return sources


def _guess_lang(path_str: str) -> str:
    """Return a valid Pygments lexer name for a captured source file."""
    name = posixpath.basename(path_str)
    if name in ("Makefile", "Make.defs") or name.endswith(".mk"):
        return "makefile"
    if name in ("Kconfig", "defconfig"):
        return "kconfig"
    if name in ("rcS", "rc.sysinit"):
        return "sh"
    ext = posixpath.splitext(name)[1].lower()
    return {
        ".c": "c",
        ".h": "c",
        ".py": "python",
        ".sh": "sh",
    }.get(ext, "text")


def _render_source_viewer_md(src_uri: str) -> str:
    """Build a syntax-highlighted Markdown wrapper page with line numbers."""
    src_path = DOCS_ROOT / src_uri
    content = src_path.read_text(encoding="utf-8")
    max_ticks = max((len(m.group(0)) for m in re.finditer(r"`+", content)), default=0)
    fence = "`" * max(3, max_ticks + 1)
    lang = _guess_lang(src_uri)
    raw_name = posixpath.basename(src_uri)
    gh_url = f"https://github.com/{GITHUB_OWNER_REPO}/blob/main/docs/{src_uri}"
    body = content if content.endswith("\n") or not content else content + "\n"
    return (
        f"# `{src_uri}`\n\n"
        f"[Raw file]({raw_name}) · [View on GitHub]({gh_url})\n\n"
        f'{fence}{lang} linenums="1"\n'
        f"{body}"
        f"{fence}\n"
    )


def _compile_tutorial_html(out_dir: str) -> None:
    """Compile docs/egm/tutorial/ to standalone Tufte HTML pages."""
    build_script = str(DOCS_ROOT / "egm/tutorial/build.py")
    cmd = [sys.executable, build_script, "--out-dir", out_dir, "--check"]
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        uv_cmd = [
            "uv",
            "run",
            "--isolated",
            "--offline",
            "--with",
            "pyyaml",
            "--with",
            "markdown-it-py",
            "python3",
            build_script,
            "--out-dir",
            out_dir,
            "--check",
        ]
        subprocess.run(uv_cmd, check=True, capture_output=True, text=True)


def _collect_referenced_line_anchors() -> dict[str, set[str]]:
    """Scan Markdown files for #L... fragments targeting captured source files."""
    anchors: dict[str, set[str]] = {}
    md_files = [REPO_ROOT / "EXPERIMENT_RUNBOOK.md", *sorted(DOCS_ROOT.rglob("*.md"))]
    for md_file in md_files:
        if not md_file.is_file():
            continue
        in_fence = False
        fence_char = ""
        fence_len = 0
        for line in md_file.read_text(encoding="utf-8").splitlines():
            fm = FENCE_RE.match(line)
            if fm:
                marker = fm.group(1)
                if not in_fence:
                    in_fence = True
                    fence_char = marker[0]
                    fence_len = len(marker)
                elif marker[0] == fence_char and len(marker) >= fence_len and line.strip() == marker:
                    in_fence = False
                continue
            if in_fence:
                continue
            for m in LINK_OR_CODE_RE.finditer(line):
                if not m.group(1):
                    continue
                raw_url = m.group(2)
                if "#" not in raw_url or raw_url.startswith(
                    ("http://", "https://", "mailto:", "conversation://", "file://", "#")
                ):
                    continue
                path_part, frag = raw_url.split("#", 1)
                if not path_part or not LINE_FRAG_RE.match(frag):
                    continue
                resolved = (md_file.parent / path_part).resolve()
                if not resolved.exists():
                    continue
                try:
                    doc_rel = resolved.relative_to(DOCS_ROOT).as_posix()
                except ValueError:
                    continue
                if doc_rel in _WRAPPED_SOURCES:
                    anchors.setdefault(doc_rel, set()).add(frag)
    return anchors


def _extract_headings(md_path: pathlib.Path) -> list[tuple[int, str]]:
    """Extract (line_number, mkdocs_slug) pairs for a Markdown file."""
    cached = _HEADING_CACHE.get(md_path)
    if cached is not None:
        return cached

    lines = md_path.read_text(encoding="utf-8").splitlines()
    headings: list[tuple[int, str]] = []
    used: dict[str, int] = {}
    idx = 0

    if lines and lines[0].strip() == "---":
        for end in range(1, len(lines)):
            if lines[end].strip() in ("---", "..."):
                idx = end + 1
                break

    in_fence = False
    fence_char = ""
    fence_len = 0
    for lineno in range(idx + 1, len(lines) + 1):
        line = lines[lineno - 1]
        fm = FENCE_RE.match(line)
        if fm:
            marker = fm.group(1)
            if not in_fence:
                in_fence = True
                fence_char = marker[0]
                fence_len = len(marker)
            elif marker[0] == fence_char and len(marker) >= fence_len and line.strip() == marker:
                in_fence = False
            continue
        if in_fence:
            continue

        hm = HEADING_RE.match(line)
        if hm:
            raw_title = hm.group(2)
            explicit_id = hm.group(3)
            clean_title = re.sub(r"\[([^\]]+)\]\([^)]+\)", r"\1", raw_title)
            clean_title = re.sub(r"`([^`]*)`", r"\1", clean_title)
            clean_title = clean_title.replace("*", "")
            base_slug = explicit_id if explicit_id else slugify(clean_title, "-")
            count = used.get(base_slug, 0)
            used[base_slug] = count + 1
            actual_slug = f"{base_slug}_{count}" if count else base_slug
            headings.append((lineno, actual_slug))

    _HEADING_CACHE[md_path] = headings
    return headings


def _heading_slug_for_line(md_path: pathlib.Path, line_no: int) -> str:
    """Return the nearest MkDocs heading anchor at or above line_no."""
    headings = _extract_headings(md_path)
    best = ""
    for h_line, slug in headings:
        if h_line <= line_no:
            best = slug
        else:
            break
    return f"#{best}" if best else ""


def on_files(files, config):
    """Add root runbook, cast players, compiled tutorial HTML, and source viewer pages."""
    global _WRAPPED_SOURCES, _WRAPPED_SOURCE_MD_URIS, _REFERENCED_LINE_ANCHORS
    _WRAPPED_SOURCES = set(_collect_wrapped_sources())
    _WRAPPED_SOURCE_MD_URIS = {f"{s}.md" for s in _WRAPPED_SOURCES}
    _REFERENCED_LINE_ANCHORS = _collect_referenced_line_anchors()
    _HEADING_CACHE.clear()

    # 1. Root EXPERIMENT_RUNBOOK.md
    if files.get_file_from_path("EXPERIMENT_RUNBOOK.md") is None:
        files.append(
            File(
                "EXPERIMENT_RUNBOOK.md",
                src_dir=str(REPO_ROOT),
                dest_dir=config["site_dir"],
                use_directory_urls=config["use_directory_urls"],
                inclusion=InclusionLevel.INCLUDED,
            )
        )

    # 2. Standalone HTML cast players under prototypes/
    proto_dir = REPO_ROOT / "prototypes"
    if proto_dir.is_dir():
        for html_file in sorted(proto_dir.glob("*.html")):
            src_uri = f"prototypes/{html_file.name}"
            if files.get_file_from_path(src_uri) is None:
                files.append(
                    File(
                        src_uri,
                        src_dir=str(REPO_ROOT),
                        dest_dir=config["site_dir"],
                        use_directory_urls=config["use_directory_urls"],
                        inclusion=InclusionLevel.NOT_IN_NAV,
                    )
                )

    # 2b. Presentation specification, script, map, issues, and HTML prototypes under .scratch/egm-nuttx-casts-presentation/
    pres_dir = REPO_ROOT / ".scratch" / "egm-nuttx-casts-presentation"
    if pres_dir.is_dir():
        scratch_root = REPO_ROOT / ".scratch"
        for pres_file in sorted(pres_dir.rglob("*")):
            if not pres_file.is_file() or pres_file.suffix not in (".md", ".html"):
                continue
            src_uri = pres_file.relative_to(scratch_root).as_posix()
            if files.get_file_from_path(src_uri) is None:
                files.append(
                    File(
                        src_uri,
                        src_dir=str(scratch_root),
                        dest_dir=config["site_dir"],
                        use_directory_urls=config["use_directory_urls"],
                        inclusion=(
                            InclusionLevel.INCLUDED
                            if pres_file.suffix == ".md"
                            else InclusionLevel.NOT_IN_NAV
                        ),
                    )
                )

    # 2c. Register placeholder for compiled Slidev deck at slides/index.html (populated in on_post_build)
    if files.get_file_from_path("slides/index.html") is None:
        files.append(
            File.generated(
                config,
                "slides/index.html",
                content="",
                inclusion=InclusionLevel.NOT_IN_NAV,
            )
        )

    # 3. Un-exclude .gitignore files inside docs/ so documentation links resolve
    for f in list(files):
        if f.src_uri.endswith("/.gitignore") or f.src_uri == ".gitignore":
            f.inclusion = InclusionLevel.NOT_IN_NAV

    # 4. Separate egm/tutorial/README.md and egm/tutorial/index.md destinations
    readme_f = files.get_file_from_path("egm/tutorial/README.md")
    if readme_f is not None:
        files.remove(readme_f)
    files.append(
        File(
            "egm/tutorial/README.md",
            src_dir=config["docs_dir"],
            dest_dir=config["site_dir"],
            use_directory_urls=config["use_directory_urls"],
            dest_uri="egm/tutorial/README/index.html",
            inclusion=InclusionLevel.INCLUDED,
        )
    )

    idx_f = files.get_file_from_path("egm/tutorial/index.md")
    if idx_f is not None:
        files.remove(idx_f)
    files.append(
        File(
            "egm/tutorial/index.md",
            src_dir=config["docs_dir"],
            dest_dir=config["site_dir"],
            use_directory_urls=config["use_directory_urls"],
            dest_uri="egm/tutorial/course-index/index.html",
            inclusion=InclusionLevel.INCLUDED,
        )
    )

    # 5. Compile egm/tutorial via build.py and register generated HTML files
    with tempfile.TemporaryDirectory() as tmp_tut:
        _compile_tutorial_html(tmp_tut)
        tmp_path = pathlib.Path(tmp_tut)
        for html_file in sorted(tmp_path.rglob("*.html")):
            rel = html_file.relative_to(tmp_path).as_posix()
            src_uri = f"egm/tutorial/{rel}"
            if files.get_file_from_path(src_uri) is None:
                files.append(
                    File.generated(
                        config,
                        src_uri,
                        content=html_file.read_text(encoding="utf-8"),
                        inclusion=InclusionLevel.NOT_IN_NAV,
                    )
                )

    # 6. Generate syntax-highlighted HTML viewer pages for captured source files
    for src_uri in sorted(_WRAPPED_SOURCES):
        md_uri = f"{src_uri}.md"
        if files.get_file_from_path(md_uri) is None:
            viewer = File(
                md_uri,
                src_dir=None,
                dest_dir=config["site_dir"],
                use_directory_urls=config["use_directory_urls"],
                dest_uri=f"{src_uri}.html",
                inclusion=InclusionLevel.NOT_IN_NAV,
            )
            viewer.generated_by = "github_alerts"
            viewer._content = _render_source_viewer_md(src_uri)
            files.append(viewer)

    return files


def _indent_admonition(kind: str, title: str, body_lines: list[str]) -> list[str]:
    """Format an MkDocs Material admonition block from title and body lines."""
    header = f'!!! {kind} "{title}"' if title else f"!!! {kind}"
    out = [header]
    for line in body_lines:
        out.append(f"    {line}" if line else "")
    out.append("")
    return out


def _render_quiz_admonition(title: str, badge: str, body_lines: list[str]) -> list[str]:
    """Convert a YAML quiz directive into a Material question admonition."""
    full_title = f"{title} ({badge})" if badge else title
    out = [f'!!! question "{full_title}"']
    try:
        questions = yaml.safe_load("\n".join(body_lines)) or []
    except yaml.YAMLError:
        return _indent_admonition("question", full_title, body_lines)

    for idx, q in enumerate(questions, 1):
        q_text = q.get("question", "")
        out.append(f"    **{idx}. {q_text}**")
        out.append("")
        correct_opt = None
        for opt in q.get("options", []):
            letter = opt.get("letter", "")
            text = opt.get("text", "")
            out.append(f"    - **{letter}.** {text}")
            if opt.get("correct"):
                correct_opt = opt
        out.append("")
        if correct_opt is not None:
            out.append('    ??? success "Show Answer"')
            out.append(
                f'        **{correct_opt.get("letter", "")}**: {correct_opt.get("explain", "")}'
            )
            out.append("")
    return out


def _render_exercise_admonition(title: str, badge: str, body_lines: list[str]) -> list[str]:
    """Convert an exercise directive (with nested prompts) into Material markdown."""
    full_title = f"{title} ({badge})" if badge else title
    cleaned: list[str] = []
    in_prompts = False
    for line in body_lines:
        stripped = line.strip()
        if stripped.startswith(":::prompts"):
            in_prompts = True
            continue
        if in_prompts and stripped == ":::":
            in_prompts = False
            continue
        if in_prompts:
            if stripped.startswith("hint:"):
                val = stripped[len("hint:") :].strip().strip('"')
                cleaned.append(f"**Hint:** {val}")
            elif stripped.startswith("success:"):
                val = stripped[len("success:") :].strip().strip('"')
                cleaned.append(f"**Goal:** {val}")
            elif stripped:
                cleaned.append(stripped)
            continue
        cleaned.append(BLANK_TOKEN_RE.sub(r"\1", line))
    return _indent_admonition("abstract", full_title, cleaned)


def _transform_tutorial_directives(markdown: str, src_uri: str) -> str:
    """Convert /teach-code custom directives and widgets into MkDocs Material markdown."""
    lines = markdown.splitlines()
    out: list[str] = []

    html_target = ""
    html_label = ""
    if src_uri == "egm/tutorial/index.md":
        html_target = "index.html"
        html_label = "Open Interactive Tufte HTML Course"
    elif src_uri.startswith("egm/tutorial/lessons/"):
        stem = posixpath.splitext(posixpath.basename(src_uri))[0]
        html_target = f"{stem}.html"
        html_label = "Open Interactive Lesson (Tufte HTML)"
    elif src_uri.startswith("egm/tutorial/reference/"):
        stem = posixpath.splitext(posixpath.basename(src_uri))[0]
        out_name = "glossary.html" if stem == "GLOSSARY" else f"{stem}.html"
        html_target = out_name
        html_label = "Open Printable Reference Sheet (Tufte HTML)"

    if html_target:
        out.extend(
            [
                '!!! tip "Interactive Course Version (Tufte HTML)"',
                f"    This page is also compiled with interactive widgets, quizzes, and print styles: [{html_label}]({html_target}).",
                "",
            ]
        )

    i = 0
    in_fence = False
    fence_char = ""
    fence_len = 0
    pending_mermaid_caption = ""

    while i < len(lines):
        line = lines[i]
        fm = FENCE_RE.match(line)
        if fm:
            marker = fm.group(1)
            info = fm.group(2).strip()
            if not in_fence:
                in_fence = True
                fence_char = marker[0]
                fence_len = len(marker)
                cm = CAPTION_RE.match(info)
                if cm:
                    lang, caption = cm.group(1), cm.group(2)
                    if lang == "mermaid":
                        pending_mermaid_caption = caption
                        out.append(marker + "mermaid")
                    else:
                        out.append(f'{marker}{lang} title="{caption}"')
                else:
                    out.append(line)
                i += 1
                continue
            elif marker[0] == fence_char and len(marker) >= fence_len and line.strip() == marker:
                in_fence = False
                out.append(line)
                if pending_mermaid_caption:
                    out.append(f"*{pending_mermaid_caption}*")
                    pending_mermaid_caption = ""
                i += 1
                continue

        if in_fence:
            out.append(BLANK_TOKEN_RE.sub(r"\1", line))
            i += 1
            continue

        stripped = line.lstrip()
        if stripped.startswith(":::") and stripped != ":::":
            header = stripped[3:].strip()
            directive = header.split()[0]
            attrs = {m.group(1): m.group(2) for m in ATTR_RE.finditer(header)}
            depth = 1
            body: list[str] = []
            i += 1
            while i < len(lines) and depth > 0:
                cur = lines[i]
                cur_s = cur.strip()
                if cur_s.startswith(":::") and cur_s != ":::":
                    depth += 1
                elif cur_s == ":::":
                    depth -= 1
                    if depth == 0:
                        i += 1
                        break
                body.append(cur)
                i += 1

            title = attrs.get("title", "")
            badge = attrs.get("badge", "")
            if directive == "callout":
                out.extend(_indent_admonition("info", title, body))
            elif directive == "callout-warn":
                out.extend(_indent_admonition("warning", title, body))
            elif directive == "teacher-prompt":
                out.extend(_indent_admonition("tip", title, body))
            elif directive == "verify":
                full_title = f"{title} ({badge})" if badge else title
                out.extend(_indent_admonition("success", full_title, body))
            elif directive == "exercise":
                out.extend(_render_exercise_admonition(title, badge, body))
            elif directive == "quiz":
                out.extend(_render_quiz_admonition(title, badge, body))
            else:
                out.extend(_indent_admonition("note", title, body))
            continue

        wm = WIDGET_LINE_RE.match(line)
        if wm:
            wid = wm.group(1)
            if wid == "lesson-cards":
                out.extend(
                    [
                        "1. [Lesson 0001 — Breadboard Circuit, Pinout & 23.6 mA Current Budget](lessons/0001-breadboard-circuit-and-current-budget.md) ([Interactive HTML](lessons/0001-breadboard-circuit-and-current-budget.html))",
                        "2. [Lesson 0002 — Proportional 6-Segment Bar Graph Math & 16-Bit PWM Scaling](lessons/0002-proportional-bargraph-math-and-pwm-scaling.md) ([Interactive HTML](lessons/0002-proportional-bargraph-math-and-pwm-scaling.html))",
                        "3. [Lesson 0003 — Fixed-Point Q4 EMA Filtering, Deadbands & ADC Sample Extraction](lessons/0003-q4-ema-filtering-and-adc-extraction.md) ([Interactive HTML](lessons/0003-q4-ema-filtering-and-adc-extraction.html))",
                        "4. [Lesson 0004 — Multi-Channel RP2350 PWM Routing & Safe Hardware Shutdown](lessons/0004-multichannel-pwm-routing-and-safe-shutdown.md) ([Interactive HTML](lessons/0004-multichannel-pwm-routing-and-safe-shutdown.html))",
                        "5. [Lesson 0005 — Out-of-Tree NuttX Board Bringup, RP2350-E9 Pad Isolation & NVIC IRQ Gating](lessons/0005-out-of-tree-bsp-and-adc-irq-gating.md) ([Interactive HTML](lessons/0005-out-of-tree-bsp-and-adc-irq-gating.html))",
                        "6. [Lesson 0006 — The 50 Hz `bargraphd` Daemon State Machine & Thread-Safe NSH CLI](lessons/0006-bargraphd-daemon-and-nsh-cli.md) ([Interactive HTML](lessons/0006-bargraphd-daemon-and-nsh-cli.html))",
                    ]
                )
            elif wid == "reference-cards":
                out.extend(
                    [
                        "- [Glossary of XIAO RP2350 & Apache NuttX Terms](reference/GLOSSARY.md) ([Printable HTML](reference/glossary.html))",
                        "- [Hardware Pinout, Breadboard Coordinates & Current Budget](reference/hardware-pinout-and-wiring.md) ([Printable HTML](reference/hardware-pinout-and-wiring.html))",
                        "- [7 Upstream NuttX & RP2350 Hardware/Driver Traps](reference/nuttx-rp2350-driver-traps.md) ([Printable HTML](reference/nuttx-rp2350-driver-traps.html))",
                        "- [Verification, Testing & NSH CLI Cheat Sheet](reference/verification-and-cli-cheatsheet.md) ([Printable HTML](reference/verification-and-cli-cheatsheet.html))",
                    ]
                )
            elif html_target:
                out.extend(
                    [
                        f'!!! note "Interactive Simulator: `{wid}`"',
                        f"    Open [{html_label}]({html_target}) to run the interactive `{wid}` widget in your browser.",
                    ]
                )
            i += 1
            continue

        line = SIDENOTE_RE.sub(lambda m: f" *(Note: {m.group(1).strip()})*", line)
        out.append(line)
        i += 1

    return "\n".join(out) + ("\n" if markdown.endswith("\n") else "")


def _append_source_inventory(markdown: str, src_uri: str) -> str:
    """Append a navigable table of captured source files to snapshot READMEs."""
    prefix_map = {
        "vanilla/xiao-rp2350-bargraph/README.md": "vanilla/xiao-rp2350-bargraph/",
        "egm/xiao-rp2350-bargraph/README.md": "egm/xiao-rp2350-bargraph/",
        "egm/tutorial/README.md": "egm/tutorial/",
    }
    prefix = prefix_map.get(src_uri)
    if not prefix:
        return markdown

    matching = sorted(s for s in _WRAPPED_SOURCES if s.startswith(prefix))
    if not matching:
        return markdown

    rows = [
        "",
        "## Captured Source Files",
        "",
        "| File | Viewer | Raw |",
        "| :--- | :--- | :--- |",
    ]
    for s in matching:
        rel = s[len(prefix) :]
        rows.append(f"| `{rel}` | [View source]({rel}.md) | [Raw]({rel}) |")
    return markdown.rstrip() + "\n" + "\n".join(rows) + "\n"


def _rewrite_relative_links(markdown: str, src_uri: str) -> str:
    """Rewrite repo-relative Markdown links to site pages, source viewers, or GitHub URLs."""
    is_vanilla = src_uri.startswith(("vanilla/", "prompts-vanilla/"))
    sub_branch = "master" if is_vanilla else "egm"

    if src_uri == "EXPERIMENT_RUNBOOK.md":
        phys_dir = REPO_ROOT
    elif src_uri.startswith("egm-nuttx-casts-presentation/"):
        phys_dir = (REPO_ROOT / ".scratch" / posixpath.dirname(src_uri)).resolve()
    else:
        phys_dir = (DOCS_ROOT / posixpath.dirname(src_uri)).resolve()
    doc_dir = posixpath.dirname(src_uri)

    def _resolve_target(m: re.Match[str]) -> str:
        if not m.group(1):
            return m.group(0)
        prefix, raw_url, suffix = m.group(1), m.group(2), m.group(3)
        if raw_url.startswith(("http://", "https://", "mailto:", "conversation://", "file://", "#")):
            return m.group(0)

        if "[" in raw_url or "..." in raw_url:
            return f"`{prefix}{raw_url}{suffix}`"

        if "#" in raw_url:
            path_part, frag = raw_url.split("#", 1)
            frag_with_hash = "#" + frag
        else:
            path_part, frag, frag_with_hash = raw_url, "", ""

        if not path_part:
            return m.group(0)

        resolved = (phys_dir / path_part).resolve()
        if not resolved.exists() and src_uri.startswith("egm/"):
            cleaned = path_part.lstrip("./")
            if cleaned.startswith("xiao-rp2350-bargraph/"):
                egm_candidate = (DOCS_ROOT / "egm" / cleaned).resolve()
                if egm_candidate.exists():
                    resolved = egm_candidate

        if not resolved.exists():
            return m.group(0)

        try:
            repo_rel = resolved.relative_to(REPO_ROOT).as_posix()
        except ValueError:
            return m.group(0)

        if repo_rel == "EXPERIMENT_RUNBOOK.md":
            rel_to_doc = posixpath.relpath("EXPERIMENT_RUNBOOK.md", doc_dir or ".")
            lm = LINE_FRAG_RE.match(frag)
            if lm:
                frag_with_hash = _heading_slug_for_line(resolved, int(lm.group(1)))
            return f"{prefix}{rel_to_doc}{frag_with_hash}{suffix}"

        if repo_rel.startswith("prototypes/") and resolved.is_file():
            rel_to_doc = posixpath.relpath(repo_rel, doc_dir or ".")
            return f"{prefix}{rel_to_doc}{frag_with_hash}{suffix}"

        if repo_rel.startswith(".scratch/egm-nuttx-casts-presentation/") and resolved.is_file():
            pres_target = repo_rel[len(".scratch/") :]
            if resolved.suffix == ".md":
                lm = LINE_FRAG_RE.match(frag)
                if lm:
                    frag_with_hash = _heading_slug_for_line(resolved, int(lm.group(1)))
            rel_to_doc = posixpath.relpath(pres_target, doc_dir or ".")
            return f"{prefix}{rel_to_doc}{frag_with_hash}{suffix}"

        if repo_rel in ("slides", "slides/slides.md"):
            rel_to_doc = posixpath.relpath("slides/index.html", doc_dir or ".")
            return f"{prefix}{rel_to_doc}{frag_with_hash}{suffix}"

        if repo_rel.startswith("docs/"):
            doc_target = repo_rel[len("docs/") :]
            if resolved.is_dir():
                if (resolved / "README.md").is_file():
                    doc_target = f"{doc_target.rstrip('/')}/README.md"
                else:
                    return (
                        f"{prefix}https://github.com/{GITHUB_OWNER_REPO}/tree/main/"
                        f"{repo_rel}{frag_with_hash}{suffix}"
                    )
            elif doc_target in _WRAPPED_SOURCES:
                doc_target = f"{doc_target}.md"
            elif resolved.suffix == ".md":
                lm = LINE_FRAG_RE.match(frag)
                if lm:
                    frag_with_hash = _heading_slug_for_line(resolved, int(lm.group(1)))

            rel_to_doc = posixpath.relpath(doc_target, doc_dir or ".")
            return f"{prefix}{rel_to_doc}{frag_with_hash}{suffix}"

        if repo_rel.startswith("nuttx/"):
            sub_path = repo_rel[len("nuttx/") :]
            kind = "tree" if resolved.is_dir() else "blob"
            return (
                f"{prefix}https://github.com/{GITHUB_NUTTX_REPO}/{kind}/"
                f"{sub_branch}/{sub_path}{frag_with_hash}{suffix}"
            )

        if repo_rel.startswith("apps/"):
            sub_path = repo_rel[len("apps/") :]
            kind = "tree" if resolved.is_dir() else "blob"
            return (
                f"{prefix}https://github.com/{GITHUB_APPS_REPO}/{kind}/"
                f"{sub_branch}/{sub_path}{frag_with_hash}{suffix}"
            )

        kind = "tree" if resolved.is_dir() else "blob"
        return (
            f"{prefix}https://github.com/{GITHUB_OWNER_REPO}/{kind}/main/"
            f"{repo_rel}{frag_with_hash}{suffix}"
        )

    lines = markdown.splitlines()
    out: list[str] = []
    in_fence = False
    fence_char = ""
    fence_len = 0

    for line in lines:
        fm = FENCE_RE.match(line)
        if fm:
            marker = fm.group(1)
            if not in_fence:
                in_fence = True
                fence_char = marker[0]
                fence_len = len(marker)
            elif marker[0] == fence_char and len(marker) >= fence_len and line.strip() == marker:
                in_fence = False
            out.append(line)
            continue
        if not in_fence:
            line = LINK_OR_CODE_RE.sub(_resolve_target, line)
        out.append(line)

    return "\n".join(out) + ("\n" if markdown.endswith("\n") else "")


def _convert_github_alerts(markdown: str) -> str:
    """Convert GitHub alert blockquotes into MkDocs Material admonitions."""
    lines = markdown.splitlines()
    out = []
    i = 0
    fence_char = None
    fence_len = 0

    while i < len(lines):
        line = lines[i]
        fence_match = FENCE_RE.match(line)
        if fence_match:
            marker = fence_match.group(1)
            if fence_char is None:
                fence_char = marker[0]
                fence_len = len(marker)
            elif (
                marker[0] == fence_char
                and len(marker) >= fence_len
                and line.strip() == marker
            ):
                fence_char = None
                fence_len = 0
            out.append(line)
            i += 1
            continue

        if fence_char is not None:
            out.append(line)
            i += 1
            continue

        alert_match = ALERT_RE.match(line)
        if alert_match:
            indent = alert_match.group(1)
            alert_type = ALERT_MAP[alert_match.group(2)]
            custom_title = (alert_match.group(3) or "").strip()
            if custom_title:
                out.append(f'{indent}!!! {alert_type} "{custom_title}"')
            else:
                out.append(f"{indent}!!! {alert_type}")

            i += 1
            while i < len(lines):
                nxt = lines[i]
                stripped = nxt.lstrip()
                if stripped.startswith(">"):
                    content = stripped[1:]
                    if content.startswith(" "):
                        content = content[1:]
                    out.append(f"{indent}    {content}" if content else indent)
                    i += 1
                else:
                    break

            if i < len(lines) and lines[i].strip() != "":
                out.append("")
            continue

        out.append(line)
        i += 1

    return "\n".join(out) + ("\n" if markdown.endswith("\n") else "")


def _prepend_presentation_banner(markdown: str, src_uri: str) -> str:
    """Prepend a navigation banner on egm-nuttx-casts-presentation pages linking to the Slidev deck."""
    doc_dir = posixpath.dirname(src_uri)
    slides_rel = posixpath.relpath("slides/index.html", doc_dir or ".")
    extra_links = ""
    if src_uri == "egm-nuttx-casts-presentation/slide-structure-prototype.md":
        extra_links = " · [Open Interactive Timing Planner (HTML)](slide-structure-prototype.html)"
    elif src_uri == "egm-nuttx-casts-presentation/cast-curation-prototype.md":
        extra_links = " · [Open Interactive Cast Curation Matrix (HTML)](cast-curation-prototype.html)"
    elif src_uri in (
        "egm-nuttx-casts-presentation/spec.md",
        "egm-nuttx-casts-presentation/map.md",
        "egm-nuttx-casts-presentation/speaker-script-bjm-voice.md",
    ):
        extra_links = (
            " · [Timing Planner (HTML)](slide-structure-prototype.html)"
            " · [Cast Curation Matrix (HTML)](cast-curation-prototype.html)"
        )

    banner = [
        '!!! tip "Live Slidev Presentation Deck"',
        f"    [**Launch Slidev Presentation Deck**]({slides_rel}) · "
        f"[Slide Overview]({slides_rel}#/overview) · "
        f"[Presenter Console]({slides_rel}#/presenter){extra_links}",
        "",
    ]
    return "\n".join(banner) + markdown


def on_page_markdown(markdown, page, config, files):
    """Transform directives, normalize links, and convert GitHub alerts."""
    src_uri = page.file.src_uri
    if src_uri in _WRAPPED_SOURCE_MD_URIS:
        return markdown

    if src_uri.startswith("egm/tutorial/"):
        markdown = _transform_tutorial_directives(markdown, src_uri)
    elif src_uri.startswith("egm-nuttx-casts-presentation/"):
        markdown = _prepend_presentation_banner(markdown, src_uri)

    markdown = _append_source_inventory(markdown, src_uri)
    markdown = _rewrite_relative_links(markdown, src_uri)
    return _convert_github_alerts(markdown)


def on_post_build(config):
    """Copy compiled Slidev presentation deck (slides/dist) into site/slides."""
    slides_dist = REPO_ROOT / "slides" / "dist"
    if not (slides_dist / "index.html").is_file():
        return
    dest_slides = pathlib.Path(config["site_dir"]) / "slides"
    shutil.copytree(slides_dist, dest_slides, dirs_exist_ok=True)


def on_page_content(html, page, config, files):
    """Expose #L<n> line anchors and register referenced #L<a>-L<b> ranges."""
    src_uri = page.file.src_uri
    if src_uri not in _WRAPPED_SOURCE_MD_URIS:
        return html

    raw_uri = src_uri[: -len(".md")]
    html = re.sub(r'\bid="__codelineno-0-(\d+)"', r'id="L\1"', html)
    html = re.sub(r'\bname="__codelineno-0-(\d+)"', r'name="L\1"', html)
    html = re.sub(r'\bhref="#__codelineno-0-(\d+)"', r'href="#L\1"', html)

    if page.present_anchor_ids is not None:
        line_ids = re.findall(r'\bid="(L\d+)"', html)
        page.present_anchor_ids.update(line_ids)
        page.present_anchor_ids.update(_REFERENCED_LINE_ANCHORS.get(raw_uri, ()))

    return html
