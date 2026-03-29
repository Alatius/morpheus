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
