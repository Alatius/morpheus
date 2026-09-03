morpheus XML output
====================

This branch adds `morpheus`, a second binary alongside `cruncher` that shares
the same analysis pipeline (`AnalyzeString`, `SortAnals`, `GoodAnals`) but
emits Perseus-style XML instead of the `<NL>`-line format. It is unconditional: 
morpheus always emits XML, there is no flag to switch it off.

Why XML
-------

`cruncher`'s Perseus format represents each analysis as a single positional
text line: abbreviated vocabulary (`nom`, `masc`, `pres`), a single
part-of-speech prefix (`P/N/V/E/I`), and multi-value fields on one line
(e.g. `nom/voc/acc masc/fem pl`). morpheus renders the same analyses as a
structured document instead: full-word vocabulary, a refined part of speech
per analysis, and multi-value fields expanded into a cartesian product of
`<infl>` elements (one case/gender/etc. combination per element), so
downstream code doesn't have to re-parse a slash-separated field.

Because both binaries run through the same `SortAnals`/`GoodAnals` filtering,
the *set* of analyses is guaranteed identical between the two. XML is a
different serialization of the same data, not a different analysis.

Building
--------

`morpheus` is built as part of the normal top-level build:
```bash
make
```

It installs to `bin/morpheus` alongside `cruncher`.

Architecture / rebase isolation
--------------------------------

This is meant to be a long-lived branch that stays rebasable against
upstream. The XML support is kept isolated to two files:

- `src/anal/morpheus.c` — dedicated main loop (arg parsing, word intake).
- `src/anal/prntalph.c` — XML formatting and vocabulary-mapping tables.

The core analysis pipeline and dictionary compilation (`checkstring`,
`AnalyzeString`, `gener/`, `gkdict/`) are untouched. If you rebase and the
dictionary changes, `morpheus`'s output changes exactly as much as
`cruncher`'s does. No separate mapping logic to keep in sync.

Running morpheus
-----------------
```bash
MORPHLIB=stemlib bin/morpheus < wordlist
```

Words can also be given directly as arguments instead of on stdin:

```bash
MORPHLIB=stemlib bin/morpheus lo/gos timh/
```

For Latin, add -L:

```bash
echo "rosa" | MORPHLIB=stemlib bin/morpheus -L
```

```bash
MORPHLIB=stemlib bin/morpheus -L rosa dominus
```

The `echo | ...` form pipes the word in on stdin; the 
`bin/morpheus -L rosa dominus` form passes words as argv arguments instead.
Both produce identical output for a single word — see above for how the two 
input paths differ.

Each argv word is analyzed as its own word (not split further); after argv words 
are consumed, morpheus exits rather than falling through to read stdin.

### Flags

Shares most analysis flags with `cruncher`:

`-L` Latin instead of Greek. `-I` Italian. `-S` disables strict case matching
(allows capitalized/sentence-initial words). `-n` accent-insensitive retry.
`-V` verb forms only. `-i` adds a `<dump_analysis>` block per analysis with
low-level internal fields (raw stem/suffix/preverb pieces, morphflag bytes).

