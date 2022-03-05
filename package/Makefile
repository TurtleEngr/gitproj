# Directions

# All the "meta-data" related to the gitproj product is put in the
# ver.sh file. The mkver.pl script will generate all the other ver.*
# files.  Notice the ../doc/VERSION file is updated from a variable
# that is in the ver.env file, which was generated from ver.sh. So the
# version number should ONLY be changed in ver.sh

# Run this first to check for dependencies and to setup meta-data for
# packaging (see ver.sh and mkver.sh)
# make first

# Collect the files for the package into pseudo root dist/ and create
# list of files for epm packager.
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
# Config

SHELL = /bin/bash
mRsyncOpt = -azP
mDoc=dist/usr/share/doc/git-proj

mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

# --------------------
include ver.mak

# ----------------------------------------
# Main targets

# --------------------
first ver.mak : /usr/lib/git-core ver.sh
	@echo '--------------------'
	@echo make first
	-mkver.pl -e 'mak env' 2>&1 | grep -v Warning:
	command -v git
	command -v epm
	command -v /usr/bin/markdown
	command -v mkver.pl
	command -v mkepmlist
	command -v patch-epm-list
	command -v perl
	command -v gawk
	command -v fmt
	command -v sed
	command -v tr
	command -v pod2markdown
	. ./ver.env; echo $$ProdVer >../doc/VERSION

# --------------------
# Build pseudo root of all files at dist/

build epm.list : build-prep build-core build-doc build-man build-contrib build-clean
	@echo '--------------------'
	@echo make epm.list
	mkepmlist -u root -g root --prefix / dist | patch-epm-list -f ./epm.patch >epm.list

build-prep : ver.mak ver.epm ver.env ../doc/VERSION
	@echo '--------------------'
	@echo make build-prep
	if [ "$(USER)" = "root" ]; then exit 1; fi
	-rm -rf dist

../doc/VERSION : ver.env
	. ./ver.env; echo $$ProdVer >../doc/VERSION

