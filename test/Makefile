# gitproj/doc/test/Makefile

# Run tests for any dependencies that have changed:
# 	make test

# Force a run of all tests, stop on any test group error
# 	make test-all

# Force a run of all tests, and keep going, even if errors
# 	make -k test-all


# --------------------
# Config

SHELL = /bin/bash

mDepPkg = \
	git \
	bash \
	coreutils \
	gawk \
	git-flow \
	libpod-markdown-perl \
	markdown \
	openssh-client \
	perl \
	rsync \
	sed \
	tidy

mEpmPkg = epm-5.0.2-1-ubuntu18-x86_64.deb
mEpmHelperPkg = epm-helper-1.5.1-3-linux-noarch.deb

mHtmlOpt = --cachedir=/tmp --index --backlink
mRsyncOpt = -rlptzLP

mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

mCore = ../git-core

mHost = moria.whyayh.com
mReleaseUrl = $(mHost):/rel/released/software/ThirdParty
mTestUrl = $(mHost):/rel/development/software/own/git-proj/test
mTestOpt = --user=guest --password=guest

mTestEnv = ../..

# Note: the order matters, when *recreating* these files.
# The order of tests do not matter when *using* thise files.
mTestEnvFiles = \
	$(mTestEnv)/test-env_HomeAfterBMove.tgz \
	$(mTestEnv)/test-env_ProjAfterGInit.tgz \
	$(mTestEnv)/test-env_ProjLocalDefined.tgz \
	$(mTestEnv)/test-env_TestDestDirAfterMkRemote.tgz \
	$(mTestEnv)/test-env_TestDestDirAfterRemoteReport.tgz \
	$(mTestEnv)/test-env_Home2AfterPush.tgz \
	$(mTestEnv)/test-env_TestDestDirAfterCreateRemoteGit.tgz \
	$(mTestEnv)/test-env_Home3AfterCloneMkGit.tgz \
	$(mTestEnv)/test-env_Home3AfterCloneSummary.tgz

mDepList = \
	shunit2.1 \
	$(mCore)/gitproj-com.inc \
	test.inc \
	$(mTestEnv)/test-env.tgz \
	$(mTestEnv)/saved-path.tgz

mBaseList = \
	.ran-test-infra.tmp \
	.ran-test-com.tmp \
	.ran-test-com2.tmp \
	.ran-test-pre-commit.tmp

mCmdList = \
	.ran-test-gitproj.tmp \
	.ran-test-init.tmp \
	.ran-test-remote.tmp \
	.ran-test-push.tmp \
	.ran-test-pull.tmp \
	.ran-test-clone.tmp \
	.ran-test-status.tmp \
	.ran-test-config.tmp

# The might be implemented later
mCmdToDo = \
	add \
	move \
	rm \
	config \
	check

# --------------------
clean :
	-find * -name '*~' -exec rm -f {} \;
	-rm $(mEpmPkg) $(mEpmHelperPkg)

dist-clean : clear-ran

# --------------------
install-deps : epm epm-helper
	sudo apt-get update
	sudo apt-get install -y $(mDepPkg)

epm :
	if ! dpkg -l $@; then \
		wget $(mTestOpt) https://$(mReleaseUrl)/epm/ubuntu18/$(mEpmPkg); \
		sudo apt-get install -y ./$(mEpmPkg); \
	fi

epm-helper :
	if ! dpkg -l $@; then \
		wget $(mTestOpt) https://$(mReleaseUrl)/epm/$(mEpmHelperPkg); \
		sudo apt-get install -y ./$(mEpmHelperPkg); \
	fi

mk-ssh : util/ssh/id.hamp util/ssh/config
	-mkdir ~/.ssh
	cp -f util/ssh/id.hamp  ~/.ssh
	touch ~/.ssh/config; \
	if ! grep 'Host moria.whyayh.com' ~/.ssh/config; then \
		cat util/ssh/config >>~/.ssh/config; \
	fi
	chmod -R go= ~/.ssh

# --------------------
# Test

