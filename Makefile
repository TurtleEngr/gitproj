
# --------------------
# Config

# --------------------
clean :
	-find . -name '*~' -exec rm {} \;
	-find . -name '*.tmp' -exec rm {} \;

test : clean
	cd test; make test-all

install :
	ll
	
build : test
	cd package; make build
	
package : build
	cd package: make package

release : package
	cd package; make release

# --------------------
fmt :
	+which shfmt
	-git commit -am "Before fmt"
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
