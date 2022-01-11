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
mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

# --------------------
check : doc git-core package test

clean : check
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

gen-doc : doc/CHANGES.md doc/README.md doc/user-doc/git-proj.md doc/user-doc/git-proj.html
	-for tCmd in git-core/git-proj-*; do \
		pod2markdown $$tCmd >doc/user-doc/$${tCmd##*/}.md; \
		pod2html $(mHtmlOpt) $$tCmd >doc/user-doc/$${tCmd##*/}.html; \
		$(mTidy) doc/user-doc/$${tCmd##*/}.html 2>/dev/null; \
	done

doc/CHANGES.md : CHANGES.md
	grep -Ev 'INT:' <$? | uniq >$@

doc/README.md : README.md
	grep -Ev 'INT:' <$? | uniq | awk ' \
	    /^## For Developers/ { exit 0 } \
	    { print $$0 } \
	' >$@

doc/user-doc/git-proj.md : git-core/git-proj
	-mkdir doc/user-doc 2>/dev/null
	-$? -H md >$@

doc/user-doc/git-proj.html : git-core/git-proj
	-mkdir doc/user-doc 2>/dev/null
	-$? -H md >$@

# Remove internal doc. Any line with 'INT:' in it.
# "uniq" is a quick way of removing any extra blank lines

fmt : clean
	which shfmt
	which $(mTestDir)/rm-trailing-sp
	-rm fmt-err.tmp
	rm-trailing-sp doc/config/* doc/hooks/* git-core/* test/*.sh
	rm-trailing-sp doc/LICENSE doc/VERSION
	rm-trailing-sp Makefile README.md TODO.md CHANGES.md
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
