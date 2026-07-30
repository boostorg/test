#!/usr/bin/env python3
"""Convert the Boost.Test QuickBook documentation to Antora AsciiDoc.

One-shot migration tool: it is run once to produce doc/modules/ROOT/pages and
the [def]-derived attribute block for doc/antora.yml, after which the .qbk
sources and this script are deleted. It is kept in branch history rather than
maintained.

Usage:
    python3 tools/qbk2adoc.py [--out DIR] [--attributes FILE] [--list-pages]

Two passes:

1. `Index` walks test.qbk following [include], recording the section tree
   (QuickBook id path -> page + anchor), every [#anchor] and [hN:id] target,
   and the [def] macros.
2. `Renderer` walks the same tree and emits one .adoc per page, resolving
   [link]s against the pass-1 map.

Anything the converter cannot resolve or does not understand is emitted as a
`// FIXME:` comment in the output and counted on stderr, so nothing is dropped
silently.
"""

import argparse
import os
import re
import sys
from collections import OrderedDict, Counter

DOC = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIBRARY_ID = "boost_test"

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

# Sections whose QuickBook id is absent or unsuitable as a page/anchor slug.
ID_OVERRIDES = {
    "Practical usage recommendations": "usage_recommendations",
    "Change log": "change_log",
    "Acknowledgements": "acknowledgments",
    "Report formats": "report_formats",
    "Summary of the API for declaring and organizing tests": "summary",
    "Custom command line arguments": "runtime_custom",
}

# Chapter (depth-1 section) id -> directory under pages/. `None` means the
# chapter has no sub-pages and renders as a single top-level page.
CHAPTER_DIRS = {
    "intro": "intro",
    "usage_variants": None,
    "tests_organization": "tests_organization",
    "testing_tools": "testing_tools",
    "test_output": "test_output",
    "runtime_config": "runtime_config",
    "adv_scenarios": "adv_scenarios",
    "pem": "prod_use",
    "usage_recommendations": "usage_recommendations",
    "section_faq": None,
    "section_glossary": None,
    "change_log": None,
    "acknowledgments": None,
    "utf_reference": "utf_reference",
}

# The introduction is the component's landing page.
PAGE_OVERRIDES = {"intro": "index.adoc"}

# QuickBook tags naming a C++ entity documented by MrDocs; they all become the
# `cpp:` macro contributed by antora-cpp-tagfiles-extension.
CPP_REF_TAGS = ("classref", "funcref", "memberref", "enumref")

ADMONITIONS = {
    "note": "NOTE",
    "tip": "TIP",
    "warning": "WARNING",
    "caution": "CAUTION",
    "important": "IMPORTANT",
}

# Macros documented only in the hand-written reference pages: MrDocs does not
# emit pages for them, so [macroref] resolves against these anchors instead.
MACROREF_TARGETS = {
    "BOOST_TEST_DISABLE_ALT_STACK": "config_disable_alt_stack",
    "BOOST_TEST_NO_GRID_COMPOSITION_AVAILABLE": "config_no_grid_composition",
    "BOOST_TEST_NO_ZIP_COMPOSITION_AVAILABLE": "config_no_zip_composition",
    "BOOST_TEST_NO_RANDOM_DATASET_AVAILABLE": "config_no_random_dataset",
}
MACROREF_PAGE = "utf_reference/link_references.adoc"


# ---------------------------------------------------------------------------
# Lexing helpers
# ---------------------------------------------------------------------------

def scan_code(text, i):
    """Scan the backtick-delimited span starting at text[i].

    QuickBook delimits inline code with a single backtick and code blocks with
    a run of two or more; Boost.Test's sources use runs of two *and* three, and
    a closing run must match the opening run exactly. Getting this wrong makes
    a stray backtick swallow the rest of the file, so it is centralised here.

    Returns (run_length, content_start, content_end, end_of_span); a run_length
    of 0 means the backtick is unpaired and should be treated as literal text.
    The sources contain two such strays (`parameter`s description` in
    runtime_config_reference.qbk and a misplaced backtick in nullary_tests.qbk);
    without this guard one of them swallows the rest of the file.
    """
    n = len(text)
    k = 0
    while i + k < n and text[i + k] == "`":
        k += 1
    start = i + k
    if k == 1:
        # Inline code never crosses a blank line.
        limit = text.find("\n\n", start)
        limit = n if limit < 0 else limit
        j = text.find("`", start, limit)
        return (1, start, j, j + 1) if j >= 0 else (0, i, i, i + 1)
    j = start
    while j < n:
        if text[j] != "`":
            j += 1
            continue
        m = 0
        while j + m < n and text[j + m] == "`":
            m += 1
        if m == k:
            return (k, start, j, j + m)
        j += m
    return (k, start, n, n)


