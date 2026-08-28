#!/bin/bash
#
# Regression test for the Morpheus cruncher and build pipeline.
#
# Usage:
#   tests/run_tests.sh           Run tests, comparing against baselines.
#   tests/run_tests.sh --update  Regenerate baselines from current state.
#
# Baseline files (tests/*_expected.txt, tests/*_probe_*.txt, etc.) are not
# tracked in git.  If baselines exist, this script compares against them and
# shows diffs, but a diff alone does not count as a failure -- only crashes
# and build errors do.
#

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CRUNCHER="$PROJECT_DIR/bin/cruncher"
UPDATE=0

if [ "$1" = "--update" ]; then
    UPDATE=1
fi

if [ ! -x "$CRUNCHER" ]; then
    echo "FAIL: cruncher not found (run 'make' first)"
    exit 1
fi

export MORPHLIB="$PROJECT_DIR/stemlib"
export PATH="$PROJECT_DIR/bin:$PATH"

FAILURES=0
DIFFS=0

# ── Test matrix ──────────────────────────────────────────────────────────
#
# Each entry: label, cruncher flags, input word list, baseline file.
# Adding a test here is all that is needed -- both test and baseline
# generation use this table.

run_cruncher_test() {
    local label="$1" flags="$2" words="$3" baseline="$4"

    if [ "$UPDATE" -eq 1 ]; then
        "$CRUNCHER" $flags < "$words" > "$baseline" 2>/dev/null
        return
    fi

    if ! "$CRUNCHER" $flags < "$words" > /tmp/morpheus_test_$$.txt 2>/dev/null; then
        echo "FAIL: $label (cruncher crashed)"
        rm -f /tmp/morpheus_test_$$.txt
        FAILURES=$((FAILURES + 1))
        return
    fi

    if [ ! -f "$baseline" ]; then
        echo "SKIP: $label (no baseline; run $0 --update)"
        rm -f /tmp/morpheus_test_$$.txt
        return
    fi

    if diff -u "$baseline" /tmp/morpheus_test_$$.txt > /dev/null 2>&1; then
        echo "PASS: $label"
    else
        echo "DIFF: $label"
        diff -u "$baseline" /tmp/morpheus_test_$$.txt | head -40
        DIFFS=$((DIFFS + 1))
    fi
    rm -f /tmp/morpheus_test_$$.txt
}

