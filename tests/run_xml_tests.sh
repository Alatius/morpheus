#!/bin/bash
#
# XML output test suite for morpheus
#
# This is intentionally INDEPENDENT of tests/run_tests.sh and is NOT wired
# into the Makefile.
# Run it by hand:
#
#   bash tests/run_xml_tests.sh --update   (re)generate golden baselines
#   bash tests/run_xml_tests.sh            run tests against saved baselines
#
# Fixtures reused from the existing (tracked) test suite -- not modified:
#   tests/greek_words.txt tests/latin_words.txt tests/greek_probe.txt
#   tests/greek_probe_upper.txt tests/latin_probe_upper.txt
#   tests/greek_probe_noaccent.txt
#
# Golden baselines this script creates all end in "_expected.txt", which is
# already covered by the existing .gitignore rule (tests/*_expected.txt) --
# no .gitignore changes needed.
#
# Conventions (matching tests/run_tests.sh):
#   - A golden-baseline text diff is reported but does NOT fail the run.
#   - A crash, a malformed-XML document, or a violated parity contract
#     DOES fail the run.
#   - Known, already-diagnosed defects get a dedicated XFAIL test so that
#     fixing them shows up as a visible XPASS instead of silently doing
#     nothing.
#
set -u

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
MORPHEUS="$PROJECT_DIR/bin/morpheus"
CRUNCHER="$PROJECT_DIR/bin/cruncher"
COMPARATOR="$SCRIPT_DIR/xml_vs_nl.py"
PY=python3

UPDATE=0
[ "${1:-}" = "--update" ] && UPDATE=1

export MORPHLIB="$PROJECT_DIR/stemlib"
export PATH="$PROJECT_DIR/bin:$PATH"

PASS=0; FAIL=0; WARN=0; SKIP=0; XFAIL=0; XPASS=0

pass()  { echo "PASS:  $*";  PASS=$((PASS+1)); }
fail()  { echo "FAIL:  $*";  FAIL=$((FAIL+1)); }
warn()  { echo "WARN:  $*";  WARN=$((WARN+1)); }
skip()  { echo "SKIP:  $*";  SKIP=$((SKIP+1)); }
xfail() { echo "XFAIL: $* (known issue; expected to fail until fixed)"; XFAIL=$((XFAIL+1)); }
xpass() { echo "XPASS: $* -- unexpectedly passing now; remove the XFAIL marker for this test"; XPASS=$((XPASS+1)); }

hr() { echo; echo "--- $* ---"; }

print_summary() {
    echo
    echo "================================================================"
    echo " PASS=$PASS  FAIL=$FAIL  WARN=$WARN  SKIP=$SKIP  XFAIL=$XFAIL  XPASS=$XPASS"
    echo "================================================================"
}

finish() {
    print_summary
    [ "$FAIL" -gt 0 ] && exit 1
    exit 0
}

# ---------------------------------------------------------------------------
# Well-formedness helper (xmllint if present, else python3 stdlib, else SKIP)
# ---------------------------------------------------------------------------
have_wf_checker() {
    command -v xmllint >/dev/null 2>&1 || command -v "$PY" >/dev/null 2>&1
}

check_wellformed() {
    # returns 0 if well-formed, 1 otherwise. Caller must call have_wf_checker first.
    local f="$1"
    if command -v xmllint >/dev/null 2>&1; then
        xmllint --noout "$f" >/dev/null 2>&1
        return $?
    fi
    "$PY" -c '
import sys, xml.etree.ElementTree as ET
try:
    ET.parse(sys.argv[1])
except Exception:
    sys.exit(1)
' "$f"
    return $?
}

wf_check() {
    local label="$1" file="$2"
    if ! have_wf_checker; then
        skip "$label wellformed (no xmllint or python3 available)"
        return
    fi
    if check_wellformed "$file"; then
        pass "$label wellformed"
    else
        fail "$label wellformed"
    fi
}

# ---------------------------------------------------------------------------
# 1. Build/link smoke tests (5.4) 
# ---------------------------------------------------------------------------
hr "Build/link smoke tests"

if [ ! -x "$MORPHEUS" ]; then
    fail "bin/morpheus exists and is executable (build it with 'make')"
    echo
    echo "bin/morpheus is not available; skipping the rest of the XML test suite."
    finish