`-c`, `-p`, `-x` are accepted (for `getopt` parity with `cruncher`'s flag set)
but currently have no effect on morpheus's output: it always produces XML
regardless.

### Output format

Root element is `<words>...</words>`. Comment-only lines in the input
(starting with `#`) pass through as XML comments; literal `--` inside a
comment is neutralized to `- ` so it can't collide with `<!--`/`-->`.

Each analyzed word is a `<word>` element containing a `<form>` (the input
form) and one `<entry>` per distinct lemma. Each `<entry>` has a `<dict>`
block (`<hdwd>` = lemma, `<pofs>` = part of speech with an `order` attribute,
`<gend>` for nouns) followed by one `<infl>` per case/gender/mood/tense/etc.
combination that analysis covers.

Unrecognized words are emitted inline as `<unknown xml:lang="...">word</unknown>`
rather than being dropped to stderr the way `cruncher` handles them.

If there are no words at all (empty stdin, no argv words, no comments),
morpheus emits `<error>No words provided</error>` with no `<words>` wrapper
and exits `2`.

### Intentional divergences from cruncher

Deliberate, not bugs. See `tests/run_xml_tests.sh` (tests N1–N8) for the
checks that pin them down:

- Long-form vocabulary (`nominative`, `singular`) instead of cruncher's
  abbreviations (`nom`, `sg`).
- `<pofs>` is a refined category (pronoun/article/preposition/etc., not just
  cruncher's coarse `P/N/V/E/I` prefix), with a sentinel `order="0"` for
  categories not in the ordering table.
- Case/gender combinations expand into a cartesian product of `<infl>`
  elements (one per combination) rather than a single combined cruncher line.
- A line with multiple whitespace-separated words only analyzes the first
  token (matches cruncher's batch behavior; diverges from the original
  upstream alpheios code, which analyzed every token on the line).

Testing
-------

```bash
bash tests/run_xml_tests.sh
```

This is intentionally independent of `make test` / `tests/run_tests.sh` and
is not wired into the Makefile. Run it by hand. To (re)generate golden
baselines:

```bash
bash tests/run_xml_tests.sh --update
```

Only regenerate a baseline after you've deliberately changed something that
affects that fixture's output *and* manually confirmed the new output is
correct. A baseline diff on its own is not a reason to `--update`; it's the
suite's signal to go look at what changed, not to silently accept it.

### Test ID legend

IDs are grouped by what they assert:

- **P*n*** — Positive: morpheus **should** agree with cruncher on the same
  word (same known/unknown status, same lemmas, same stemtypes, etc.), or
  otherwise behave as documented (flag parsing, argv handling). A failure
  here means morpheus disagrees with cruncher, or with its own documented
  behavior, where it shouldn't.
- **N*n*** — Negative / divergence: morpheus is **deliberately** different
  from cruncher here (richer vocabulary, expanded fields, inline `<unknown>`,
  etc.). A failure here means morpheus accidentally matches cruncher's old
  behavior where it was supposed to have changed it, or produced something
  outside the intended divergence.

Plain-English meaning of each:

| ID             | What it checks                                                          |
|----------------|-------------------------------------------------------------------------|
| flag-n-arg     | `-n` doesn't swallow the next argv word as an option-argument           |
| argv-cleanup   | argv words get the same digit-trimming cleanup as stdin words           |
| P1             | known/unknown word partition matches cruncher                           |
| P2             | lemma set and first-occurrence order match cruncher                     |
| P3             | stemtype set matches cruncher                                           |
| P4             | (soft) feature-token coverage (approximate, see comparator notes)       |
| P5             | (soft) case/gender cartesian-expansion count (approximate)              |
| P6             | determinism: same input twice gives byte-identical output               |
| P7             | exit codes, and the empty-input error document is well-formed           |
| P8             | after consuming argv words, morpheus exits instead of hanging on stdin  |
| N1             | long-form vocabulary (`nominative`) instead of cruncher's abbreviations |
| N2             | refined `<pofs>` categories, with a sentinel `order="0"` fallback       |
| N3             | runtime XML escaping (literal `&amp;`) instead of a pre-escaped table   |
| N4             | unknown words appear inline as `<unknown>` instead of going to stderr   |
| N5             | dialect table isn't truncated early; `prose`/`Homeric`/etc. reachable   |
| N6             | no literal `(null)` text leaks into gender/pofs output                  |
| N7             | comment lines produce a single well-formed `<words>` document           |
| N8             | a multi-token input line only analyzes the first token                  |

### Result levels

- `PASS`/`FAIL` — hard checks: build/link smoke tests, well-formedness,
  flag/argv-handling checks (flag-n-arg, argv-cleanup, P8), known/unknown-word
  partition parity (P1), lemma set/order parity (P2), stemtype set parity
  (P3), determinism (P6), exit codes (P7), and the N1–N8 divergence checks.
  A `FAIL` here blocks the suite (`exit 1`).
- `WARN` — soft checks: golden-baseline text diffs (informational — see
  above), and feature-token coverage (P4) / case-gender expansion counts (P5)
  from the comparator script, which use an approximate normalization map and
  can produce false positives on multi-value `<dial>`/`<morph>` tags. Don't
  block on these without reading the diff first.

  P4 deliberately excludes anything rendered via `<gend>` as gender is
  already validated via P1–P3 and N1, and P4 checking it too would just be
  redundant duplication of a stricter check.
- `XFAIL`/`XPASS` — reserved for a known, already-diagnosed defect that's
  expected to fail until fixed. None currently active in this suite; if you
  introduce one, add the `xfail`/`xpass` wrapper and a comment explaining
  what's broken, and convert it back to plain `pass`/`fail` once fixed.

Requires `python3` (or `xmllint`) for well-formedness checks and
`bin/cruncher` for the parity suite. Both are skipped gracefully if absent.