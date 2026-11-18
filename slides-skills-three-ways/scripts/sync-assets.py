#!/usr/bin/env python3
"""Sync canonical casts, breadboard SVGs, zoomed SVG crops, HTML prototypes, and tutorial HTML into slides-skills-three-ways/public/."""

from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path


def make_cropped_svg(src_svg: Path, dst_svg: Path, view_box: str, width: str, height: str) -> None:
    text = src_svg.read_text(encoding="utf-8")

    # Update root <svg ...> viewBox, width, and height attributes cleanly
    def repl(match: re.Match[str]) -> str:
        tag = match.group(0)
        tag = re.sub(r'viewBox="[^"]*"', f'viewBox="{view_box}"', tag, count=1)
        if re.search(r'\bwidth="[^"]*"', tag):
            tag = re.sub(r'\bwidth="[^"]*"', f'width="{width}"', tag, count=1)
        if re.search(r'\bheight="[^"]*"', tag):
            tag = re.sub(r'\bheight="[^"]*"', f'height="{height}"', tag, count=1)
        return tag

    cropped = re.sub(r"<svg\b[^>]*>", repl, text, count=1)
    dst_svg.write_text(cropped, encoding="utf-8")


def build_tutorial_html(repo_root: Path, tutorial_out: Path) -> None:
    tutorial_dir = repo_root / "docs" / "egm" / "tutorial"
    build_script = tutorial_dir / "build.py"
    if not build_script.is_file():
        print(f"[WARN] Tutorial builder not found: {build_script}", file=sys.stderr)
        return

    if tutorial_out.exists():
        shutil.rmtree(tutorial_out)
    tutorial_out.mkdir(parents=True, exist_ok=True)

    # Choose Python interpreter capable of running build.py (pyyaml + markdown-it-py)
    cmd: list[str] | None = None
    venv_py = tutorial_dir / ".venv" / "bin" / "python"
    if venv_py.is_file():
        cmd = [str(venv_py), str(build_script), "--out-dir", str(tutorial_out), "--check"]
    else:
        try:
            import yaml  # noqa: F401
            import markdown_it  # noqa: F401

            cmd = [sys.executable, str(build_script), "--out-dir", str(tutorial_out), "--check"]
        except ImportError:
            uv_bin = shutil.which("uv")
            if uv_bin:
                # Prefer offline cache inside restricted sandboxes, fall back if needed
                offline_cmd = [
                    uv_bin,
                    "run",
                    "--isolated",
                    "--no-project",
                    "--offline",
                    "--with",
                    "pyyaml",
                    "--with",
                    "markdown-it-py",
                    "python3",
                    str(build_script),
                    "--out-dir",
                    str(tutorial_out),
                    "--check",
                ]
                res = subprocess.run(offline_cmd, cwd=str(tutorial_dir))
                if res.returncode == 0:
                    return
                cmd = [
                    uv_bin,
                    "run",
                    "--isolated",
                    "--no-project",
                    "--with",
                    "pyyaml",
                    "--with",
                    "markdown-it-py",
                    "python3",
                    str(build_script),
                    "--out-dir",
                    str(tutorial_out),
                    "--check",
                ]

    if cmd is None:
        raise RuntimeError(
            "Unable to build tutorial HTML: neither pyyaml/markdown_it nor uv found."
        )

    res = subprocess.run(cmd, cwd=str(tutorial_dir))
    if res.returncode != 0:
        raise RuntimeError(f"Tutorial build.py failed with exit code {res.returncode}")

    # Keep docs/egm/tutorial/ clean of transient generated HTML if any appeared
    for stray in [
        tutorial_dir / "index.html",
        *(tutorial_dir / "lessons").glob("*.html"),
        *(tutorial_dir / "reference").glob("*.html"),
    ]:
        if stray.exists():
            stray.unlink()