fi
pass "bin/morpheus exists and is executable"

"$MORPHEUS" </dev/null >/tmp/morpheus_smoke_$$ 2>/dev/null
rc=$?
rm -f /tmp/morpheus_smoke_$$
if [ "$rc" -eq 2 ]; then
    pass "morpheus </dev/null exits 2"
else
    fail "morpheus </dev/null exits 2 (got $rc)"
fi

for lang in Greek Latin; do
    if [ ! -f "$PROJECT_DIR/stemlib/$lang/steminds/nomind" ]; then
        echo "NOTE: stemlib/$lang does not look built (steminds/nomind missing)."
        echo "      Run 'make' at the project root first, or most words below"
        echo "      will show up as <unknown> and several tests will fail."
    fi
done

# ---------------------------------------------------------------------------
# 2. Golden baseline tests (5.1)
# ---------------------------------------------------------------------------
hr "Golden baseline tests"

run_morpheus_golden() {
    local label="$1" flags="$2" input="$3" baseline="$4"
    local out err
    out=$(mktemp); err=$(mktemp)
    "$MORPHEUS" $flags < "$input" > "$out" 2>"$err"
    local rc=$?

    if [ "$rc" -ge 128 ]; then
        fail "$label (morpheus crashed / killed, exit=$rc)"
        sed 's/^/    stderr: /' "$err"
        rm -f "$out" "$err"
        return
    fi
    rm -f "$err"

    if [ "$UPDATE" -eq 1 ]; then
        cp "$out" "$baseline"
        echo "updated: $baseline"
        rm -f "$out"
        return
    fi

    if [ ! -f "$baseline" ]; then
        skip "$label (no baseline; run 'bash $0 --update')"
    elif diff -u "$baseline" "$out" > /tmp/golden_diff_$$ 2>&1; then
        pass "$label matches baseline"
    else
        warn "$label differs from baseline (diff below; not a hard failure)"
        head -40 /tmp/golden_diff_$$
        rm -f /tmp/golden_diff_$$
    fi

    wf_check "$label" "$out"
    rm -f "$out"
}

GP="$SCRIPT_DIR/greek_probe.txt"

run_morpheus_golden "Greek words (default)"     ""    "$SCRIPT_DIR/greek_words.txt"          "$SCRIPT_DIR/greek_xml_expected.txt"
run_morpheus_golden "Latin words (-L)"          "-L"  "$SCRIPT_DIR/latin_words.txt"          "$SCRIPT_DIR/latin_xml_expected.txt"
run_morpheus_golden "Greek -i (dump_analysis)"  "-i"  "$GP"                                  "$SCRIPT_DIR/greek_xml_probe_i_expected.txt"
run_morpheus_golden "Greek -S (upper case)"     "-S"  "$SCRIPT_DIR/greek_probe_upper.txt"     "$SCRIPT_DIR/greek_xml_probe_S_expected.txt"
run_morpheus_golden "Latin -LS (upper case)"    "-LS" "$SCRIPT_DIR/latin_probe_upper.txt"     "$SCRIPT_DIR/latin_xml_probe_S_expected.txt"
run_morpheus_golden "Greek -n (no accent)"      "-n"  "$SCRIPT_DIR/greek_probe_noaccent.txt"  "$SCRIPT_DIR/greek_xml_probe_n_expected.txt"

if [ "$UPDATE" -eq 1 ]; then
    finish
fi

# ---------------------------------------------------------------------------
# 3. Regression tests for specific bugs of the original implementation for Alpheios
# ---------------------------------------------------------------------------
hr "Flag parsing and argv-handling tests"

# ARGS has "...n:" so bare -n incorrectly swallows the next argv token
# as its option-argument. Detect this without needing a real argv word: feed
# -n a word via argv with /dev/null stdin. If -n eats it, the program falls
# straight through to empty stdin and exits 2 ("No words provided"). If -n
# is fixed to take no argument, the word is processed from argv and it exits 0.
test_flag_n_no_arg() {
    "$MORPHEUS" -n 'lo/gos' </dev/null >/tmp/flag_n_$$ 2>/dev/null
    local rc=$?
    rm -f /tmp/flag_n_$$
    if [ "$rc" -eq 0 ]; then
        pass "flag-n-arg: -n does not consume the following argv word as its optarg"
    else
        fail "flag-n-arg: -n incorrectly requires an argument (ARGS has 'n:'); exit=$rc, expected 0"
    fi
}
test_flag_n_no_arg

