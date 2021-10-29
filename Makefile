
# --------------------
# Config

sh-files = \
	git-core/* \
	doc/hooks/*

# --------------------
clean :
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

test : clean
	cd git-core; ./gitproj-com.test

build : test
	-rm -rf dist
	mkdir -p dist/usr/lib/git-core
	rsync -a git-core/* dist/usr/lib/git-core/
	mkdir -p dist/usr/share/doc/git-proj
	rsync -a doc/* dist/usr/share/doc/git-proj/
	rsync -a ../test-gitproj.tgz dist/usr/share/doc/git-proj/test

package : build

# --------------------
fmt :
	+which shfmt
	git commit -am "Before fmt"
	-rm fmt-err.tmp
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
