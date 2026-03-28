#!/bin/bash
#
# Regenerate all test baseline files from the current stemlib state.
# Run this before making changes, so that run_tests.sh can show diffs.
#

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
CRUNCHER="$PROJECT_DIR/bin/cruncher"

if [ ! -x "$CRUNCHER" ]; then
    echo "Error: cruncher not found (run 'cd src && make && make install' first)"
    exit 1
fi

export MORPHLIB="$PROJECT_DIR/stemlib"

# Rebuild stemlibs to ensure baselines match a clean build
for lang in Greek Latin; do
    echo "Building $lang stemlib ..."
    (cd "$PROJECT_DIR/stemlib/$lang" && \
     PATH="$PROJECT_DIR/bin:$PATH" MORPHLIB=.. make -B all) > /dev/null 2>&1
done

echo "Generating baselines ..."

# Main cruncher tests
"$CRUNCHER"    < "$SCRIPT_DIR/greek_words.txt" > "$SCRIPT_DIR/greek_expected.txt" 2>/dev/null
"$CRUNCHER" -L < "$SCRIPT_DIR/latin_words.txt" > "$SCRIPT_DIR/latin_expected.txt" 2>/dev/null

# Flag-variant probe tests
for flags in d l Px e Pk V; do
    "$CRUNCHER" -$flags    < "$SCRIPT_DIR/greek_probe.txt" > "$SCRIPT_DIR/greek_probe_${flags}.txt" 2>/dev/null
done

for flags in d l Px e V; do
    "$CRUNCHER" -L$flags   < "$SCRIPT_DIR/latin_probe.txt" > "$SCRIPT_DIR/latin_probe_${flags}.txt" 2>/dev/null
done

# Special cases: -S (case-insensitive) and -n (no accents)
"$CRUNCHER" -LS < "$SCRIPT_DIR/latin_probe_upper.txt" > "$SCRIPT_DIR/latin_probe_S.txt" 2>/dev/null
"$CRUNCHER" -S  < "$SCRIPT_DIR/greek_probe_upper.txt" > "$SCRIPT_DIR/greek_probe_S.txt" 2>/dev/null
"$CRUNCHER" -n  < "$SCRIPT_DIR/greek_probe_noaccent.txt" > "$SCRIPT_DIR/greek_probe_n.txt" 2>/dev/null

echo "Done. Baselines written to $SCRIPT_DIR/"