# P8: after consuming argv words, morpheus must not fall through and
# block on stdin. Use a pipe that is held open (never closed, never written
# to) so a real attempt to read stdin would hang until `timeout` kills it.
test_p8() {
    if ! command -v timeout >/dev/null 2>&1; then
        skip "P8: morpheus does not hang after argv words (timeout(1) unavailable)"
        return
    fi
    local out
    out=$(mktemp)
    timeout 5 "$MORPHEUS" 'lo/gos' < <(sleep 30) > "$out" 2>/dev/null
    local rc=$?
    if [ "$rc" -eq 124 ]; then
        fail "P8: morpheus hung reading stdin after processing an argv word (timed out)"
    elif [ "$rc" -ne 0 ]; then
        fail "P8: unexpected exit code $rc for argv-word invocation"
    else
        local nwords
        nwords=$(grep -c '<word>' "$out")
        if [ "$nwords" -eq 1 ]; then
            pass "P8: morpheus with an argv word exits promptly with exactly one <word>"
        else
            fail "P8: expected exactly one <word> element, got $nwords"
        fi
    fi
    rm -f "$out"
}
test_p8

# argv words skip the same cleaning (trimdigit) that stdin words get.
test_argv_cleanup() {
    local w='lo/gos2'
    local out_argv out_stdin
    out_argv=$(mktemp); out_stdin=$(mktemp)
    "$MORPHEUS" "$w" </dev/null >"$out_argv" 2>/dev/null
    printf '%s\n' "$w" | "$MORPHEUS" >"$out_stdin" 2>/dev/null
    if grep -q '<word>' "$out_stdin"; then
        if grep -q '<word>' "$out_argv"; then
            pass "argv-cleanup: argv word '$w' is cleaned the same way as the stdin word"
        else
            fail "argv-cleanup: argv word '$w' not cleaned like the stdin word (stdin=known, argv=unknown)"
        fi
    else
        skip "argv-cleanup check for '$w': the stdin path itself did not recognize it"
    fi
    rm -f "$out_argv" "$out_stdin"
}
test_argv_cleanup

# ---------------------------------------------------------------------------
# 4. Positive parity tests (P1-P5 via comparator, plus P6/P7 directly)
# ---------------------------------------------------------------------------
hr "Positive parity tests (P1-P5, P6, P7)"

run_parity_suite() {
    local label="$1" wordlist="$2" cflags="$3" mflags="$4"
    if [ ! -x "$CRUNCHER" ]; then
        skip "$label (bin/cruncher missing)"
        return
    fi
    if ! command -v "$PY" >/dev/null 2>&1; then
        skip "$label (python3 unavailable)"
        return
    fi
    local out
    out=$(mktemp)
    "$PY" "$COMPARATOR" "$wordlist" \
        --cruncher "$CRUNCHER" --cruncher-flags="$cflags" \
        --morpheus "$MORPHEUS" --morpheus-flags="$mflags" \
        > "$out"
    local rc=$?
    cat "$out"
    PASS=$((PASS + $(grep -c '^PASS:' "$out")))
    FAIL=$((FAIL + $(grep -c '^FAIL:' "$out")))
    WARN=$((WARN + $(grep -c '^WARN:' "$out")))
    if [ "$rc" -ne 0 ]; then
        echo "--- $label: comparator reported hard parity failures (see above) ---"
    else
        echo "--- $label: all hard parity checks (P1-P3) passed ---"
    fi
    rm -f "$out"
}

run_parity_suite "Greek parity vs cruncher" "$SCRIPT_DIR/greek_words.txt" "" ""
run_parity_suite "Latin parity vs cruncher" "$SCRIPT_DIR/latin_words.txt" "-L" "-L"

