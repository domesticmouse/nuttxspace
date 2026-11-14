import re

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
FENCE_RE = re.compile(r"^\s*(`{3,}|~{3,})")


def on_page_markdown(markdown, **kwargs):
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
