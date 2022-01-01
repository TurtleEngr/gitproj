
# --------------------
# Config

# --------------------
clean :
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

test : clean
	cd test; make test-all

build : test fmt
	cd package; make build

install :
	cd package; make install

package : build
	cd package: make package

release : package
	cd package; make release

# --------------------
mkdocs :
	-mkdir -p man/man1
	git-core/git-proj -H man >man/man1/gitproj.1
	gzip man/man1/gitproj.1
	git-core/git-proj -H text >doc/gitproj.html
	# Generate internal docs at devel/
	-mkdir -p devel/
	#gitproj-com.inc

fmt :
	+which shfmt
	-git commit -am "Before fmt"
	-rm fmt-err.tmp
	rm-trailing-sp doc/config/* doc/hooks/* git-core/*
	rm-trailing-sp test/*.sh
	rm-trailing-sp doc/LICENSE doc/VERSION
	rm-trailing-sp Makefile  README.md TODO.md
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