# P6: determinism
test_p6() {
    local out1 out2
    out1=$(mktemp); out2=$(mktemp)
    "$MORPHEUS" < "$SCRIPT_DIR/greek_words.txt" > "$out1" 2>/dev/null
    "$MORPHEUS" < "$SCRIPT_DIR/greek_words.txt" > "$out2" 2>/dev/null
    if cmp -s "$out1" "$out2"; then
        pass "P6: morpheus output is byte-identical across repeated runs"
    else
        fail "P6: morpheus output differs between repeated runs (non-determinism)"
    fi
    rm -f "$out1" "$out2"
}
test_p6

# P7: exit codes + well-formedness of the empty-input error document
test_p7() {
    "$MORPHEUS" </dev/null >/dev/null 2>/dev/null
    local rc_empty=$?
    [ "$rc_empty" -eq 2 ] && pass "P7: empty stdin exits 2" \
                           || fail "P7: empty stdin expected exit 2, got $rc_empty"

    printf 'lo/gos\n' | "$MORPHEUS" >/dev/null 2>/dev/null
    local rc_ok=$?
    [ "$rc_ok" -eq 0 ] && pass "P7: normal input exits 0" \
                       || fail "P7: normal input expected exit 0, got $rc_ok"

    if ! have_wf_checker; then
        skip "P7: empty-input error document wellformed (no checker)"
        return
    fi
    local out
    out=$(mktemp)
    "$MORPHEUS" < /dev/null > "$out" 2>/dev/null
    if check_wellformed "$out"; then
        pass "P7: empty-input error document is well-formed XML"
    else
        fail "P7: empty-input error document is well-formed XML"
    fi
    rm -f "$out"
}
test_p7

# ---------------------------------------------------------------------------
# 5. Negative tests -- intentional divergences from cruncher (N1-N8)
# ---------------------------------------------------------------------------
hr "Negative tests (intentional divergences from cruncher)"

# N1: vocabulary (long-form case/number, not abbreviations)
test_n1() {
    local out
    out=$(mktemp)
    printf 'lo/gos\n' | "$MORPHEUS" >"$out" 2>/dev/null
    if grep -q '<case order="7">nominative</case>' "$out" \
       && grep -q '<num>singular</num>' "$out" \
       && ! grep -q '>nom<' "$out"; then
        pass "N1: XML uses long-form vocabulary (nominative/singular), not cruncher's abbreviations"
    else
        fail "N1: expected long-form case/number vocabulary for lo/gos"
    fi
    wf_check "N1 (lo/gos)" "$out"
    rm -f "$out"
}
test_n1

# N2: pofs refinement (article, sentinel order=0)
test_n2() {
    local out
    out=$(mktemp)
    printf 'o(\n' | "$MORPHEUS" >"$out" 2>/dev/null
    if grep -q '<pofs order="0">article</pofs>' "$out"; then
        pass "N2: XML refines article pofs with sentinel order=0 (cruncher just says 'I')"
    else
        fail "N2: expected <pofs order=\"0\">article</pofs> for o("
    fi
    wf_check "N2 (o()" "$out"
    rm -f "$out"
}
test_n2

# N3: runtime escaping (raw &amp; in <decl>, replacing alpheios's pre-escaped table)
test_n3() {
    local out
    out=$(mktemp)
    printf 'bonus\n' | "$MORPHEUS" -L >"$out" 2>/dev/null
    if grep -q '<decl>1st &amp; 2nd</decl>' "$out"; then
        pass "N3: runtime XML escaping produces literal &amp; in <decl>"
    else
        fail "N3: expected literal '&amp;' escaping in <decl> for bonus"
    fi
    wf_check "N3 (bonus)" "$out"
    rm -f "$out"
}
test_n3

# N4: unknown word handling (inline <unknown>, both languages)
test_n4() {
    local out
    out=$(mktemp)
    printf 'xyzzy\n' | "$MORPHEUS" >"$out" 2>/dev/null
    if grep -q '<unknown xml:lang="grc-x-beta">xyzzy</unknown>' "$out"; then
        pass "N4: unknown Greek word emitted inline as <unknown> inside <words>"
    else
        fail "N4: expected inline <unknown xml:lang=\"grc-x-beta\"> for xyzzy"
    fi
    wf_check "N4 (xyzzy, greek)" "$out"
    rm -f "$out"

    out=$(mktemp)
    printf 'xyzzy\n' | "$MORPHEUS" -L >"$out" 2>/dev/null
    if grep -q '<unknown xml:lang="lat">xyzzy</unknown>' "$out"; then
        pass "N4b: unknown Latin word emitted inline as <unknown>"
    else
        fail "N4b: expected inline <unknown xml:lang=\"lat\"> for xyzzy"
    fi
    wf_check "N4b (xyzzy, latin)" "$out"
    rm -f "$out"
}
test_n4

