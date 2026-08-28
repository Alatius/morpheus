## Lemma assertion tests (latin_assertions_*.txt)

Format: `form <TAB> lemma1 lemma2 ...`, optional trailing
`(commentary)` ignored by the harness. Multiple lines for the
same form are unioned into one expected set.

File names reflect which stemsrc file was under investigation
when the regression was found and NOT which file the underlying
fix lives in. A single word's analysis is often split across
several stemsrc files (nom.latin.bas, ls.nom, vbs.latin, ...),
so there is no clean one-file-per-source mapping.

Before adding a new assertion, grep all existing
latin_assertions_*.txt for the headword -- it may already be
tracked under a different file's name. Occasionally run:
```bash
    awk '/^#|^$/{next} { sub(/\(.*/,""); if (NF<2) next; if (!seen[$1 SUBSEP FILENAME]++) filecount[$1]++ } END { for (w in filecount) if (filecount[w]>1) print w }' tests/latin_assertions*.txt
```
to catch a form asserted in more than one file.

New investigative work gets its own new latin_assertions_<name>.txt
-- the run_tests.sh loop picks up any matching file automatically.