# gitproj/doc/test/Makefile

# TBD add missing tar file rules
# TBD add tar file deps to the tests scripts

mCore = ../git-core

# Note: the order matters
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

mCmdToDo = \
	add \
	move \
	rm \
	config \
	check

# --------------------
test : $(mDepList) $(mBaseList) $(mCmdList)

test-all : clear-logs test

clear-logs :
	rm test-*.log

stats :
	./stats.sh

# --------------------
test-com.log : $(mDepList) test-com.sh
	./test-com.sh 2>&1 | tee $@

test-com2.log : $(mDepList) test-com2.sh
	./test-com2.sh 2>&1 | tee $@

test-pre-commit.log : $(mDepList) test-pre-commit.sh ../doc/hooks/pre-commit test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz
	./test-pre-commit.sh 2>&1 | tee $@

test-gitproj.log : $(mDepList) test-gitproj.sh $(mCore)/git-proj
	./test-gitproj.sh 2>&1 | tee $@

# --------------------
test-init.sh : test-env_HomeAfterBMove.tgz test-env_ProjAfterGInit.tgz

test-remote.sh : test-env_ProjLocalDefined.tgz test-env_TestDestDirAfterMkRemote.tgz

test-push.sh : test-env_TestDestDirAfterRemoteReport.tgz test-env_ProjLocalDefined.tgz

test-pull.sh : test-env_ProjLocalDefined.tgz test-clone.sh :

test-status.sh : test-env_TestDestDirAfterCreateRemoteGit.tgz test-env_Home3AfterCloneSummary.tgz

# --------------------
test-%.log : test-%.sh $(mDepList) $(mCore)/gitproj-%.inc $(mCore)/git-proj-%
	./$< 2>&1 | tee $@

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