run_assert_test() {
    local flags="$1" file="$2"
    [ -f "$file" ] || return
    
    local filename
    filename=$(basename "$file")
    echo "=== Assertions: $filename ==="
    
    # AWK: Group lemmas, skip comments, chop '(', and handle "" for empty sets
    local aggregated
    aggregated=$(awk '/^#|^$/ {next} {
        
        # 1. Ignore everything from "(" to the end of the line
        sub(/\(.*/, "")
        
        if (NF < 2) next
        
        word = $1
        if (!(word in map)) {
            order[++count] = word
            map[word] = "" # Initialize empty in case it is just ""
        }
        for(i=2; i<=NF; i++) {
            # Skip arrow and our empty set marker
            if ($i == "->") continue
            if ($i == "\"\"") continue
            
            map[word] = map[word] " " $i
        }
    } END {
        for(i=1; i<=count; i++) {
            print order[i] map[order[i]]
        }
    }' "$file")

    # Loop through the grouped data
    while read -r form expected_str; do
        [ -z "$form" ] && continue
        
        # Get actual output, remove 'form:', sort alphabetically, format as one line
        actual=$(echo "$form" | "$CRUNCHER" $flags 2>/dev/null | grep -v '^form:' | sort | tr '\n' ' ' | sed 's/ *$//')
        
        # Format the expected string (sort alphabetically and remove duplicates)
        expected=$(echo "$expected_str" | tr ' ' '\n' | sort | uniq | tr '\n' ' ' | sed 's/ *$//')
        
        # Format for display so empty strings show up as "" in the terminal logs
        disp_expected=${expected:-\"\"}
        disp_actual=${actual:-\"\"}
        
        if [ "$actual" = "$expected" ]; then
            echo "PASS: '$form' -> $disp_expected"
        else
            echo "FAIL: $form"
            echo "      expected: [$disp_expected]"
            echo "      got:      [$disp_actual]"
            FAILURES=$((FAILURES + 1))
        fi
    done <<< "$aggregated"
    echo ""
}

for assert_file in "$SCRIPT_DIR"/latin_assertions*.txt; do
    run_assert_test "-Ll" "$assert_file"
done
GP="$SCRIPT_DIR/greek_probe.txt"
LP="$SCRIPT_DIR/latin_probe.txt"

# Main cruncher tests (full word lists)
run_cruncher_test "Greek cruncher" "" \
    "$SCRIPT_DIR/greek_words.txt" "$SCRIPT_DIR/greek_expected.txt"
run_cruncher_test "Latin cruncher" "-L" \
    "$SCRIPT_DIR/latin_words.txt" "$SCRIPT_DIR/latin_expected.txt"

# Output format flags
run_cruncher_test "Greek -d (dbase)"        "-d"   "$GP" "$SCRIPT_DIR/greek_probe_d.txt"
run_cruncher_test "Latin -d (dbase)"        "-Ld"  "$LP" "$SCRIPT_DIR/latin_probe_d.txt"
run_cruncher_test "Greek -l (lemma)"        "-l"   "$GP" "$SCRIPT_DIR/greek_probe_l.txt"
run_cruncher_test "Latin -l (lemma)"        "-Ll"  "$LP" "$SCRIPT_DIR/latin_probe_l.txt"
run_cruncher_test "Greek -Px (lexicon)"     "-Px"  "$GP" "$SCRIPT_DIR/greek_probe_Px.txt"
run_cruncher_test "Latin -Px (lexicon)"     "-LPx" "$LP" "$SCRIPT_DIR/latin_probe_Px.txt"
run_cruncher_test "Greek -Pp (parse)"       "-Pp"  "$GP" "$SCRIPT_DIR/greek_probe_Pp.txt"
run_cruncher_test "Latin -Pp (parse)"       "-LPp" "$LP" "$SCRIPT_DIR/latin_probe_Pp.txt"
run_cruncher_test "Greek -e (ending index)" "-e"   "$GP" "$SCRIPT_DIR/greek_probe_e.txt"
run_cruncher_test "Latin -e (ending index)" "-Le"  "$LP" "$SCRIPT_DIR/latin_probe_e.txt"
run_cruncher_test "Greek -Pk (keep beta)"   "-Pk"  "$GP" "$SCRIPT_DIR/greek_probe_Pk.txt"

# Analysis flags
run_cruncher_test "Greek -V (verbs only)"       "-V"  "$GP" "$SCRIPT_DIR/greek_probe_V.txt"
run_cruncher_test "Latin -V (verbs only)"       "-LV" "$LP" "$SCRIPT_DIR/latin_probe_V.txt"
run_cruncher_test "Greek -S (non-strict case)"  "-S"  \
    "$SCRIPT_DIR/greek_probe_upper.txt" "$SCRIPT_DIR/greek_probe_S.txt"
run_cruncher_test "Latin -S (non-strict case)"  "-LS" \
    "$SCRIPT_DIR/latin_probe_upper.txt" "$SCRIPT_DIR/latin_probe_S.txt"
run_cruncher_test "Greek -n (ignore accents)"   "-n"  \
    "$SCRIPT_DIR/greek_probe_noaccent.txt" "$SCRIPT_DIR/greek_probe_n.txt"

# ── Baseline update mode ────────────────────────────────────────────────

if [ "$UPDATE" -eq 1 ]; then
    echo "Baselines written to $SCRIPT_DIR/"
    exit 0
fi

# ── Stemlib rebuild verification ─────────────────────────────────────────

# Build stemlibs if needed (ensures test works from a clean checkout)
for lang in Greek Latin; do
    dir="$PROJECT_DIR/stemlib/$lang"
    if [ ! -f "$dir/steminds/nomind" ]; then
        echo "     (building $lang stemlib ...)"
        if ! (cd "$dir" && MORPHLIB=.. make -B all) > /dev/null 2>&1; then
            echo "FAIL: $lang stemlib build"
            FAILURES=$((FAILURES + 1))
        fi
    fi
done

# Rebuild stemlibs from source and verify the build succeeds
for lang in Greek Latin; do
    dir="$PROJECT_DIR/stemlib/$lang"
    echo "     (rebuilding $lang stemlib ...)"
    if ! (cd "$dir" && MORPHLIB=.. make -B all) > /dev/null 2>&1; then
        echo "FAIL: $lang stemlib rebuild"
        FAILURES=$((FAILURES + 1))
    else
        echo "PASS: $lang stemlib rebuild"
    fi
done

echo ""
if [ "$FAILURES" -gt 0 ]; then
    echo "$FAILURES failure(s)."
    [ "$DIFFS" -gt 0 ] && echo "$DIFFS baseline diff(s)."
    exit 1
elif [ "$DIFFS" -gt 0 ]; then
    echo "$DIFFS baseline diff(s) (review above). No failures."
    exit 0
else
    echo "All tests passed."
    exit 0
fi
