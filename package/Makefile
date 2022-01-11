# Directions

# Run this first to check for dependencies and to setup meta-date for packaging
# make first

# Collect the files for the package into pseudo root dist/
# make build

# Use this to start with a fresh build (build always runs this)
# make clean

# Use this to clean up all unversioned files (after making and releasing
# packages)
# make dist-clean

# Create the native and portable packages.
# make test-package

# Push the packages to a test "release" server and/or apt repository
# make test-release

# Remove all but the last test package
# make clean-test-release

# Create the native and portable packages
# Production packages versions use the ProdBuild var
# make package

# Push the packages to a "release" server and/or apt repository
# make release

# Remove all gitproj RC packages
# make clean-release

# --------------------
include ver.mak

# --------------------
# Config

# --------------------
# Main targets

first : ver.mak ver.epm /usr/lib/git-core
	touch ver.sh
	make ver.mak
	@which git
	@which epm
	@which mkver.pl
	@which mkepmlist
	@which patch-epm-list
	@which perl
	@which gawk
	@which fmt
	@which sed
	@which tr
	@which pod2markdown

build epm.list : clean ver.mak ver.epm ver.env
	if [ "$(USER)" = "root" ]; then exit 1; fi
	-rm -rf dist tmp >/dev/null 2>&1
	cd ..; make gen-doc
	mkdir -p dist/usr/lib/git-core
	mkdir -p dist/usr/share/doc/git-proj
	mkdir -p dist/usr/share/man/man1
	echo $(ProdVer) >../doc/VERSION
	cp ../git-core/* dist/usr/lib/git-core
	rsync -a ../doc/* dist/usr/share/doc/git-proj
	pod2man ../git-core/git-proj >dist/usr/share/man/man1/git-proj.1
	gzip dist/usr/share/man/man1/git-proj.1
	ln dist/usr/share/man/man1/git-proj.1.gz dist/usr/share/man/man1/gitproj.1.gz
	chmod -R a+r dist
	find dist -executable -exec chmod a+rx {} \;
	mkepmlist -u root -g root --prefix / dist | patch-epm-list -f ./epm.patch >epm.list

install :
	if [ ! -f epm.list ]; then exit 1; fi
	if [ "$(USER)" != "root" ]; then exit 1; fi
	chown -R root:root dist/*
	rsync -n -aP dist/usr/lib/git-core/* /usr/lib/git-core/
	rsync -n -aP dist/usr/share/doc/git-proj /usr/share/doc
	rsync -n -aP dist/usr/share/man/man1/* /usr/share/man/man1/
	chown -R $(CurLogName):$(CurLogName) dist/*
	# First: make build
	# Only use this for testing.

uninstall :
	if [ "$(USER)" != "root" ]; then exit 1; fi
	rm /usr/lib/git-core/git-proj
	rm /usr/lib/git-core/git-proj-*
	rm /usr/lib/git-core/gitproj-*.inc
	rm -rf /usr/share/doc/git-proj

clean :
	-rm epm.list ver.cs ver.env ver.epm ver.h ver.java ver.pl ver.xml >/dev/null 2>&1
	-rm -rf dist tmp >/dev/null 2>&1
	-find * -name '*~' -exec rm {} \; >/dev/null 2>&1

dist-clean : clean
	-cd ../test; make clean
	-rm ver.mak
	-rm -rf pkg >/dev/null 2>&1

test-package : epm.list $(ProdOSDist).require
	if [ "$(USER)" = "root" ]; then exit 1; fi
	-rm -rf pkg ver.epm >/dev/null 2>&1
	mkdir pkg
	export RELEASE=0; mkver.pl -e epm
	epm -v -f native -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm

test-release : TBD

clean-test-release :
	# Remove all $(ProdName) packages from repo

package : epm.list $(ProdOSDist).require
	if [ "$(USER)" = "root" ]; then exit 1; fi
	-rm -rf pkg ver.epm >/dev/null 2>&1
	mkdir pkg
	export RELEASE=1; mkver.pl -e epm
	epm -v -f native -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm
	#epm -v -f portable -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm
	# Set ProdRC for Release Candidate packages

release : ver.env ver.mak
	+. ./ver.env; echo ssh $(ProdRelServer) mkdir -p $(ProdRelDir)
	-. ./ver.env; ssh $(ProdRelServer) mkdir -p $(ProdRelDir)
	. ./ver.env; ssh $(ProdRelServer) test -d $(ProdRelDir)
	. ./ver.env; rsync -zP pkg/$(ProdName)-* $(ProdRelServer):$(ProdRelDir)

tag : ver.env ver.mak
	-git commit -am Updated
	. ./ver.env; git tag -f -a -m "Released to: $(ProdRelServer):$(ProdRelDir)" $(ProdTag)
	echo "# --------------------" >>VERSION
	date -u +'%F %R UTC' >>VERSION
	. ./ver.env; echo "$(ProdTag)" >>VERSION
	. ./ver.env; echo "Released: $$(ls pkg)" >>VERSION
	. ./ver.env; echo "to: $(ProdRelServer):$(ProdRelDir)" >>VERSION
	#git commit -am Updated
	#git push origin develop
	#git push --tags -f

clean-rc-release :
	# Remove all $(ProdName) Release Candidates from repo

# --------------------
# Work Targets

ver.mak ver.epm ver.env : ver.sh
	if [ "$(USER)" = "root" ]; then exit 1; fi
	mkver.pl -e 'mak env epm'
