
# --------------------
# Config

release-files = \
	git-proj \
	git-proj-add \
	git-proj-clone \
	git-proj-config \
	git-proj-init \
	git-proj-pull \
	git-proj-push \
	git-proj-status \
	gitproj-com.inc

doc-files = \
	LICENSE \
	README.md \
	global.gitproj.config \
	local.getproj.config \
	hooks/* \
	gitproj-com.test \
	shunit2.1

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
	mkdir -p dist/usr/share/doc/git-proj
	rsync -a $(doc-files) dist/usr/share/doc/git-proj/

package :

# --------------------
fmt :
	+which shfmt
	for i in $(sh-files); do \
		echo $$i; \
		'shfmt' -i 4 -ci -fn <$$i >$$i.tmp; \
		if [ $$? -ne 0 ]; then \
			continue; \
		fi; \
		mv -f $$i.tmp $$i; \
		chmod a+rx $$i; \
	done
