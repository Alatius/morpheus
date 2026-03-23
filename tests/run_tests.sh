#!/bin/bash
#
# Regression test for the Morpheus cruncher and build pipeline.
#

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CRUNCHER="$PROJECT_DIR/bin/cruncher"

if [ ! -x "$CRUNCHER" ]; then
    echo "FAIL: cruncher not found (run 'cd src && make && make install' first)"
    exit 1
fi

FAILURES=0

# Build stemlibs if needed (ensures test works from a clean checkout)
if [ ! -f "$PROJECT_DIR/stemlib/Greek/steminds/nomind" ]; then
    echo "     (building Greek stemlib ...)"
    if ! (cd "$PROJECT_DIR/stemlib/Greek" && PATH="$PROJECT_DIR/bin:$PATH" MORPHLIB=.. make -B all) \
            > /dev/null 2>&1; then
        echo "FAIL: Greek stemlib build"
        FAILURES=$((FAILURES + 1))
    fi
fi

if [ ! -f "$PROJECT_DIR/stemlib/Latin/steminds/nomind" ]; then
    echo "     (building Latin stemlib ...)"
    if ! (cd "$PROJECT_DIR/stemlib/Latin" && PATH="$PROJECT_DIR/bin:$PATH" MORPHLIB=.. make -B all) \
            > /dev/null 2>&1; then
        echo "FAIL: Latin stemlib build"
        FAILURES=$((FAILURES + 1))
    fi
fi

run_cruncher_test() {
    local label="$1" lang_flag="$2" words="$3" expected="$4"

    local actual
    actual=$(MORPHLIB="$PROJECT_DIR/stemlib" "$CRUNCHER" $lang_flag < "$words" 2>/dev/null)

    if diff -u "$expected" - <<< "$actual" > /dev/null 2>&1; then
        echo "PASS: $label"
    else
        echo "FAIL: $label"
        diff -u "$expected" - <<< "$actual" | head -40
        FAILURES=$((FAILURES + 1))
    fi
}

# Test cruncher output against baselines
run_cruncher_test "Greek cruncher" "" \
    "$SCRIPT_DIR/greek_words.txt" \
    "$SCRIPT_DIR/greek_expected.txt"

run_cruncher_test "Latin cruncher" "-L" \
    "$SCRIPT_DIR/latin_words.txt" \
    "$SCRIPT_DIR/latin_expected.txt"

# Flag-variant tests using small probe word lists
run_cruncher_test "Greek -d (dbase)" "-d" \
    "$SCRIPT_DIR/greek_probe.txt" \
    "$SCRIPT_DIR/greek_probe_d.txt"

run_cruncher_test "Latin -d (dbase)" "-Ld" \
    "$SCRIPT_DIR/latin_probe.txt" \
    "$SCRIPT_DIR/latin_probe_d.txt"

run_cruncher_test "Greek -l (lemma)" "-l" \
    "$SCRIPT_DIR/greek_probe.txt" \
    "$SCRIPT_DIR/greek_probe_l.txt"

run_cruncher_test "Latin -l (lemma)" "-Ll" \
    "$SCRIPT_DIR/latin_probe.txt" \
    "$SCRIPT_DIR/latin_probe_l.txt"

run_cruncher_test "Greek -Px (lexicon)" "-Px" \
    "$SCRIPT_DIR/greek_probe.txt" \
    "$SCRIPT_DIR/greek_probe_x.txt"

run_cruncher_test "Latin -Px (lexicon)" "-LPx" \
    "$SCRIPT_DIR/latin_probe.txt" \
    "$SCRIPT_DIR/latin_probe_x.txt"

run_cruncher_test "Greek -e (ending index)" "-e" \
    "$SCRIPT_DIR/greek_probe.txt" \
    "$SCRIPT_DIR/greek_probe_e.txt"

run_cruncher_test "Latin -e (ending index)" "-Le" \
    "$SCRIPT_DIR/latin_probe.txt" \
    "$SCRIPT_DIR/latin_probe_e.txt"

run_cruncher_test "Greek -Pk (keep beta)" "-Pk" \
    "$SCRIPT_DIR/greek_probe.txt" \
    "$SCRIPT_DIR/greek_probe_Pk.txt"

run_cruncher_test "Greek -V (verbs only)" "-V" \
    "$SCRIPT_DIR/greek_probe.txt" \
    "$SCRIPT_DIR/greek_probe_V.txt"

run_cruncher_test "Latin -V (verbs only)" "-LV" \
    "$SCRIPT_DIR/latin_probe.txt" \
    "$SCRIPT_DIR/latin_probe_V.txt"

run_cruncher_test "Latin -S (non-strict case)" "-LS" \
    "$SCRIPT_DIR/latin_probe_upper.txt" \
    "$SCRIPT_DIR/latin_probe_S.txt"

run_cruncher_test "Greek -S (non-strict case)" "-S" \
    "$SCRIPT_DIR/greek_probe_upper.txt" \
    "$SCRIPT_DIR/greek_probe_S.txt"

run_cruncher_test "Greek -n (ignore accents)" "-n" \
    "$SCRIPT_DIR/greek_probe_noaccent.txt" \
    "$SCRIPT_DIR/greek_probe_n.txt"

# Rebuild stemlibs from source and verify output files are byte-identical
echo "     (rebuilding Greek stemlib ...)"
if ! (cd "$PROJECT_DIR/stemlib/Greek" && PATH="$PROJECT_DIR/bin:$PATH" MORPHLIB=.. make -B all) \
        > /dev/null 2>&1; then
    echo "FAIL: Greek stemlib rebuild (build failed)"
    FAILURES=$((FAILURES + 1))
fi

echo "     (rebuilding Latin stemlib ...)"
if ! (cd "$PROJECT_DIR/stemlib/Latin" && PATH="$PROJECT_DIR/bin:$PATH" MORPHLIB=.. make -B all) \
        > /dev/null 2>&1; then
    echo "FAIL: Latin stemlib rebuild (build failed)"
    FAILURES=$((FAILURES + 1))
fi

CHECKSUMS="$SCRIPT_DIR/stemlib_checksums.txt"
if [ -f "$CHECKSUMS" ]; then
    actual=$(cd "$PROJECT_DIR" && awk '{print $NF}' "$CHECKSUMS" | xargs cksum)
    if diff -u "$CHECKSUMS" - <<< "$actual" > /dev/null 2>&1; then
        echo "PASS: stemlib rebuild ($(wc -l < "$CHECKSUMS" | tr -d ' ') files verified)"
    else
        echo "FAIL: stemlib rebuild (files differ after rebuild)"
        diff -u "$CHECKSUMS" - <<< "$actual" | head -20
        FAILURES=$((FAILURES + 1))
    fi
else
    echo "SKIP: stemlib checksums (stemlib_checksums.txt not found)"
fi

echo ""
if [ "$FAILURES" -gt 0 ]; then
    echo "$FAILURES test(s) failed."
    exit 1
else
    echo "All tests passed."
    exit 0
fi
