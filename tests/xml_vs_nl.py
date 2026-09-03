#!/usr/bin/env python3
"""
xml_vs_nl.py -- parity comparator between cruncher's <NL> Perseus-format
output and morpheus's XML output.

Implements a best-effort version of invariants P1-P5 from the morpheus XML
test plan:

  P1  known/unknown-word partition parity
  P2  lemma set + first-occurrence order parity
  P3  stemtype set parity (whole word, deduped)
  P4  feature-value coverage under a normalization map      (soft: WARN)
  P5  case/gender cartesian-expansion sanity                (soft: WARN)

Design notes
------------
Both binaries are invoked *once per word*, feeding the single word on
stdin.  This sidesteps the fact that cruncher prints *nothing at all* to
stdout for an unknown word (so there is no way to realign a multi-word
batch run against the input list).  It is slower than a single batch
call, but for the small curated fixture lists used by the test suite this
is a non-issue, and it is far more robust than trying to re-derive block
boundaries from cruncher's flat stream.

Exit status is 1 if any *hard* check (P1-P3) failed for any word, 0
otherwise.  P4/P5 mismatches are printed as WARN and never affect the
exit status, because the mapping between cruncher's single-line combined
notation and morpheus's cartesian-expanded <infl> elements is not fully
specified anywhere and this script's normalization is necessarily
approximate.

stdlib only (per the project's testing conventions).
"""

import argparse
import re
import subprocess
import sys
import xml.etree.ElementTree as ET

# --------------------------------------------------------------------------
# cruncher morphkeys.h abbreviation -> long-form vocabulary used by morpheus
# --------------------------------------------------------------------------
NORM = {
    "nom": "nominative",
    "gen": "genitive",
    "dat": "dative",
    "acc": "accusative",
    "voc": "vocative",
    "abl": "ablative",
    "masc": "masculine",
    "fem": "feminine",
    "neut": "neuter",
    "sg": "singular",
    "pl": "plural",
    "dual": "dual",
    "1st": "1st",
    "2nd": "2nd",
    "3rd": "3rd",
    "pres": "present",
    "imperf": "imperfect",
    "fut": "future",
    "aor": "aorist",
    "perf": "perfect",
    "plup": "pluperfect",
    "futperf": "future perfect",
    "ind": "indicative",
    "subj": "subjunctive",
    "opt": "optative",
    "imperat": "imperative",
    "inf": "infinitive",
    "part": "participle",
    "gerundive": "gerundive",
    "supine": "supine",
    "act": "active",
    "mid": "middle",
    "pass": "passive",
    "mp": "mediopassive",
    "dep": "deponent",
    "attic": "Attic",
    "ionic": "Ionic",
    "doric": "Doric",
    "aeolic": "Aeolic",
    "epic": "epic",
    "homeric": "Homeric",
    "parad_form": "paradigm form",
    "comp": "comparative",
    "comparative": "comparative",
    "superl": "superlative",
    "superlative": "superlative",
}

CASE_ABBRS = ("nom", "gen", "dat", "acc", "voc", "abl")
GEND_ABBRS = ("masc", "fem", "neut")

NL_RE = re.compile(r"<NL>(.*?)</NL>", re.S)


def norm_tok(tok):
    return NORM.get(tok, tok)


def run_cmd(binary, flags, word, timeout=15):
    args = [binary] + list(flags)
    try:
        p = subprocess.run(
            args,
            check=False,
            capture_output=True,
            input=word + "\n",
            text=True,
            timeout=timeout,
        )
        return p.stdout, p.returncode
    except (OSError, subprocess.SubprocessError):
        return "", -1


def parse_cruncher(stdout):
    """Return a list of dicts: type, workword, lemma, features, stemtype, derivtype."""
    entries = []
    for m in NL_RE.finditer(stdout):
        toks = m.group(1).split()
        if len(toks) < 2:
            continue
        typ = toks[0]
        lemma_field = toks[1]
        if "," in lemma_field:
            workword, lemma = lemma_field.split(",", 1)
        else:
            workword, lemma = None, lemma_field
        rest = toks[2:]
        stemfield = rest[-1] if rest else ""
        features = rest[:-1]
        if "," in stemfield:
            stemtype, derivtype = stemfield.split(",", 1)
        else:
            stemtype, derivtype = stemfield, ""
        entries.append(
            dict(
                type=typ,
                workword=workword,
                lemma=lemma,
                features=features,
                stemtype=stemtype,
                derivtype=derivtype,
            )
        )
    return entries


