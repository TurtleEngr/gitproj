# gitproj/doc/test/Makefile

# TBD add missing tar file rules
# TBD add tar file deps to the tests scripts

# --------------------
# Config

mHtmlOpt = --cachedir=/tmp --index --backlink

mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

mCore = ../git-core

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
	test-env.tgz

mBaseList = \
	test-com.log \
	test-com2.log \
	test-pre-commit.log

mCmdList = \
	test-gitproj.log \
	test-init.log \
	test-remote.log \
	test-push.log \
	test-pull.log \
	test-clone.log \
	test-status.log

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

dist-clean : clear-logs

# --------------------
# Test

# Only run tests where the dependencies have changed
test : rm-trailing-sp bash-fmt bash-lint $(mDepList) $(mBaseList) $(mCmdList)

# Use this for *force* all tests to be run
test-all : rm-trailing-sp bash-fmt bash-lint clear-logs test

clear-logs :
	-rm test-*.log

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
rm-trailing-sp : ~/bin/rm-trailing-sp
	diff $@ $?

# bash-fmt :
bash-fmt : ~/bin/bash-fmt
	diff $@ $?

# bash-lint :
bash-lint : ~/bin/bash-lint
	diff $@ $?

# ========================================
# Tests for each of the commands

# --------------------
# Special non-CMD tests

test-com.log : $(mDepList) test-com.sh
	./test-com.sh 2>&1 | tee $@

test-com2.log : $(mDepList) test-com2.sh
	./test-com2.sh 2>&1 | tee $@

test-pre-commit.log : $(mDepList) test-pre-commit.sh ../doc/hooks/pre-commit test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz
	./test-pre-commit.sh 2>&1 | tee $@

test-gitproj.log : $(mDepList) test-gitproj.sh $(mCore)/git-proj
	./test-gitproj.sh 2>&1 | tee $@

# --------------------
# If a CMD log file is old, updated it by running its test script

test-%.log : test-%.sh $(mDepList) $(mCore)/gitproj-%.inc $(mCore)/git-proj-%
	./$< 2>&1 | tee $@

# --------------------
# Define the test-env*.tgz dependencies here

test-init.sh : test-env_HomeAfterBMove.tgz test-env_ProjAfterGInit.tgz

test-remote.sh : test-env_ProjLocalDefined.tgz test-env_TestDestDirAfterMkRemote.tgz

test-push.sh : test-env_TestDestDirAfterRemoteReport.tgz test-env_ProjLocalDefined.tgz

test-pull.sh : test-env_ProjLocalDefined.tgz

test-status.sh : test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz

# --------------------
test-env.tgz :
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

test-env : $(mTestEnvFiles)
	@echo "To update the test-env files:"
	@echo "$^"
	@echo "move them out of the way, then run this make target again"

test-env_HomeAfterBMove.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitMoveBinaryFiles

test-env_ProjAfterGInit.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitMkGitDir

test-env_ProjLocalDefined.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitCreateLocalGitAuto

test-env_TestDestDirAfterMkRemote.tgz :
	export gpSaveTestEnv=1; ./test-remote.sh testRemoteMkRemote

test-env_TestDestDirAfterRemoteReport.tgz :
	export gpSaveTestEnv=1; ./test-remote.sh testRemoteReport

test-env_TestDestDirAfterCreateRemoteGit.tgz :
	export gpSaveTestEnv=1; ./test-remote.sh testRemoteCreateRemoteGit

test-env_Home2AfterPush.tgz :
	export gpSaveTestEnv=1; ./test-push.sh testGitProjPushCLI

test-env_Home3AfterCloneMkGit.tgz :
	export gpSaveTestEnv=1; ./test-clone.sh testCloneMkGitDirPass

test-env_Home3AfterCloneSummary.tgz :
	export gpSaveTestEnv=1; ./test-clone.sh testCloneSummary