# Only run tests where the dependencies have changed
test : util/rm-trailing-sp util/bash-fmt util/bash-lint $(mDepList) $(mBaseList) $(mCmdList)

# Use this for *force* all tests to be run
test-all : util/rm-trailing-sp util/bash-fmt util/bash-lint clear-ran test

clear-ran :
	-rm .ran-test-*.tmp

# Get project statistics
stats :
	./stats.sh

# ----------------------------------------
gen-doc : dev-doc/README.html dev-doc/git-proj.md dev-doc/git-proj.html dev-doc/outline.html dev-doc/coding-convention.html dev-doc/release-server.html

dev-doc/README.html : dev-doc/README.pod
	-pod2html --title='README for Developers' $(mHtmlOpt) <$? >$@
	-$(mTidy) $@
	-pod2markdown <dev-doc/README.pod >dev-doc/README.md

dev-doc/git-proj.md : $(mCore)/git-proj $(mCore)/gitproj-com.inc
	-$(mCore)/git-proj -H int-md >$@

dev-doc/git-proj.html : $(mCore)/git-proj $(mCore)/gitproj-com.inc
	-$(mCore)/git-proj -H int-html >$@

dev-doc/outline.html : dev-doc/outline.pod
	-pod2html --title='Outline of Files' $(mHtmlOpt) <$? >$@
	-$(mTidy) $@
	-pod2markdown <dev-doc/outline.pod >dev-doc/outline.md

dev-doc/coding-convention.html : dev-doc/coding-convention.pod
	-pod2html --title='Coding Convention' $(mHtmlOpt) <$? >$@
	-$(mTidy) $@
	-pod2markdown <dev-doc/coding-convention.pod >dev-doc/coding-convention.md

dev-doc/release-server.html : dev-doc/release-server.pod
	-pod2html --title='GNU Rush Configuration' $(mHtmlOpt) <$? >$@
	-$(mTidy) $@
	-pod2markdown <dev-doc/release-server.pod >dev-doc/release-server.md

# rm-trailing-sp :
##util/rm-trailing-sp : ~/bin/rm-trailing-sp
##	diff $@ $?

# bash-fmt :
##util/bash-fmt : ~/bin/bash-fmt
##	diff $@ $?

# bash-lint :
##util/bash-lint : ~/bin/bash-lint
##	diff $@ $?

# ========================================
# Tests for each of the commands

# --------------------
# Special non-CMD tests

.ran-test-infra.tmp : $(mDepList) test-infra.sh
	./test-infra.sh 2>&1
	touch $@

.ran-test-com.tmp : $(mDepList) test-com.sh
	./test-com.sh 2>&1
	touch $@

.ran-test-com2.tmp : $(mDepList) test-com2.sh $(mTestEnv)/test-env_ProjLocalDefined.tgz
	./test-com2.sh 2>&1
	touch $@

.ran-test-pre-commit.tmp : $(mDepList) test-pre-commit.sh ../doc/hooks/pre-commit $(mTestEnv)/test-env_TestDestDirAfterCreateRemoteGit.tgz $(mTestEnv)/test-env_Home3AfterCloneSummary.tgz
	./test-pre-commit.sh 2>&1
	touch $@

.ran-test-gitproj.tmp : $(mDepList) test-gitproj.sh $(mCore)/git-proj
	./test-gitproj.sh 2>&1
	touch $@

# --------------------
# If a CMD .ran file is old, updated it by running its test script

.ran-test-%.tmp : test-%.sh $(mDepList) $(mCore)/gitproj-%.inc $(mCore)/git-proj-%
	./$< 2>&1
	touch $@

# --------------------
# Define the test-env*.tgz dependencies here

test-clone.sh : $(mTestEnv)/test-env_TestDestDirAfterCreateRemoteGit.tgz $(mTestEnv)/test-env_Home3AfterCloneMkGit.tgz

test-com2.sh : $(mTestEnv)/test-env_ProjLocalDefined.tgz

test-init.sh : $(mTestEnv)/test-env_HomeAfterBMove.tgz $(mTestEnv)/test-env_ProjAfterGInit.tgz

