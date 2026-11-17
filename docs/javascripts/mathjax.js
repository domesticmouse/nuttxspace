window.MathJax = {
  tex: {
    inlineMath: [["\\(", "\\)"]],
    displayMath: [["\\[", "\\]"]],
    processEscapes: true,
    processEnvironments: true
  },
  options: {
    ignoreHtmlClass: ".*|",
    processHtmlClass: "arithmatex"
  }
};

function clearLineHighlights() {
  document.querySelectorAll("span.hll[data-gh-line]").forEach((span) => {
    const parent = span.parentNode;
    if (!parent) return;
    while (span.firstChild) {
      parent.insertBefore(span.firstChild, span);
    }
    parent.removeChild(span);
  });
  document.querySelectorAll(".linenos a[data-gh-line]").forEach((a) => {
    a.removeAttribute("data-gh-line");
    a.style.backgroundColor = "";
    a.style.color = "";
    a.style.fontWeight = "";
    a.style.borderRadius = "";
    a.style.padding = "";
  });
}

function highlightCodeLinesFromHash() {
  clearLineHighlights();
  const m = window.location.hash.match(/^#L(\d+)(?:-L?(\d+))?$/);
  if (!m) return;

  const start = parseInt(m[1], 10);
  const end = m[2] ? Math.max(start, parseInt(m[2], 10)) : start;

  for (let line = start; line <= end; line++) {
    document.querySelectorAll(`.linenos a[href="#L${line}"]`).forEach((a) => {
      a.setAttribute("data-gh-line", "1");
      a.style.backgroundColor = "var(--md-accent-fg-color--transparent, rgba(0, 150, 136, 0.25))";
      a.style.color = "var(--md-accent-fg-color, #009688)";
      a.style.fontWeight = "700";
      a.style.borderRadius = "2px";
      a.style.padding = "0 2px";
    });

    const anchor =
      document.getElementById(`L${line}`) ||
      document.getElementById(`__codelineno-0-${line}`);
    if (!anchor || !anchor.parentNode) continue;

    const hll = document.createElement("span");
    hll.className = "hll";
    hll.setAttribute("data-gh-line", "1");
    hll.style.display = "inline";

    let node = anchor.nextSibling;
    const toWrap = [];
    while (node) {
      if (node.nodeType === Node.ELEMENT_NODE && node.tagName === "A" && /^L\d+$/.test(node.id)) {
        break;
      }
      toWrap.push(node);
      if (node.textContent && node.textContent.includes("\n")) {
        break;
      }
      node = node.nextSibling;
    }
    if (toWrap.length > 0) {
      anchor.parentNode.insertBefore(hll, toWrap[0]);
      for (const n of toWrap) {
        if (n.nodeType === Node.TEXT_NODE && n.textContent.includes("\n")) {
          const idx = n.textContent.indexOf("\n");
          const before = n.textContent.slice(0, idx);
          const after = n.textContent.slice(idx);
          if (before) hll.appendChild(document.createTextNode(before));
          n.textContent = after;
          break;
        }
        hll.appendChild(n);
      }
    }
  }

  const firstAnchor =
    document.getElementById(`L${start}`) ||
    document.getElementById(`__codelineno-0-${start}`);
  if (firstAnchor) {
    firstAnchor.scrollIntoView({ block: "center" });
  }
}

window.addEventListener("hashchange", highlightCodeLinesFromHash);

document$.subscribe(() => {
  if (typeof MathJax !== "undefined" && MathJax.typesetPromise) {
    MathJax.startup.output.clearCache();
    MathJax.typesetClear();
    MathJax.texReset();
    MathJax.typesetPromise();
  }
  highlightCodeLinesFromHash();
});