def parse_morpheus(stdout):
    """Return (known, entries, parse_error).

    entries: list of {hdwd, stemtypes:set, infls:list[dict]}
    known:   True/False, or None if the document did not parse at all.
    """

    try:
        root = ET.fromstring(stdout)
    except ET.ParseError as e:
        return None, [], str(e)

    word_el = root.find("word")
    if word_el is None:
        return False, [], None

    entries = []
    for entry in word_el.findall("entry"):
        d = entry.find("dict")
        hdwd_el = d.find("hdwd") if d is not None else None
        hdwd = hdwd_el.text if hdwd_el is not None else None
        stemtypes = set()
        infls = []
        for infl in entry.findall("infl"):
            info = {}
            for tag in (
                "case",
                "gend",
                "mood",
                "tense",
                "voice",
                "pers",
                "num",
                "comp",
                "dial",
                "stemtype",
                "morph",
                "pofs",
            ):
                el = infl.find(tag)
                if el is not None and el.text:
                    info[tag] = el.text
            st = infl.find("stemtype")
            if st is not None and st.text:
                stemtypes.add(st.text)
            infls.append(info)
        entries.append(dict(hdwd=hdwd, stemtypes=stemtypes, infls=infls))
    return True, entries, None


def is_case_combo(tok):
    return all(p in CASE_ABBRS for p in tok.split("/"))


def is_gend_combo(tok):
    return all(p in GEND_ABBRS for p in tok.split("/"))