test-pre-commit.sh : $(mTestEnv)/test-env_TestDestDirAfterCreateRemoteGit.tgz $(mTestEnv)/test-env_Home3AfterCloneSummary.tgz

test-remote.sh : $(mTestEnv)/test-env_ProjLocalDefined.tgz $(mTestEnv)/test-env_TestDestDirAfterMkRemote.tgz

test-pull.sh : $(mTestEnv)/test-env_ProjLocalDefined.tgz $(mTestEnv)/test-env_Home2AfterPush.tgz

test-push.sh : $(mTestEnv)/test-env_TestDestDirAfterRemoteReport.tgz $(mTestEnv)/test-env_ProjLocalDefined.tgz

test-remore.sh : $(mTestEnv)/test-env_ProjLocalDefined.tgz $(mTestEnv)/test-env_TestDestDirAfterMkRemote.tgz

test-status.sh : $(mTestEnv)/test-env_TestDestDirAfterCreateRemoteGit.tgz $(mTestEnv)/test-env_Home3AfterCloneSummary.tgz

test-config.sh : $(mTestEnv)/test-env_ProjLocalDefined.tgz $(mTestEnv)/test-env_Home2AfterPush.tgz

# --------------------
$(mTestEnv)/test-env.tgz :
	wget $(mTestOpt) https://$(mTestUrl)/$@
	sleep 0.5
	@echo "$@ needs to be manually updated."
	@echo "Do not remove: $@"
	@echo "First run:  ./test-init.sh testGitProjInit"
	@echo "Make manual changes to the env area: ../../test/"
	@echo "cd ../../test"
	@echo "tar -czf $(mTestEnv)/test-env.tgz test"
	@echo "Now you should update the other test-env tar files."

# --------------------
# Define the tests that create the test-env*.tgz files.
# New files are manually moved and symlinks added.

$(mTestEnv)/saved-path.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		cd ../..; \
		mkdir test; \
		echo "export cSavedPath=$$PWD/test" >test/saved-path.inc; \
		chmod a+rx test/saved-path.inc; \
		tar -czf $(mTestEnv)/saved-path.tgz test/saved-path.inc; \
	fi

test-env : $(mTestEnvFiles)
	@echo "To update the test-env files:"
	@echo "$^"
	@echo 'Move the above files out of the way (not the symlinks)'
	@echo 'export gpSaveTestEnv=1'
	@echo 'Then run: make test-env'

$(mTestEnv)/test-env_HomeAfterBMove.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
	        wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh testInitMoveBinaryFiles; \
	fi

$(mTestEnv)/test-env_ProjAfterGInit.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
	        wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh testInitMkGitDir; \
	fi

$(mTestEnv)/test-env_ProjLocalDefined.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh testInitCreateLocalGitAuto; \
	fi

$(mTestEnv)/test-env_TestDestDirAfterMkRemote.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-remote.sh testRemoteMkRemote; \
	fi

$(mTestEnv)/test-env_TestDestDirAfterRemoteReport.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-remote.sh testRemoteReport; \
	fi

$(mTestEnv)/test-env_TestDestDirAfterCreateRemoteGit.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-remote.sh testRemoteCreateRemoteGit; \
	fi

$(mTestEnv)/test-env_Home2AfterPush.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-push.sh testGitProjPushCLI; \
	fi

$(mTestEnv)/test-env_Home3AfterCloneMkGit.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-clone.sh testCloneMkGitDirPass; \
	fi

$(mTestEnv)/test-env_Home3AfterCloneSummary.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-clone.sh testCloneSummary; \
	fi

# --------------------
# Update test-env*.tgz files at $(mTestUrl)

upload-tgz-files :
	for i in $(mTestEnv)/saved-path.tgz $(mTestEnvFiles); do \
		'rsync' $(mRsyncOpt) $$i $(mTestUrl); \
		if [ -z $$? ]; then \
			exit 1; \
		fi; \
		sleep 7; \
	done

	# sleep required because there is a rate limit that only allows
        # 6 ssh connections allowed every 30 sec (from the same IP).
