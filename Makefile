
# --------------------
# Config

release-files = \
	git-proj \
	git-proj-add \
	git-proj-clone \
	git-proj-init \
	git-proj-pull \
	git-proj-push \
	git-proj-set \
	git-proj-status \
	gitproj-com.inc

sh-files = \
	$(release-files) \
	hooks/* \
	gitproj-com.test

# --------------------
test : 
	./gitproj-com.test

build :
	-rm -rf dist
	mkdir -p dist/usr/lib/git-core
	cp $(release-files) dist/usr/lib/git-core
	mkdir -p dist/usr/share/doc/git-proj/hooks

package :

# --------------------
fmt :
	+which shfmt
	for i in $(sh-files); do \
		echo $$i; \
		'shfmt' -i 4 -ci -fn <$$i >$$i.tmp; \
		if [ $? -ne 0 ]; then \
			continue; \
		fi; \
		mv -f $$i.tmp $$i; \
		chmod a+rx $$i; \
	done