def match_bracket(text, i):
    """Return the index just past the ']' matching the '[' at text[i].

    Skips code spans/blocks and '''passthrough''', which may legitimately
    contain unbalanced brackets.
    """
    depth = 0
    n = len(text)
    while i < n:
        c = text[i]
        if c == "[":
            depth += 1
            i += 1
        elif c == "]":
            depth -= 1
            i += 1
            if depth == 0:
                return i
        elif text.startswith("'''", i):
            j = text.find("'''", i + 3)
            i = n if j < 0 else j + 3
        elif c == "`":
            i = scan_code(text, i)[3]
        else:
            i += 1
    return n


def strip_comments(text):
    """Remove [/ ... ] comments while preserving line numbering."""
    out = []
    i = 0
    n = len(text)
    while i < n:
        if text.startswith("[/", i):
            j = match_bracket(text, i)
            out.append("\n" * text.count("\n", i, j))
            i = j
        elif text.startswith("'''", i):
            j = text.find("'''", i + 3)
            j = n if j < 0 else j + 3
            out.append(text[i:j])
            i = j
        elif text[i] == "`":
            j = scan_code(text, i)[3]
            out.append(text[i:j])
            i = j
        else:
            out.append(text[i])
            i += 1
    return "".join(out)


def split_cells(body):
    """Split a table/variablelist row body into its bracketed cells."""
    cells = []
    i = 0
    n = len(body)
    while i < n:
        if body[i] == "[":
            j = match_bracket(body, i)
            cells.append(body[i + 1:j - 1])
            i = j
        else:
            i += 1
    return cells


def split1(text):
    """Split off the first whitespace-delimited token; QuickBook arguments are
    routinely separated by a newline rather than a space."""
    parts = re.split(r"\s+", text.strip(), 1)
    return parts[0], (parts[1] if len(parts) > 1 else "")


def tag_of(inner):
    """Return the tag of a bracket body, e.g. 'section' for 'section:id T'."""
    m = re.match(r"([A-Za-z_][\w]*)", inner)
    return m.group(1) if m else ""


def make_id(title):
    """Approximate QuickBook's auto-generated section id."""
    title = re.sub(r"\[[@#$*'_^]?[^\]\s]*\s+([^\]]*)\]", r"\1", title)
    title = title.replace("`", "").replace("'", "")
    slug = re.sub(r"[^a-z0-9]+", "_", title.lower()).strip("_")
    return slug or "section"


def attr_name(def_name):
    """Map a QuickBook [def] name such as __BOOST_TEST__ to an attribute name."""
    return def_name.strip("_").lower() or "def"


# ---------------------------------------------------------------------------
# Pass 1 - document model
# ---------------------------------------------------------------------------

class Section(object):
    def __init__(self, sid, title, depth, src, line, parent):
        self.id = sid
        self.title = title
        self.depth = depth
        self.src = src
        self.line = line
        self.parent = parent
        self.children = []
        self.chunks = []      # interleaved ('text', str) | ('section', Section)
        self.page = None
        self.qid = None


