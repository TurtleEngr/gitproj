# Makefile

# --------------------
# Config

mTestDir=$(PWD)/test
mBinDir=$(PWD)/git-core
mDocDir=$(PWD)/doc
mGenDir=$(PWD)/generate
mDistDir=$(PWD)/dist

# --------------------
check : $(mTestDir) $(mBinDir) $(mDocDir)

clean : check
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

dist-clean : clean
	-find $(mGenDir) $(mDistDir) -type l -exec rm {} \;
	-rm -rf $(mGenDir) $(mDistDir)

test : clean
	cd test; make test-all

build : clean test fmt
	cd package; make build

install : build
	cd package; make install

package : build
	cd package: make package

release : package
	cd package; make release

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