def compare_word(word, cbin, cflags, mbin, mflags):
    lines = []
    hard_fail = False

    cout, _ = run_cmd(cbin, cflags, word)
    mout, _ = run_cmd(mbin, mflags, word)

    c_entries = parse_cruncher(cout)
    c_known = len(c_entries) > 0

    m_known, m_entries, perr = parse_morpheus(mout)
    if m_known is None:
        lines.append(f"FAIL: [{word}] morpheus output is not well-formed XML: {perr}")
        return lines, True

    # ---- P1: known/unknown partition -------------------------------------
    if c_known == m_known:
        lines.append(
            f"PASS: [{word}] P1 known/unknown parity "
            f"({'known' if c_known else 'unknown'})"
        )
    else:
        lines.append(
            f"FAIL: [{word}] P1 known/unknown mismatch: "
            f"cruncher={c_known} morpheus={m_known}"
        )
        hard_fail = True

    if not c_known or not m_known:
        return lines, hard_fail

    # ---- P2: lemma set + first-occurrence order --------------------------
    c_lemmas = []
    for e in c_entries:
        if e["lemma"] not in c_lemmas:
            c_lemmas.append(e["lemma"])
    m_lemmas = []
    for e in m_entries:
        if e["hdwd"] and e["hdwd"] not in m_lemmas:
            m_lemmas.append(e["hdwd"])

    if set(c_lemmas) == set(m_lemmas):
        lines.append(f"PASS: [{word}] P2 lemma set parity {sorted(set(c_lemmas))}")
        if c_lemmas == m_lemmas:
            lines.append(f"PASS: [{word}] P2 lemma order parity")
        else:
            lines.append(
                f"FAIL: [{word}] P2 lemma order mismatch: "
                f"cruncher={c_lemmas} morpheus={m_lemmas}"
            )
            hard_fail = True
    else:
        lines.append(
            f"FAIL: [{word}] P2 lemma set mismatch: "
            f"cruncher={sorted(set(c_lemmas))} morpheus={sorted(set(m_lemmas))}"
        )
        hard_fail = True

    # ---- P3: stemtype set parity (whole word, deduped) --------------------
    c_stemtypes = {e["stemtype"] for e in c_entries if e["stemtype"]}
    m_stemtypes = set()
    for e in m_entries:
        m_stemtypes |= e["stemtypes"]

    if c_stemtypes == m_stemtypes:
        lines.append(f"PASS: [{word}] P3 stemtype set parity {sorted(c_stemtypes)}")
    else:
        lines.append(
            f"FAIL: [{word}] P3 stemtype set mismatch: "
            f"cruncher={sorted(c_stemtypes)} morpheus={sorted(m_stemtypes)}"
        )
        hard_fail = True

    # ---- P4 (soft): normalized feature-token coverage ----------------------

    # Gender values that don't participate in masc/fem/neut slash-combos, so
    # is_gend_combo() alone won't catch them, but are still rendered via <gend>
    # rather than anything P4 scans. Excluded here rather than documented as a
    # permanent WARN, since a check that can never pass shouldn't fire at all.
    NON_COMBO_GEND_TOKENS = {"adverbial"}

    for e in c_entries:
        plain_tokens = [
            t
            for t in e["features"]
            if not is_case_combo(t)
            and not is_gend_combo(t)
            and t not in NON_COMBO_GEND_TOKENS
        ]
        wanted = {norm_tok(t) for t in plain_tokens}
        pool = [i for me in m_entries if me["hdwd"] == e["lemma"] for i in me["infls"]]
        if not pool:
            continue
        seen_raw = [
            v
            for info in pool
            for k, v in info.items()
            if k not in ("case", "gend", "stemtype", "pofs")
        ]
        missing = {w for w in wanted if not any(w in v for v in seen_raw)}
        if missing:
            lines.append(
                f"WARN: [{word}] P4 feature coverage: lemma={e['lemma']} "
                f"tokens not observed in any matching <infl> "
                f"(normalization is approximate): "
                f"{sorted(m for m in missing if m is not None)}"
            )

    # ---- P5 (soft): case/gender cartesian-expansion sanity -----------------
    for e in c_entries:
        case_tok = next((t for t in e["features"] if is_case_combo(t)), None)
        gend_tok = next((t for t in e["features"] if is_gend_combo(t)), None)
        if not case_tok and not gend_tok:
            continue
        cases = [norm_tok(c) for c in (case_tok.split("/") if case_tok else [None])]
        genders = [norm_tok(g) for g in (gend_tok.split("/") if gend_tok else [None])]
        expected = len(cases) * len(genders)
        pool = [i for me in m_entries if me["hdwd"] == e["lemma"] for i in me["infls"]]
        combos = set()
        for info in pool:
            c = info.get("case")
            g = info.get("gend")
            case_ok = case_tok is None or c in cases
            gend_ok = gend_tok is None or g in genders
            if case_ok and gend_ok:
                combos.add((c, g))
        if len(combos) < expected:
            lines.append(
                f"WARN: [{word}] P5 expansion: lemma={e['lemma']} "
                f"case={case_tok} gend={gend_tok}: expected >= {expected} distinct "
                f"case/gender <infl> combos, found {len(combos)}"
            )

    return lines, hard_fail


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("wordlist")
    ap.add_argument("--cruncher", required=True)
    ap.add_argument("--cruncher-flags", default="")
    ap.add_argument("--morpheus", required=True)
    ap.add_argument("--morpheus-flags", default="")
    args = ap.parse_args()

    cflags = args.cruncher_flags.split()
    mflags = args.morpheus_flags.split()

    with open(args.wordlist, encoding="utf-8") as f:
        words = [w.strip() for w in f if w.strip() and not w.startswith("#")]

    any_hard_fail = False
    for word in words:
        try:
            lines, hard_fail = compare_word(
                word, args.cruncher, cflags, args.morpheus, mflags
            )
        # don't let one bad word kill the whole run
        except Exception as e:  # pylint: disable=broad-except
            print(f"FAIL: [{word}] comparator raised an exception: {e}")
            any_hard_fail = True
            continue
        for line in lines:
            print(line)
        any_hard_fail = any_hard_fail or hard_fail

    sys.exit(1 if any_hard_fail else 0)


if __name__ == "__main__":
    main()