class Index(object):
    def __init__(self, root_file):
        self.defs = OrderedDict()
        self.root = Section(LIBRARY_ID, "Boost.Test", 0, root_file, 0, None)
        self.by_qid = {}
        self.anchors = {}
        self.pending_anchor = None
        self._load(root_file, self.root)
        self._assign_qids(self.root, [LIBRARY_ID])
        self._assign_pages()
        self._collect_anchors()

    # -- loading ------------------------------------------------------------

    def _load(self, path, current):
        text = strip_comments(open(path, encoding="utf-8", errors="replace").read())
        rel = os.path.relpath(path, DOC)
        i = 0
        n = len(text)
        buf = []
        line = 1

        def flush():
            if buf:
                current.chunks.append(("text", "".join(buf)))
                del buf[:]

        while i < n:
            c = text[i]
            if c == "\n":
                line += 1
                buf.append(c)
                i += 1
                continue
            if text.startswith("'''", i) or c == "`":
                if c == "`":
                    j = scan_code(text, i)[3]
                else:
                    j = text.find("'''", i + 3)
                    j = n if j < 0 else j + 3
                buf.append(text[i:j])
                line += text.count("\n", i, j)
                i = j
            else:
                if c != "[":
                    buf.append(c)
                    i += 1
                    continue
                j = match_bracket(text, i)
                inner = text[i + 1:j - 1]
                tag = tag_of(inner)
                if tag == "include":
                    flush()
                    target = inner.split(None, 1)[1].strip()
                    self._load(os.path.normpath(
                        os.path.join(os.path.dirname(path), target)), current)
                elif tag == "def":
                    name, value = inner.split(None, 2)[1:]
                    self.defs[name] = value.strip()
                elif tag == "template":
                    pass          # bt_example et al are handled structurally
                elif tag == "section":
                    flush()
                    # `[#anchor][section ...]` puts the anchor just before the
                    # section; it names the section, so move it inside.
                    if current.chunks and current.chunks[-1][0] == "text":
                        m = re.search(r"(\[#[^\]\s]+\])\s*$", current.chunks[-1][1])
                        if m:
                            self.pending_anchor = m.group(1)
                            current.chunks[-1] = (
                                "text", current.chunks[-1][1][:m.start()])
                    sid, title = self._parse_section_head(inner)
                    child = Section(sid, title, current.depth + 1, rel, line, current)
                    current.chunks.append(("section", child))
                    current.children.append(child)
                    current = child
                    if self.pending_anchor:
                        current.chunks.append(("text", self.pending_anchor + "\n"))
                        self.pending_anchor = None
                elif tag == "endsect":
                    flush()
                    if current.parent is not None:
                        current = current.parent
                elif tag in ("library", "quickbook"):
                    pass
                else:
                    buf.append(text[i:j])
                line += text.count("\n", i, j)
                i = j
        flush()

    def _parse_section_head(self, inner):
        rest = inner[len("section"):]
        sid = None
        if rest.startswith(":"):
            m = re.match(r":(\S+)\s*(.*)", rest, re.S)
            sid, rest = m.group(1), m.group(2)
        title = rest.strip()
        # `[section Report formats][#ref_report_formats]` leaves the anchor in
        # the title; peel it off and re-emit it inside the section body.
        while True:
            m = re.search(r"(\[#[^\]\s]+\])\s*$", title)
            if not m:
                break
            self.pending_anchor = m.group(1)
            title = title[:m.start()].rstrip()
        if title in ID_OVERRIDES:
            sid = ID_OVERRIDES[title]
        elif sid is None:
            sid = make_id(title)
        return sid, title

    # -- ids and pages ------------------------------------------------------

    def _assign_qids(self, sec, path):
        sec.qid = ".".join(path)
        self.by_qid[sec.qid] = sec
        for child in sec.children:
            self._assign_qids(child, path + [child.id])

    def _assign_pages(self):
        for chapter in self.root.children:
            directory = CHAPTER_DIRS.get(chapter.id, chapter.id)
            override = PAGE_OVERRIDES.get(chapter.id)
            if override:
                chapter.page = override
            elif directory is None:
                chapter.page = chapter.id + ".adoc"
            else:
                chapter.page = directory + "/index.adoc"
            self._assign_pages_below(chapter, chapter.src, directory)

    def _assign_pages_below(self, sec, chapter_src, directory):
        """A section starts a new page when it opens a .qbk file of its own."""
        for child in sec.children:
            if directory is not None and child.src != chapter_src:
                child.page = "%s/%s.adoc" % (directory, child.id)
                self._assign_pages_below(child, child.src, directory)
            else:
                self._assign_pages_below(child, chapter_src, directory)

    def owning_page(self, sec):
        while sec is not None and sec.page is None:
            sec = sec.parent
        return sec

    def _collect_anchors(self):
        for qid, sec in self.by_qid.items():
            if sec is self.root:
                continue
            owner = self.owning_page(sec)
            self.anchors[qid] = (owner, None if owner is sec else sec.id)
        # A section renamed by ID_OVERRIDES is still linked to by the id
        # QuickBook derived from its title; keep both resolvable.
        for sec in list(self.by_qid.values()):
            if sec.parent is None or sec.title not in ID_OVERRIDES:
                continue
            auto = make_id(sec.title)
            if auto != sec.id:
                parent_qid = sec.parent.qid
                self.anchors["%s.%s" % (parent_qid, auto)] = self.anchors[sec.qid]
        for sec in self.by_qid.values():
            owner = self.owning_page(sec)
            for kind, chunk in sec.chunks:
                if kind != "text":
                    continue
                found = [m.group(1) for m in re.finditer(r"\[#([^\]\s]+)\]", chunk)]
                found += [m.group(1)
                          for m in re.finditer(r"\[h[1-6]:([^\]\s]+)[\s\]]", chunk)]
                for anchor in found:
                    self.anchors[anchor] = (owner, anchor)
                    # links also address these as <section-qid>.<anchor>
                    self.anchors.setdefault("%s.%s" % (sec.qid, anchor),
                                            (owner, anchor))

    def resolve(self, target):
        """Map a QuickBook link target to (page-section, anchor) or None."""
        if target in self.anchors:
            return self.anchors[target]
        sec = self.by_qid.get(target)
        if sec is not None:
            owner = self.owning_page(sec)
            return (owner, None if owner is sec else sec.id)
        return None


# ---------------------------------------------------------------------------
# Pass 2 - rendering
# ---------------------------------------------------------------------------

LIST_RE = re.compile(r"^(\s*)([*#]+)\s+(.*)$")
FENCE_RE = re.compile(r"^\s*``\s*$")


