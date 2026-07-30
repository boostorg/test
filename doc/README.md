This folder contains the documentation for the Boost.Test library.
Any contribution or submission to the library should be accompanied by the corresponding documentation.

The narrative chapters are written in [AsciiDoc](https://docs.asciidoctor.org/asciidoc/latest/)
and assembled by [Antora](https://antora.org). The API reference is generated from the
headers by [MrDocs](https://www.mrdocs.com).

Layout
======

```
doc/
├── antora.yml                     component descriptor; also defines the
│                                  attributes the pages use for reference links
├── local-playbook.yml             site playbook, used for local builds and CI
├── build_antora.sh                the entry point: npm ci, then Antora
├── mrdocs.yml                     what MrDocs extracts, and how
├── mrdocs.cpp                     umbrella translation unit: the public headers
├── CMakeLists.txt                 gives MrDocs a compilation database for it
├── mrdocs-addons/                 template overrides, each explaining itself
└── modules/ROOT/
    ├── nav.adoc                   the navigation tree
    ├── pages/                     the chapters
    ├── partials/bt_example.adoc   renders an example's code beside its output
    ├── images/, attachments/
    └── examples/                  example programs, compiled and run by the
        └── snippets/              test suite; included into the pages by tag
```

How to build the documentation
==============================

You need [Node.js](https://nodejs.org) 18 or later, CMake and a C++ compiler.
MrDocs itself is downloaded automatically.

```
> cd $boost_root/libs/test/doc
> npm ci
> ./build_antora.sh
```

The site lands in `doc/html`; open `doc/html/test/index.html`. To serve it
rather than opening the files directly:

```
> python3 -m http.server -d html 8080
```

The build needs the rest of Boost, because MrDocs compiles `mrdocs.cpp` against
it. `build_antora.sh` finds a surrounding superproject checkout by itself;
otherwise set `BOOST_SRC_DIR`, or let the playbook clone Boost for you.

Antora exits successfully even when a cross-reference or an include fails to
resolve. `build_antora.sh` passes `--log-failure-level=warn` when `CI` is set,
which makes those fatal. To get the same locally:

```
> CI=1 ./build_antora.sh
```

Working on the prose alone
--------------------------

Regenerating the reference dominates the build time. Comment out the
`cpp-reference` stanza in `antora.yml` and run Antora directly:

```
> npx antora --clean local-playbook.yml
```

Nothing else changes; only the reference module disappears.

If the cached MrDocs nightly goes stale, `./refresh_mrdocs_cache.sh` clears it
so that the next build downloads a current one.

Writing documentation
=====================

- Cross-references into the reference chapter go through the attributes defined
  in `antora.yml`, for example ``xref:{boost_test}[`BOOST_TEST`]``. The
  attribute holds only the target, so a reference page can be renamed in one
  place; the label stays at the call site, because Asciidoctor does not apply
  formatting to text that arrives from an attribute.
- References to a C++ entity use the `cpp:` macro, which resolves against what
  MrDocs generated: `cpp:boost::test_tools::assertion_result[assertion_result]`.
- Examples live in `modules/ROOT/examples` as complete programs named
  `<name>.run.cpp` or `<name>.run-fail.cpp`, with their expected output in
  `<name>.output`. The test suite compiles and runs every one of them, so an
  example that stops working breaks the build rather than just the docs. Show
  one with:

  ```
  :bt-name: example22
  :bt-rule: run-fail
  :bt-descr: BOOST_TEST_CHECKPOINT usage
  include::partial$bt_example.adoc[]
  ```

Recommendations
===============

- Documentation is part of the "definition of done". A feature does not exist until it is implemented, tested, documented and reviewed.
- It is highly recommended that each of your pull request comes with an updated documentation. Not doing so put this work on the shoulders
  of the maintainers and as a result, it would be likely that the pull request is not addressed in a timely manner.
- Please also update the changelog in [`modules/ROOT/pages/change_log.adoc`](modules/ROOT/pages/change_log.adoc)
  indicating your contribution
- Every file should come with a copyright notice at their very beginning