def main() -> int:
    slides_dir = Path(__file__).resolve().parent.parent
    repo_root = slides_dir.parent
    public_dir = slides_dir / "public"
    casts_dir = public_dir / "casts"
    images_dir = public_dir / "images"
    prototypes_dir = public_dir / "prototypes"
    tutorial_out = public_dir / "tutorial"

    casts_dir.mkdir(parents=True, exist_ok=True)
    images_dir.mkdir(parents=True, exist_ok=True)
    prototypes_dir.mkdir(parents=True, exist_ok=True)

    # 1. Sync final .cast recordings
    src_casts_dir = repo_root / "recordings" / "final"
    expected_casts = [
        "vanilla-vibe.cast",
        "01-bootstrap.cast",
        "02-brainstorm.cast",
        "03-draft.cast",
        "04-review.cast",
        "05-implement.cast",
        "06-teach-code.cast",
    ]
    for name in expected_casts:
        src = src_casts_dir / name
        if not src.is_file():
            raise FileNotFoundError(f"Missing canonical cast file: {src}")
        shutil.copy2(src, casts_dir / name)

    # 2. Copy breadboard SVGs, brand logos, and generate zoomed viewBox crops
    egm_svg = repo_root / "docs" / "egm" / "breadboard-wiring.svg"
    vanilla_svg = repo_root / "docs" / "vanilla" / "breadboard_circuit.svg"
    assets_scopes_svg = repo_root / "docs" / "assets" / "breadboard-scopes.svg"
    assets_wiring_svg = repo_root / "docs" / "assets" / "breadboard-wiring.svg"
    nuttx_logo_png = repo_root / "docs" / "assets" / "nuttx-logo.png"
    antigravity_logo_svg = repo_root / "docs" / "assets" / "antigravity-lockup.svg"
    antigravity_icon_png = repo_root / "docs" / "assets" / "antigravity-icon__full-color.png"
    if (
        not egm_svg.is_file()
        or not vanilla_svg.is_file()
        or not assets_scopes_svg.is_file()
        or not assets_wiring_svg.is_file()
        or not nuttx_logo_png.is_file()
        or not antigravity_logo_svg.is_file()
        or not antigravity_icon_png.is_file()
    ):
        raise FileNotFoundError("Missing image asset source files.")

    shutil.copy2(egm_svg, images_dir / "breadboard-wiring.svg")
    make_cropped_svg(
        vanilla_svg,
        images_dir / "breadboard_circuit.svg",
        view_box="0 0 1200 760",
        width="1200",
        height="760",
    )
    shutil.copy2(assets_scopes_svg, images_dir / "breadboard-scopes.svg")
    shutil.copy2(assets_wiring_svg, images_dir / "assets-breadboard-wiring.svg")
    shutil.copy2(nuttx_logo_png, images_dir / "nuttx-logo.png")
    shutil.copy2(antigravity_logo_svg, images_dir / "antigravity-lockup.svg")
    shutil.copy2(antigravity_icon_png, images_dir / "antigravity-icon__full-color.png")

    make_cropped_svg(
        egm_svg,
        images_dir / "breadboard-wiring-crop-ef.svg",
        view_box="150 130 500 500",
        width="500",
        height="500",
    )
    make_cropped_svg(
        vanilla_svg,
        images_dir / "breadboard-vanilla-crop-ch.svg",
        view_box="120 180 320 320",
        width="320",
        height="320",
    )

    # 3. Copy interactive HTML planning prototypes into slides-skills-three-ways/public/prototypes/
    proto_src_dir = repo_root / ".scratch" / "egm-nuttx-casts-presentation"
    expected_prototypes = [
        "cast-curation-prototype.html",
        "slide-structure-prototype.html",
    ]
    for proto_name in expected_prototypes:
        src = proto_src_dir / proto_name
        if not src.is_file():
            raise FileNotFoundError(f"Missing HTML prototype file: {src}")
        shutil.copy2(src, prototypes_dir / proto_name)

    # 4. Compile interactive tutorial HTML into slides-skills-three-ways/public/tutorial/
    build_tutorial_html(repo_root, tutorial_out)

    print(
        f"[OK] Synced {len(expected_casts)} casts, 6 SVGs, {len(expected_prototypes)} HTML prototypes, and compiled tutorial -> {public_dir}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