build-core :
	@echo '--------------------'
	@echo make build-core
	-mkdir -p dist/usr/lib/git-core
	cp ../git-core/* dist/usr/lib/git-core

build-doc :
	@echo '--------------------'
	@echo make build-doc
	-cd ..; make gen-doc
	-mkdir -p $(mDoc)
	'rsync' -a ../doc/* $(mDoc)
	cp ../LICENSE $(mDoc)

build-man :
	@echo '--------------------'
	@echo make build-man
	-mkdir -p dist/usr/share/man/man1
	pod2man ../git-core/git-proj >dist/usr/share/man/man1/git-proj.1
	gzip dist/usr/share/man/man1/git-proj.1
	ln dist/usr/share/man/man1/git-proj.1.gz dist/usr/share/man/man1/gitproj.1.gz

build-contrib :
	@echo '--------------------'
	@echo make build-contrib
	-mkdir -p $(mDoc)/contrib
	cp -p ../test/util/rm-trailing-sp ../test/util/bash-fmt \
	      ../test/util/bash-lint $(mDoc)/contrib

build-clean :
	@echo '--------------------'
	@echo make build-clean
	-find dist -type f -name '*~' -exec rm {} \;
	-find dist -type f -name '*.tmp' -exec rm {} \;
	-find dist -type f -name '*.sav' -exec rm {} \;
	-find dist -type f -name '*.bak*' -exec rm {} \;
	chmod -R a+r dist
	find dist -executable -exec chmod a+rx {} \;

# --------------------
# Non-package install - only for testing

install :
	@echo '--------------------'
	@echo make install
	if [ ! -f epm.list ]; then exit 1; fi
	if [ "$(USER)" != "root" ]; then exit 1; fi
	chown -R root:root dist/*
	'rsync' -n $(mRsyncOpt) dist/usr/lib/git-core/* /usr/lib/git-core/
	'rsync' -n $(mRsyncOpt) $(mDoc) /usr/share/doc
	'rsync' -n $(mRsyncOpt) dist/usr/share/man/man1/* /usr/share/man/man1/
	chown -R $(CurLogName):$(CurLogName) dist/*
	# First: make build
	# Only use this for testing.

uninstall :
	@echo '--------------------'
	@echo make uninstall
	if [ "$(USER)" != "root" ]; then exit 1; fi
	rm /usr/lib/git-core/git-proj
	rm /usr/lib/git-core/git-proj-*
	rm /usr/lib/git-core/gitproj-*.inc
	rm -rf /usr/share/doc/git-proj

# --------------------
# Partial clean before a new build

clean :
	@echo '--------------------'
	@echo make clean
	-rm epm.list ver.cs ver.env ver.epm ver.h ver.java ver.pl ver.xml >/dev/null 2>&1
	-rm -rf dist tmp >/dev/null 2>&1
	-find .. -name '*~' -exec rm {} \; >/dev/null 2>&1
	-find ../doc ../git-core -type f -name '*.tmp' -exec rm {} \;
	-find ../doc ../git-core -type f -name '*.sav' -exec rm {} \;
	-find ../doc ../git-core -type f -name '*.bak*' -exec rm {} \;

# Clean up all generated files (that are not versioned)
dist-clean : clean
	@echo '--------------------'
	@echo make dist-clean
	-cd ../test; make dist-clean
	-rm -rf pkg >/dev/null 2>&1

archive : dist-clean
	@echo '--------------------'
	@echo make archive
	cd ../..; tar -czf gitproj-$$(cat gitproj/doc/VERSION).tgz --exclude=.git --exclude-vcs gitproj test-env*.tgz

# --------------------
# This section is for automated build/release

test-package : epm.list $(ProdOSDist).require
	@echo '--------------------'
	@echo make test-package
	if [ "$(USER)" = "root" ]; then exit 1; fi
	-rm -rf pkg ver.epm ver.env >/dev/null 2>&1
	-mkdir pkg
	export RELEASE=0; mkver.pl -e 'epm env'
	@echo '--------------------'
	cat ver.epm
	@echo '--------------------'
	epm -v -f native -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm
	epm -v -f portable -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm

test-init : ver.env
	@echo '--------------------'
	@echo make test-init
	-. ./ver.env; ssh $(ProdRelServer) mkdir -p $(ProdDevDir)
	. ./ver.env; ssh $(ProdRelServer) test -d $(ProdDevDir)
	# Run this once before automated test-release

test-release : ver.env
	@echo '--------------------'
	@echo make test-release
	@echo '--------------------'
	cat ver.env
	@echo '--------------------'
	. ./ver.env; scp -v -i ~/.ssh/id.hamp pkg/$(ProdName)-*.deb hamp@$(ProdRelServer):/$(ProdRelCategory)
	. ./ver.env; scp -v -i ~/.ssh/id.hamp pkg/$(ProdName)-*.tar.gz hamp@$(ProdRelServer):/$(ProdRelCategory)

# --------------------
# Use this to make release candidates or final builds

package : epm.list $(ProdOSDist).require
	@echo '--------------------'
	@echo make package
	if [ "$(USER)" = "root" ]; then exit 1; fi
	-rm -rf pkg ver.epm ver.env >/dev/null 2>&1
	-mkdir pkg
	export RELEASE=1; mkver.pl -e 'epm env'
	##. ./ver.env; echo "%release rc$$ProdBuild.$$ProdBuildTime" >>ver.epm
	@echo '--------------------'
	cat ver.epm
	@echo '--------------------'
	epm -v -f native -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm
	epm -v -f portable -m $(ProdOSDist)-$(ProdArch) --output-dir pkg $(ProdName) ver.epm
	# Set ProdRC for Release Candidate packages
	# Has CHANGES.md been updated?
	# If stable release, merge develop to main branch

release : ver.env
	@echo '--------------------'
	@echo make release
	@echo '--------------------'
	cat ver.env
	@echo '--------------------'
	-. ./ver.env; ssh $(ProdRelServer) mkdir -p $(ProdRelDir)
	. ./ver.env; ssh $(ProdRelServer) test -d $(ProdRelDir)
	. ./ver.env; 'rsync' $(mRsyncOpt) pkg/$(ProdName)-* $(ProdRelServer):$(ProdRelDir)

# --------------------
tag : ver.mak
	@echo '--------------------'
	@echo make tag
	-git commit -am Updated
	git tag -f -a -m "Released to: $(ProdRelServer):$(ProdRelDir)" $(ProdTag)
	echo "# --------------------" >>REL-VERSION
	date -u +'%F %R UTC' >>REL-VERSION
	echo "$(ProdTag)" >>REL-VERSION
	echo "Released: $$(ls pkg)" >>REL-VERSION
	echo "to: $(ProdRelServer):$(ProdRelDir)" >>REL-VERSION
	#git commit -am Updated
	#git push origin develop
	#git push --tags -f
	#put tag in CHANGES.md and add 'git log' messages

# --------------------
# Work Targets

ver.epm ver.env : ver.sh
	if [ "$(USER)" = "root" ]; then exit 1; fi
	mkver.pl -e 'env epm'

# ver.mak : ver.sh
#	if [ "$(USER)" = "root" ]; then exit 1; fi
#	mkver.pl -e 'mak'
