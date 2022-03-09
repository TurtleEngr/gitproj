# Makefile

# --------------------
# To run all tests:
#	cd test
#	make test-all

# Local install (only for testing; use package intaller):
#	cd package
#	make dist-clean
#	make first
#	make clean build install

# To build installer packages:
#	cd package
#	make dist-clean
#	make first
#	make clean build package
#	make release tag

# --------------------
# Config

SHELL = /bin/bash

mHtmlOpt = --cachedir=/tmp --index --backlink --podpath=.

mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

mTestDir = test

# --------------------
check : clean doc/user-doc git-core package test/dev-doc

clean :
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

# --------------------
gen-doc : top-doc cmd-doc user-doc
	-cd src-doc; make gen-doc
	-cd test; make gen-doc

# ----------
top-doc : doc/README.html doc/CHANGES.html

doc/README.html : README.md
	-grep -Ev 'INT:' <README.md | uniq >doc/README.md
	/usr/bin/markdown doc/README.md >doc/README.html
	-$(mTidy) doc/README.html

doc/CHANGES.html : CHANGES.md
	-grep -Ev 'INT:' <CHANGES.md | uniq >doc/CHANGES.md
	/usr/bin/markdown doc/CHANGES.md >doc/CHANGES.html
	-$(mTidy) doc/CHANGES.html

# ----------
cmd-doc :
	-mkdir -p doc/user-doc/subcommands
	-for tCmd in git-core/git-proj-*; do \
		pod2markdown $$tCmd >doc/user-doc/subcommands/$${tCmd##*/}.md; \
		pod2html $(mHtmlOpt) $$tCmd >doc/user-doc/subcommands/$${tCmd##*/}.html; \
		$(mTidy) doc/user-doc/$${tCmd##*/}.html 2>/dev/null; \
	done

# ----------
user-doc : doc/user-doc/git-proj.md doc/user-doc/git-proj.html

# git-proj will collect ALL usage help, and format as markdown
doc/user-doc/git-proj.md : git-core/git-proj
	-mkdir doc/user-doc 2>/dev/null
	-$? -H md >$@
	-test/util/fix-rel-links.sh doc/user-doc/git-proj.md

# git-proj will collect ALL usage help, and format as html, with a TOC
doc/user-doc/git-proj.html : git-core/git-proj
	-mkdir doc/user-doc 2>/dev/null
	-$? -H html >$@
	-test/util/fix-rel-links.sh doc/user-doc/git-proj.html

# --------------------
# Remove trailing whitespace, convert tabs to spaces, and normalized
# indents in bash fiies.
fmt : clean
	command -v shfmt
	command -v $(mTestDir)/rm-trailing-sp
	-rm fmt-err.tmp
	$(mTestDir)/rm-trailing-sp -t doc/config/* doc/hooks/* git-core/* test/*.sh
	$(mTestDir)/rm-trailing-sp -t doc/LICENSE doc/VERSION
	$(mTestDir)/rm-trailing-sp -t README.md TODO.md CHANGES.md
	$(mTestDir)/rm-trailing-sp Makefile
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
