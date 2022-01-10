# Makefile

# --------------------
# To run all tests:
#	cd test
#	make test-all

# Local install (only for testing; use package intaller):
#	cd package
#	make dist-clean install

# To build installer packages:
#	cd package
#	make dist-clean package

# --------------------
# Config

mHtmlOpt = --cachedir=/tmp --index --backlink

# --------------------
check : doc git-core package test

clean : check
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

gen-doc :
	-mkdir doc/user-doc
	for tCmd in git-core/git-proj git-core/git-proj-*; do \
		pod2markdown $$tCmd >doc/user-doc/$${tCmd##*/}.md; \
		pod2html $(mHtmlOpt) $$tCmd >doc/user-doc/$${tCmd##*/}.html; \
	done

fmt : clean
	which shfmt
	which $(mTestDir)/rm-trailing-sp
	-rm fmt-err.tmp
	rm-trailing-sp doc/config/* doc/hooks/* git-core/* test/*.sh
	rm-trailing-sp doc/LICENSE doc/VERSION doc/README
	rm-trailing-sp Makefile README.md TODO.md
	for i in $$(grep -rl '^#!/bin/bash' *); do \
		echo $$i; \
		if ! bash -n $$i; then \
			echo "Error in $$i" >>fmt-err.tmp; \
			continue; \
		fi; \
		if ! 'shfmt' -i 4 -ci -fn <$$i >t.tmp; then \
			echo "Error in $$i" >>fmt-err.tmp; \
			continue; \
		fi; \
		if [ ! -s t.tmp ]; then \
			echo "Error in $$i" >>fmt-err.tmp; \
			continue; \
		fi; \
		mv -f t.tmp $$i; \
		chmod a+rx $$i; \
	done
	-rm t.tmp
	-cat fmt-err.tmp
