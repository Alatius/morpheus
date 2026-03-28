morpheus
========

Morpheus parser code — a morphological analyzer for Greek and Latin.

Building
--------

Build everything (cruncher, tools, and stem libraries) from the project root:
```
make
```

This compiles the C source in `src/`, installs binaries to `bin/`, and builds
the Greek and Latin stem libraries in `stemlib/`.

To rebuild just the C source without the stem libraries:
```
make build
```

Testing
-------

```
make test
```

This runs the full test suite: cruncher output tests for all supported flag
combinations, followed by a from-scratch rebuild of both stem libraries to
verify the build pipeline. Test baselines are not tracked in git. To generate
or update them:
```
make baselines
```

Baselines capture the current cruncher output so that future changes show up
as diffs. A diff is reported but does not count as a failure — only crashes
and build errors do.

Running the cruncher
--------------------
```
MORPHLIB=stemlib bin/cruncher < wordlist > crunched
```

When reading from stdin, analyses are written to stdout and failed words and
statistics are written to stderr. Alternatively, an input filename can be given
as an argument (a `.words` extension is appended), in which case output goes to
`.morph`, `.failed`, and `.stats` files.

### Output format flags

By default, the cruncher outputs in Perseus format.

`-P` disables Perseus format, falling through to a human-readable display that
shows each word with its possible lemmas and full morphological breakdowns using
`$..&` beta-code markup.

`-d` selects database format: labeled lines (`:raw`, `:workw`, `:lem`, `:prvb`,
`:aug1`, `:stem`, `:suff`, `:end`) with the morphological flags for each
morpheme. Implies `-P`.

`-x` selects lexicon format: one tab-delimited line per analysis with rawword,
workword (if different), lemma, morphological flags, and crasis. Must be
combined with `-P` to take effect (Perseus format takes priority otherwise).

`-e` changes the morphological flags to a numeric form-info value. In Perseus
format (the default), this replaces the text flags in `<NL>` lines. Combined
with `-P`, it selects its own output format showing stem type, ending string,
paradigm info, and word form.

`-p` selects the `:summ`/`:stem`/`:end` format (like `-d` but more compact,
with a one-line `:summ` summary per analysis). Also collapses whitespace in the
detail lines. Must be combined with `-P` to take effect.

`-l` outputs lemma information only: `form:` followed by the input word, then
one line per unique lemma. Overrides all other format flags.

### Analysis flags

`-L` sets the language to Latin (default is Greek).

`-I` sets the language to Italian.

`-S` disables strict case matching (enabled by default). Allows analysis of
capitalized words by lowercasing them, useful for sentence-initial words.

`-n` enables accent-insensitive analysis. If normal analysis fails, retries
with smooth and then rough breathing variants.

`-V` restricts analysis to verb forms only, skipping nominal and indeclinable
checks.

### Other flags

`-k` keeps beta-code transliteration in the output. Only has effect with `-P`
(in the human-readable format, controls whether output is converted to
smart-alpha).

`-T` disables performance timing (timing is on by default). Suppresses the
`:longtime` and `:time` progress messages on stderr.

`-o FILE` writes output to `FILE` (with `.failed` and `.stats` companion
files). Use `-o -` to explicitly route output to stdout and failures/stats to
stderr.