class Renderer(object):
    def __init__(self, index, outdir):
        self.index = index
        self.outdir = outdir
        self.fixmes = Counter()
        self.unresolved = Counter()
        self.examples = set()
        self.imports = set()
        self.snippets = {}    # callout id -> example$ resource path
        self.plain = {}       # def name -> plain text, for use inside code
        for name, value in index.defs.items():
            self.plain[name] = self._plain_text(value)

    # -- helpers ------------------------------------------------------------

    @staticmethod
    def _plain_text(value):
        """Reduce a [def] body to the text a reader sees, for code blocks."""
        m = re.match(r"\[link\s+\S+\s+(.*)\]$", value.strip(), re.S)
        if m:
            value = m.group(1)
        value = re.sub(r"\[[*'_]\s*([^\]]*)\]", r"\1", value)
        return value.replace("`", "").replace("/", "").strip()

    # QuickBook's other italic form, /like this/. Excludes C comments (/* ... */)
    # and anything spanning a paragraph break; `and/or` is ruled out by the
    # word-character check at the call site.
    SLASH_ITALIC = re.compile(r"/(?!\*)([^/\n]{1,60}(?:\n[^/\n]{1,60})?)/(?![\w/])")

    # Characters that still take on meaning inside a monospace span.
    UNSAFE_IN_CODE = re.compile(r"[{}*#^~`]|__|^_|_$")

    def code_span(self, body):
        """Render `code` so AsciiDoc leaves its contents alone."""
        body = self.subst_defs(body.strip(), code=True)
        if not self.UNSAFE_IN_CODE.search(body):
            return "`%s`" % body
        if "+" not in body and "`" not in body:
            return "`+%s+`" % body                     # no substitutions at all
        return "`%s`" % body.replace("{", "\\{").replace("`", "'")

    def fixme(self, kind, detail=""):
        self.fixmes[kind] += 1
        return "// FIXME(qbk2adoc): %s %s\n" % (kind, detail)

    # `{` is escaped in prose so stray braces in the sources cannot look like
    # attribute references. Substituted [def]s therefore go in as sentinels and
    # become real braces on the way out of inline().
    ATTR_OPEN, ATTR_CLOSE = "\x01", "\x02"

    def subst_defs(self, text, code=False):
        def repl(m):
            name = m.group(0)
            if name not in self.index.defs:
                return name
            if code:
                return self.plain[name]
            return "%s%s%s" % (self.ATTR_OPEN, attr_name(name), self.ATTR_CLOSE)
        return re.sub(r"__[A-Za-z0-9_]+?__", repl, text)

    # -- inline -------------------------------------------------------------

    def inline(self, text, page):
        text = self.subst_defs(text)
        out = []
        i = 0
        n = len(text)
        while i < n:
            c = text[i]
            if text.startswith("'''", i):
                j = text.find("'''", i + 3)
                j = n if j < 0 else j + 3
                out.append(self.fixme("raw-docbook", text[i:j].replace("\n", " ")))
                i = j
            elif c == "`":
                k, cs, ce, end = scan_code(text, i)
                if k == 0:
                    self.fixmes["stray-backtick"] += 1
                    out.append("\\`")
                else:
                    out.append(self.code_span(text[cs:ce]))
                i = end
            elif c == "[":
                j = match_bracket(text, i)
                out.append(self.inline_tag(text[i + 1:j - 1], page))
                i = j
            elif c == "{":
                out.append("\\{")
                i += 1
            elif c == "/" and (not out or not re.match(r"[\w/]", out[-1][-1:])):
                m = self.SLASH_ITALIC.match(text, i)
                if m:
                    out.append("_%s_" % self.inline(m.group(1), page))
                    i = m.end(1) + 1
                else:
                    out.append(c)
                    i += 1
            else:
                out.append(c)
                i += 1
        return ("".join(out)
                .replace(self.ATTR_OPEN, "{").replace(self.ATTR_CLOSE, "}"))

    def link_macro(self, target, label, page):
        # `boost.debug.under_debugger` and friends address the Doxygen-generated
        # reference, which MrDocs now owns; route them through the cpp: macro.
        if target.startswith("boost.") and not target.startswith(LIBRARY_ID + "."):
            return "cpp:%s[%s]" % (target.replace(".", "::"),
                                   label.replace("]", "\\]"))
        hit = self.index.resolve(target)
        if hit is None or hit[0] is None or hit[0].page is None:
            self.unresolved[target] += 1
            return "%s // FIXME(qbk2adoc): unresolved link %s" % (label, target)
        owner, anchor = hit
        ref = owner.page
        if anchor:
            ref += "#" + anchor
        return "xref:%s[%s]" % (ref, label.replace("]", "\\]"))

    def inline_tag(self, inner, page):
        tag = tag_of(inner)
        rest = inner[len(tag):].strip() if tag else inner
        if inner.startswith("#"):
            return "[[%s]]" % inner[1:].strip()
        if inner.startswith("*"):
            return "*%s*" % self.inline(inner[1:].strip(), page)
        if inner.startswith("'"):
            return "_%s_" % self.inline(inner[1:].strip(), page)
        if inner.startswith("_"):
            return "[.underline]#%s#" % self.inline(inner[1:].strip(), page)
        if inner.startswith("^"):
            return "`%s`" % inner[1:].strip()
        if inner.startswith("@"):
            url, label = split1(inner[1:])
            label = self.inline(label, page) if label else ""
            return "%s[%s]" % (url, label.replace("]", "\\]"))
        if inner.startswith("$"):
            return self.image_macro(inner[1:].strip(), block=False)
        if tag in ("link", "links"):
            target, label = split1(rest)
            label = self.inline(label, page) if label else target
            return self.link_macro(target, label, page)
        if tag in CPP_REF_TAGS:
            symbol, label = split1(rest)
            label = label or symbol
            return "cpp:%s[%s]" % (symbol, self.inline(label, page).replace("]", "\\]"))
        if tag == "headerref":
            header, label = split1(rest)
            label = label or header
            return "link:{base-url}/%s[%s]" % (header, self.inline(label, page))
        if tag == "macroref":
            macro, label = split1(rest)
            label = self.inline(label or macro, page)
            anchor = MACROREF_TARGETS.get(macro)
            if anchor:
                return "xref:%s#%s[%s]" % (MACROREF_PAGE, anchor, label)
            self.unresolved["macroref:" + macro] += 1
            return "%s // FIXME(qbk2adoc): unresolved macroref %s" % (label, macro)
        if inner.strip() in self.snippets:
            return "\n" + self.snippet_include(inner.strip())
        if tag == "footnote":
            return "footnote:[%s]" % self.inline(rest, page).replace("]", "\\]").strip()
        if tag == "ticket":
            return "{trac-url}/%s[#%s]" % (rest, rest)
        if tag == "pull_request":
            return "{pr-url}/%s[#PR-%s]" % (rest, rest)
        if tag == "github_issue":
            return "{issue-url}/%s[#GH-%s]" % (rest, rest)
        if tag == "br":
            return " +\n"
        if tag in ADMONITIONS:
            return "%s: %s" % (ADMONITIONS[tag], self.inline(rest, page).strip())
        if tag == "":
            return self.inline(inner, page)
        self.fixmes["inline-tag:" + (tag or "?")] += 1
        return "\\[%s]" % inner

    def image_macro(self, spec, block):
        m = re.match(r"(\S+)\s*(?:\[width\s+(\S+?)\s*\])?", spec)
        path = os.path.basename(m.group(1))
        attrs = "width=%s" % m.group(2) if m.group(2) else ""
        return "image%s:%s[%s]" % (":" if block else "", path, attrs)

    # -- blocks -------------------------------------------------------------

    def render_body(self, text, page, level, headings):
        """Render a section body (no nested [section]s) at AsciiDoc `level`."""
        out = []
        pending = []          # anchors waiting to attach to the next block
        pos = 0
        n = len(text)
        while pos < n:
            if text[pos].isspace():
                pos += 1
                continue
            line_end = text.find("\n", pos)
            line_end = n if line_end < 0 else line_end
            line = text[pos:line_end]

            if text[pos] == "[":
                end = match_bracket(text, pos)
                inner = text[pos + 1:end - 1]
                tag = tag_of(inner)
                if inner.startswith("#"):
                    pending.append("[#%s]" % inner[1:].strip())
                    pos = end
                    continue
                block = self.render_block_tag(tag, inner, page, level, headings)
                if block is not None:
                    out.append(self.flush_anchors(pending) + block)
                    pos = end
                    continue

            if text[pos] == "`" and text.startswith("``", pos):
                _, cs, ce, end = scan_code(text, pos)
                out.append(self.flush_anchors(pending)
                           + self.code_block(text[cs:ce], page))
                pos = end
                continue

            if LIST_RE.match(line):
                block, pos = self.list_block(text, pos, page)
                out.append(self.flush_anchors(pending) + block)
                continue

            block, pos = self.paragraph(text, pos, page)
            if block:
                out.append(self.flush_anchors(pending) + block)
        out.append(self.flush_anchors(pending))
        return "".join(out)

    @staticmethod
    def flush_anchors(pending):
        if not pending:
            return ""
        text = "".join(a + "\n" for a in pending)
        del pending[:]
        return text

    def render_block_tag(self, tag, inner, page, level, headings):
        rest = inner[len(tag):].strip() if tag else inner
        if re.match(r"^h[1-6]([\s:])", inner) or tag == "heading":
            return self.heading(inner, page, level, headings)
        if tag in ADMONITIONS:
            return self.admonition(ADMONITIONS[tag], rest, page, level)
        if tag == "table":
            return self.table(inner, page)
        if tag == "variablelist":
            return self.variablelist(rest, page, level)
        if tag == "pre":
            return self.pre_block(rest, page)
        if tag == "bt_example":
            return self.bt_example(rest, page)
        if tag == "import":
            self.register_import(rest.strip())
            return ""
        if inner.strip() in self.snippets:
            return self.snippet_include(inner.strip())
        if tag == "role":
            role, body = split1(rest)
            if role == "epigraph":
                return "[quote]\n____\n%s\n____\n\n" % self.inline(body, page).strip()
            return self.fixme("role", role)
        if tag == "xinclude":
            return ("// The Doxygen-generated reference is replaced by the "
                    "MrDocs `reference` module.\n\n")
        if inner.startswith("$"):
            return self.image_macro(inner[1:].strip(), block=True) + "\n\n"
        return None

    def heading(self, inner, page, level, headings):
        m = re.match(r"h([1-6])(?::(\S+))?\s*(.*)", inner, re.S)
        if m:
            depth, anchor, title = int(m.group(1)), m.group(2), m.group(3)
        else:                                   # [heading Title]
            depth, anchor = 3, None
            title = inner[len("heading"):].strip()
        rank = headings.index(depth) if depth in headings else 0
        out = "[#%s]\n" % anchor if anchor else ""
        out += "%s %s\n\n" % ("=" * min(level + 1 + rank, 6),
                              self.inline(title.strip(), page).strip())
        return out

    def admonition(self, kind, body, page, level):
        rendered = self.render_body(body, page, level, []).strip()
        if "\n" in rendered:
            return "[%s]\n====\n%s\n====\n\n" % (kind, rendered)
        return "%s: %s\n\n" % (kind, rendered)

    def code_block(self, body, page):
        body = self.subst_defs(body, code=True)
        # QuickBook annotates code with /*< ... >*/; AsciiDoc uses <n> markers
        # in the listing plus a colist after it.
        callouts = []

        def callout(m):
            callouts.append(m.group(1).strip())
            return " <%d>" % len(callouts)

        body = re.sub(r"[ \t]*/\*<(.*?)>\*/", callout, body, flags=re.S)
        lines = [ln.rstrip() for ln in body.split("\n")]
        while lines and not lines[0].strip():
            lines.pop(0)
        while lines and not lines[-1].strip():
            lines.pop()
        indent = min((len(ln) - len(ln.lstrip()) for ln in lines if ln.strip()),
                     default=0)
        body = "\n".join(ln[indent:] for ln in lines)
        out = "[source,cpp]\n----\n%s\n----\n" % body
        for number, text in enumerate(callouts, 1):
            out += "<%d> %s\n" % (number, self.inline(text, page).strip())
        return out + "\n"

    def pre_block(self, body, page):
        """[pre] is a literal block, except QuickBook still expands markup in
        it. Where markup is present, re-enable substitutions on the block."""
        markup = re.search(r"\[[a-z@#$*'_]|__[A-Za-z0-9_]+__", body)
        if markup:
            body = self.inline(body, page)
        else:
            body = self.subst_defs(body, code=True)
        lines = [ln.rstrip() for ln in body.split("\n")]
        while lines and not lines[0].strip():
            lines.pop(0)
        while lines and not lines[-1].strip():
            lines.pop()
        prefix = '[subs="+macros,+attributes,+quotes"]\n' if markup else ""
        return "%s....\n%s\n....\n\n" % (prefix, "\n".join(lines))

    def bt_example(self, rest, page):
        parts = [p.strip() for p in rest.split("..")]
        if len(parts) != 3:
            return self.fixme("bt_example-arity", rest)
        name, descr, rule = parts
        self.examples.add((name, rule))
        return ("// tag reference: doc/modules/ROOT/examples/%s.%s.cpp\n"
                ":bt-name: %s\n:bt-rule: %s\n:bt-descr: %s\n"
                "include::partial$bt_example.adoc[]\n\n"
                % (name, rule, name, rule, self.inline(descr, page).strip()))

    # doc/snippet and doc/examples are now both under modules/ROOT/examples, so
    # the paths in the QuickBook [import]s have to be remapped.
    EXAMPLES_ROOT = os.path.join(DOC, "modules", "ROOT", "examples")
    IMPORT_REMAP = (("snippet/", "snippets/"), ("examples/", ""))

    def register_import(self, target):
        """Record an [import] and the tag ids the imported file defines."""
        self.imports.add(target)
        rel = target.lstrip("./")
        for old, new in self.IMPORT_REMAP:
            if rel.startswith(old):
                rel = new + rel[len(old):]
                break
        path = os.path.normpath(os.path.join(self.EXAMPLES_ROOT, rel))
        if not os.path.isfile(path):
            self.fixmes["import-missing"] += 1
            sys.stderr.write("  missing import: %s\n" % target)
            return
        for m in re.finditer(r"^\s*(?://|/\*)\s*tag::([\w-]+)\[\]",
                             open(path, encoding="utf-8", errors="replace").read(),
                             re.M):
            self.snippets[m.group(1)] = rel.replace(os.sep, "/")

    def snippet_include(self, name):
        resource = self.snippets[name]
        out = ("[source,cpp]\n----\ninclude::example$%s[tag=%s]\n----\n"
               % (resource, name))
        # Snippets carrying AsciiDoc callouts keep the colist beside the code,
        # in a `<name>-callouts` region of the same file.
        if name + "-callouts" in self.snippets:
            out += "include::example$%s[tag=%s-callouts]\n" % (resource, name)
        return out + "\n"

    def table(self, inner, page):
        m = re.match(r"table(?::(\S+))?\s*", inner)
        anchor = m.group(1)
        rest = inner[m.end():]
        first = rest.find("[")
        title = rest[:first if first >= 0 else len(rest)].strip()
        rows = [split_cells(r) for r in split_cells(rest[first:] if first >= 0 else "")]
        rows = [r for r in rows if r]
        if not rows:
            return self.fixme("empty-table", title)
        ncols = max(len(r) for r in rows)
        out = []
        if anchor:
            out.append("[#%s]\n" % anchor)
        if title:
            out.append(".%s\n" % self.inline(title, page).strip())
        out.append('[%%header%%autowidth,cols="%d*"]\n|===\n' % ncols)
        for idx, row in enumerate(rows):
            for cell in row:
                rendered = self.render_body(cell, page, 6, []).strip()
                if "\n" in rendered:
                    out.append("a|\n%s\n" % rendered)
                else:
                    out.append("|%s\n" % rendered.replace("|", "\\|"))
            out.append("\n")
            if idx == 0 and len(rows) > 1:
                pass
        out.append("|===\n\n")
        return "".join(out)

    def variablelist(self, rest, page, level):
        first = rest.find("[")
        title = rest[:first if first >= 0 else len(rest)].strip()
        rows = [split_cells(r) for r in split_cells(rest[first:] if first >= 0 else "")]
        out = []
        if title:
            out.append(".%s\n" % self.inline(title, page).strip())
        out.append("[cols=\"1,3\"]\n|===\n")
        for row in rows:
            if len(row) != 2:
                out.append(self.fixme("variablelist-row", str(row)[:60]))
                continue
            term = self.render_body(row[0], page, 6, []).strip()
            desc = self.render_body(row[1], page, 6, []).strip()
            out.append("|%s\na|\n%s\n\n" % (term.replace("|", "\\|"), desc))
        out.append("|===\n\n")
        return "".join(out)

    def list_block(self, text, pos, page):
        items = []
        n = len(text)
        while pos < n:
            line_end = text.find("\n", pos)
            line_end = n if line_end < 0 else line_end
            line = text[pos:line_end]
            m = LIST_RE.match(line)
            if m:
                indent, marker, body = m.group(1), m.group(2), m.group(3)
                items.append([len(indent) + len(marker) - 1, marker[-1], [body]])
                pos = line_end + 1
                continue
            if not line.strip():
                # a blank line ends the list unless the next line continues it
                nxt = text.find("\n", line_end + 1)
                nxt = n if nxt < 0 else nxt
                peek = text[line_end + 1:nxt]
                if items and (LIST_RE.match(peek) or peek.startswith("  ")):
                    items[-1][2].append("")
                    pos = line_end + 1
                    continue
                pos = line_end + 1
                break
            if items and line.startswith((" ", "\t")):
                items[-1][2].append(line.strip())
                pos = line_end + 1
                continue
            break
        levels = sorted({item[0] for item in items})
        out = []
        for depth, marker, body in items:
            rank = levels.index(depth) + 1
            bullet = ("*" if marker == "*" else ".") * rank
            rendered = self.inline("\n".join(body).strip(), page).strip()
            rendered = re.sub(r"\n\s*\n", "\n+\n", rendered)
            out.append("%s %s\n" % (bullet, rendered))
        out.append("\n")
        return "".join(out), pos

    def paragraph(self, text, pos, page):
        n = len(text)
        depth = 0
        i = pos
        while i < n:
            c = text[i]
            if c == "[":
                depth += 1
            elif c == "]":
                depth = max(0, depth - 1)
            elif c == "`":
                i = scan_code(text, i)[3]
                continue
            elif c == "\n" and depth == 0:
                if text.startswith("\n", i + 1):
                    break
                nxt_end = text.find("\n", i + 1)
                nxt_end = n if nxt_end < 0 else nxt_end
                nxt = text[i + 1:nxt_end]
                if LIST_RE.match(nxt) or nxt.startswith("``"):
                    break
                if nxt.strip()[1:-1] in self.snippets and nxt.strip().startswith("["):
                    break
                if nxt.lstrip().startswith("["):
                    inner_tag = tag_of(nxt.lstrip()[1:])
                    if inner_tag in ADMONITIONS or inner_tag in (
                            "table", "variablelist", "pre", "bt_example",
                            "import", "role", "xinclude") or re.match(
                            r"^h[1-6][\s:]", nxt.lstrip()[1:]):
                        break
            i += 1
        body = text[pos:i].strip()
        rendered = self.inline(body, page).strip()
        if not rendered:
            return "", i + 1
        if rendered[:1] in (".", "+", "=", "-") or rendered.startswith("//"):
            rendered = "{empty}" + rendered
        return rendered + "\n\n", i + 1

    # -- page assembly ------------------------------------------------------

    def heading_ranks(self, sec):
        """Distinct [hN] levels used directly in a section, in document order."""
        found = []
        for kind, chunk in sec.chunks:
            if kind != "text":
                continue
            for m in re.finditer(r"\[h([1-6])[\s:]", chunk):
                depth = int(m.group(1))
                if depth not in found:
                    found.append(depth)
        return sorted(found)

    def render_section(self, sec, page, level):
        out = []
        headings = self.heading_ranks(sec)
        for kind, chunk in sec.chunks:
            if kind == "text":
                out.append(self.render_body(chunk, page, level, headings))
            elif chunk.page is None:
                out.append("[#%s]\n%s %s\n\n" % (
                    chunk.id, "=" * (level + 1),
                    self.inline(chunk.title, page).strip()))
                out.append(self.render_section(chunk, page, level + 1))
        return "".join(out)

    # QuickBook spells an epigraph's attribution as a *second* epigraph whose
    # body starts with `--`; AsciiDoc puts it in the block attribute list.
    EPIGRAPH_ATTRIB = re.compile(
        r"\[quote\]\n____\n(.*?)\n____\n\n\[quote\]\n____\n--(.+?)\n____\n",
        re.S)

    def render_page(self, sec):
        body = self.render_section(sec, sec, 1)
        body = self.EPIGRAPH_ATTRIB.sub(
            lambda m: "[quote, %s]\n____\n%s\n____\n"
                      % (m.group(2).strip(), m.group(1)), body)
        header = "= %s\n" % self.inline(sec.title, sec).strip()
        alias = self.page_alias(sec)
        if alias:
            header += ":page-aliases: %s\n" % alias
        return header + "\n" + body.strip() + "\n"

    def page_alias(self, sec):
        """The path the BoostBook site used for this section."""
        parts = sec.qid.split(".")
        return "%s.adoc" % "/".join(parts)

    def run(self):
        pages = OrderedDict()
        for sec in self.index.by_qid.values():
            if sec.page:
                pages[sec.page] = sec
        for page, sec in pages.items():
            path = os.path.join(self.outdir, page)
            os.makedirs(os.path.dirname(path), exist_ok=True)
            with open(path, "w", encoding="utf-8") as fh:
                fh.write(self.render_page(sec))
        return pages


