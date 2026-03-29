all: build stemlibs

build:
	cd src && $(MAKE) all && $(MAKE) install

stemlibs: build
	cd stemlib/Greek && PATH=$(CURDIR)/bin:$$PATH MORPHLIB=.. $(MAKE) all
	cd stemlib/Latin && PATH=$(CURDIR)/bin:$$PATH MORPHLIB=.. $(MAKE) all

test: all
	bash tests/run_tests.sh

baselines: all
	bash tests/run_tests.sh --update

clean:
	cd src && $(MAKE) clean

.PHONY: all build stemlibs test baselines clean
