# gitproj/doc/test/Makefile

# Run tests for any dependencies that have changed:
# 	make test

# Force a run all tests, stop on any test group error
# 	make test-all

# Force a run all tests, and keep going, even if erros
# 	make -k test-all


# --------------------
# Config

mHtmlOpt = --cachedir=/tmp --index --backlink
mRsyncOpt = -rlptzLP

mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

mCore = ../git-core

mTestUrl = moria.whyayh.com:/rel/development/software/own/git-proj/test
mTestOpt = --user=guest --password=guest

# Note: the order matters, when *recreating* these files.
# The order of tests do not matter when *using* thise files.
mTestEnvFiles = \
	test-env_HomeAfterBMove.tgz \
	test-env_ProjAfterGInit.tgz \
	test-env_ProjLocalDefined.tgz \
	test-env_TestDestDirAfterMkRemote.tgz \
	test-env_TestDestDirAfterRemoteReport.tgz \
	test-env_Home2AfterPush.tgz \
	test-env_TestDestDirAfterCreateRemoteGit.tgz \
	test-env_Home3AfterCloneMkGit.tgz \
	test-env_Home3AfterCloneSummary.tgz

mDepList = \
	shunit2.1 \
	$(mCore)/gitproj-com.inc \
	test.inc \
	test-env.tgz \
	saved-path.tgz

mBaseList = \
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
	.ran-test-status.tmp

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

dist-clean : clear-ran

# --------------------
# Test

# Only run tests where the dependencies have changed
test : rm-trailing-sp bash-fmt bash-lint $(mDepList) $(mBaseList) $(mCmdList)

# Use this for *force* all tests to be run
test-all : rm-trailing-sp bash-fmt bash-lint clear-ran test

clear-ran :
	-rm .ran-test-*.tmp

# Get project statistics
stats :
	./stats.sh

gen-dev-doc : dev-doc/git-proj.md dev-doc/git-proj.html dev-doc/outline.html dev-doc/coding-convention.html

dev-doc/git-proj.md : $(mCore)/git-proj
	-$? -H int-md >$@

dev-doc/git-proj.html : $(mCore)/git-proj
	-$? -H int-html >$@

dev-doc/outline.html : dev-doc/outline.pod
	-pod2html --title='Outline of Files' $(mHtmlOpt) <$? >$@
	-$(mTidy) $@
	-pod2markdown <dev-doc/outline.pod >dev-doc/outline.md

dev-doc/coding-convention.html : dev-doc/coding-convention.pod
	-pod2html --title='Coding Convention' $(mHtmlOpt) <$? >$@
	-$(mTidy) $@
	-pod2markdown <dev-doc/coding-convention.pod >dev-doc/coding-convention.md

# rm-trailing-sp :
##rm-trailing-sp : ~/bin/rm-trailing-sp
##	diff $@ $?

# bash-fmt :
##bash-fmt : ~/bin/bash-fmt
##	diff $@ $?

# bash-lint :
##bash-lint : ~/bin/bash-lint
##	diff $@ $?

# ========================================
# Tests for each of the commands

# --------------------
# Special non-CMD tests

.ran-test-com.tmp : $(mDepList) test-com.sh
	./test-com.sh 2>&1
	touch $@

.ran-test-com2.tmp : $(mDepList) test-com2.sh test-env_ProjLocalDefined.tgz
	./test-com2.sh 2>&1
	touch $@

.ran-test-pre-commit.tmp : $(mDepList) test-pre-commit.sh ../doc/hooks/pre-commit test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz
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

test-clone.sh : test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneMkGit.tgz

test-com2.sh : test-env_ProjLocalDefined.tgz

test-init.sh : test-env_HomeAfterBMove.tgz test-env_ProjAfterGInit.tgz

test-pre-commit.sh : test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz

test-remote.sh : test-env_ProjLocalDefined.tgz test-env_TestDestDirAfterMkRemote.tgz

test-pull.sh : test-env_ProjLocalDefined.tgz test-env_Home2AfterPush.tgz

test-push.sh : test-env_TestDestDirAfterRemoteReport.tgz test-env_ProjLocalDefined.tgz

test-remore.sh : test-env_ProjLocalDefined.tgz test-env_TestDestDirAfterMkRemote.tgz

test-status.sh : test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz

# --------------------
test-env.tgz :
	wget $(mTestOpt) https://$(mTestUrl)/$@
	sleep 0.5
	@echo "$@ needs to be manually updated."
	@echo "Do not remove: $@"
	@echo "First run:  ./test-init.sh testGitProjInit"
	@echo "Make manual changes to the env area: ../../test/"
	@echo "cd ../../test"
	@echo "tar -czf test-env.tgz test"
	@echo "mv -fv test-env.tgz gitproj/doc/test"
	@echo "Now you should update the other test-env tar files."

# --------------------
# Define the tests that create the test-env*.tgz files.
# New files are manually moved and symlinks added.

saved-path.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		cd ../..; \
		mkdir test; \
		echo "export cSavedPath=$$PWD/test" >test/saved-path.inc; \
		chmod a+rx test/saved-path.inc; \
		tar -czf saved-path.tgz test/saved-path.inc; \
		cd -; \
		ln -sf ../../saved-path.tgz .; \
	fi

test-env : $(mTestEnvFiles)
	@echo "To update the test-env files:"
	@echo "$^"
	@echo 'Move the above files out of the way (not the symlinks)'
	@echo 'export gpSaveTestEnv=1'
	@echo 'Then run: make test-env'

test-env_HomeAfterBMove.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
	        wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh testInitMoveBinaryFiles; \
	fi

test-env_ProjAfterGInit.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
	        wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh testInitMkGitDir; \
	fi

test-env_ProjLocalDefined.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh testInitCreateLocalGitAuto; \
	fi

test-env_TestDestDirAfterMkRemote.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-remote.sh testRemoteMkRemote; \
	fi

test-env_TestDestDirAfterRemoteReport.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-remote.sh testRemoteReport; \
	fi

test-env_TestDestDirAfterCreateRemoteGit.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh $@; \
		./test-remote.sh testRemoteCreateRemoteGit; \
	fi

test-env_Home2AfterPush.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh $@; \
		./test-push.sh testGitProjPushCLI; \
	fi

test-env_Home3AfterCloneMkGit.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh $@; \
		./test-clone.sh testCloneMkGitDirPass; \
	fi

test-env_Home3AfterCloneSummary.tgz :
	if [ -z "$$gpSaveTestEnv" ]; then \
		wget $(mTestOpt) https://$(mTestUrl)/$@; \
		sleep 0.5; \
	else \
		./test-init.sh $@; \
		./test-clone.sh testCloneSummary; \
	fi

# --------------------
# Update test-env*.tgz files at $(mTestUrl)

update-tgz-files :
	for i in saved-path.tgz $(mTestEnvFiles); do \
		'rsync' $(mRsyncOpt) $$i $(mTestUrl); \
		if [ -z $$? ]; then \
			exit 1; \
		fi; \
	done