def emit_nav(index, renderer, path):
    """Draft modules/ROOT/nav.adoc from the section tree, for hand-tuning."""
    lines = []

    def walk(sec, depth):
        for child in sec.children:
            if child.page:
                lines.append("%s xref:%s[]\n" % ("*" * depth, child.page))
                walk(child, depth + 1)
            else:
                # a section that is only a heading on its parent page: keep its
                # sub-pages, but attach them at the parent's level
                walk(child, depth)

    walk(index.root, 1)
    with open(path, "w", encoding="utf-8") as fh:
        fh.writelines(lines)


def emit_attributes(index, renderer, path):
    lines = []
    for name, value in index.defs.items():
        rendered = renderer.inline(value, index.root).strip().replace("\n", " ")
        rendered = re.sub(r"\s+", " ", rendered)
        # QuickBook's /slash italics/ survive only in the [def] bodies.
        rendered = re.sub(r"(?<![\w/])/([^/]+)/(?![\w/])", r"_\1_", rendered)
        if rendered.startswith("/") and not rendered.endswith("/"):
            rendered = "_%s_" % rendered[1:]        # unterminated in the source
        lines.append("    %s: '%s'\n" % (attr_name(name), rendered.replace("'", "''")))
    with open(path, "w", encoding="utf-8") as fh:
        fh.writelines(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=os.path.join(DOC, "modules", "ROOT", "pages"))
    ap.add_argument("--attributes", default=os.path.join(DOC, "tools", "attributes.yml"))
    ap.add_argument("--nav", default=os.path.join(DOC, "tools", "nav.adoc"))
    ap.add_argument("--list-pages", action="store_true")
    args = ap.parse_args()

    index = Index(os.path.join(DOC, "test.qbk"))
    renderer = Renderer(index, args.out)
    if args.list_pages:
        for sec in index.by_qid.values():
            if sec.page:
                print("%-58s %s" % (sec.page, sec.qid))
        return
    pages = renderer.run()
    emit_attributes(index, renderer, args.attributes)
    emit_nav(index, renderer, args.nav)

    sys.stderr.write("%d sections -> %d pages\n" % (len(index.by_qid), len(pages)))
    sys.stderr.write("%d defs -> %s\n" % (len(index.defs), args.attributes))
    if renderer.unresolved:
        sys.stderr.write("\nunresolved link targets (%d distinct, %d uses):\n"
                         % (len(renderer.unresolved), sum(renderer.unresolved.values())))
        for target, count in renderer.unresolved.most_common():
            sys.stderr.write("  %4d  %s\n" % (count, target))
    if renderer.fixmes:
        sys.stderr.write("\nFIXMEs emitted:\n")
        for kind, count in renderer.fixmes.most_common():
            sys.stderr.write("  %4d  %s\n" % (count, kind))


if __name__ == "__main__":
    main()
