# gitproj/doc/test/Makefile

mCore = ../../git-core

mDepList = \
	shunit2.1 \
	test.inc \
	test-env.tgz \
	$(mCore)/gitproj-com.inc

mBaseList = \
	test-com \
	test-gitproj \

mCmdList = \
	init

mCmdToDo = \
	remote \
	clone \
	push \
	pull \
	add \
	move \
	rm \
	config \
	status \
	check

mTestEnvFiles = \
	test-env_HomeAfterBMove.tgz \
	test-env_ProjAfterGInit.tgz \
	test-env_ProjLocalDefined.tgz

# --------------------
test : $(mDepList) $(mBaseList) test-cmds

test-com : $(mDepList) test-com.sh
	./$@.sh

test-gitproj : $(mDepList) test-gitproj.sh $(mCore)/git-proj
	./$@.sh

test-cmds : $(mDepList)
	for tCmd in $(mCmdList); do \
		./test-$${tCmd}.sh; \
	done

# --------------------
test-env : $(mTestEnvFiles)
	@echo "To update the test-env files:"
	@echo "$^"
	@echo "move them out of the way, then run this make target again"

test-env.tgz :
	@echo "$@ needs to be manually updated."
	@echo "Do not remove: $@"
	@echo "First run:  ./test-init.sh testGitProjInit"
	@echo "Make manual changes to the env area: ../../../test/"
	@echo "cd ../../../test"
	@echo "tar -czf test-env.tgz test"
	@echo "mv -fv test-env.tgz gitproj/doc/test"
	@echo "Now you should update the other test-env tar files."

test-env_HomeAfterBMove.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitMoveBinaryFiles_Move

test-env_ProjAfterGInit.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitMkGitDir

test-env_ProjLocalDefined.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitCreateLocalGitAuto

# --------------------
stats :
	@echo -n "Number of tests: "
	@grep '^test' test-*.sh |grep '()' | wc -l
	@echo -n "Number of asserts: "
	@grep assert *.sh | wc -l
	@echo -n "Total lines of test code: "
	@cat \
		Makefile *.md *.sh *.inc \
	| wc -l
	@echo -n "Total lines of code: "
	@cat \
		../VERSION \
		../config/* ../hooks/* \
		../../git-core/* \
	| wc -l
	@echo -n "Total for all: "
	@cat \
		Makefile *.md *.sh *.inc \
		../VERSION \
		../config/* ../hooks/* \
		../../git-core/* \
	| wc -l