# N5: dialect table change (removal of {"all", ALL_DIAL} unblocks 'prose' etc.)
test_n5() {
    if [ ! -f "$SCRIPT_DIR/greek_xml_expected.txt" ]; then
        skip "N5: dialect table PROSE check (run --update first to create a baseline)"
        return
    fi
    if grep -q '<dial>[^<]*[Pp]rose' "$SCRIPT_DIR/greek_xml_expected.txt"; then
        pass "N5: <dial> contains 'prose' somewhere in the Greek fixture output"
    else
        warn "N5: no 'prose' dialect observed anywhere in the Greek fixture; add/confirm a word that exercises it"
    fi
}
test_n5

# N6: NULL-safety (no "(null)" leaking into gender/pofs output)
test_n6() {
    local out
    out=$(mktemp)
    "$MORPHEUS"    < "$SCRIPT_DIR/greek_words.txt" >> "$out" 2>/dev/null
    "$MORPHEUS" -L < "$SCRIPT_DIR/latin_words.txt" >> "$out" 2>/dev/null
    if grep -qi '(null)' "$out"; then
        fail "N6: literal '(null)' text found in XML output (NULL-safety regression)"
    else
        pass "N6: no '(null)' text in XML output (gender/pofs NULL guards hold)"
    fi
    rm -f "$out"
}
test_n6

# N7: comment handling
test_n7() {
    if ! have_wf_checker; then
        skip "N7: comment handling wellformedness checks (no checker)"
        return
    fi

    local out
    out=$(mktemp)
    printf '#hello\nlo/gos\n' | "$MORPHEUS" >"$out" 2>/dev/null
    local nopen nclose nword
    nopen=$(grep -c '<words>' "$out")
    nclose=$(grep -c '</words>' "$out")
    nword=$(grep -c '<word>' "$out")
    if [ "$nopen" -eq 1 ] && [ "$nclose" -eq 1 ] && [ "$nword" -eq 1 ] \
       && check_wellformed "$out"; then
        pass "N7a: comment-then-word yields a single well-formed <words> document"
    else
        fail "N7a: comment-then-word yields a single well-formed <words> document (open=$nopen close=$nclose word=$nword)"
    fi
    rm -f "$out"

    out=$(mktemp)
    printf '#just a comment\n' | "$MORPHEUS" >"$out" 2>/dev/null
    if check_wellformed "$out"; then
        pass "N7b: comment-only input produces a well-formed document"
    else
        fail "N7b: comment-only input produces a well-formed document"
    fi
    rm -f "$out"

    out=$(mktemp)
    printf '#a--b\n#a-->b\nlo/gos\n' | "$MORPHEUS" >"$out" 2>/dev/null
    if check_wellformed "$out"; then
        pass "N7c: pathological comment content (--, -->) stays well-formed"
    else
        fail "N7c: pathological comment content (--, -->) stays well-formed"
    fi
    rm -f "$out"
}
test_n7

# N8: first-token-only semantics (cruncher parity, diverges from alpheios original)
test_n8() {
    local out
    out=$(mktemp)
    printf 'lo/gos timh/\n' | "$MORPHEUS" >"$out" 2>/dev/null
    local nform
    nform=$(grep -c '<form ' "$out")
    if [ "$nform" -eq 1 ] && grep -q '>lo/gos<' "$out" && ! grep -q '>timh/<' "$out"; then
        pass "N8: multi-token line processes only the first token (cruncher parity)"
    else
        fail "N8: expected exactly one <form> for 'lo/gos' only (got $nform)"
    fi
    wf_check "N8 (lo/gos timh/)" "$out"
    rm -f "$out"
}
test_n8

# ---------------------------------------------------------------------------
finish
