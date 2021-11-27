# gitproj/doc/test/Makefile

mCore = ../../git-core

mDepList = \
	shunit2.1 \
	$(mCore)/gitproj-com.inc \
	test.inc \
	test-env.tgz \
	test-env_HomeAfterBMove.tgz \
	test-env_ProjAfterGInit.tgz \
	test-env_ProjLocalDefined.tgz

mBaseList = \
	test-com \
	test-gitproj \

mCmdList = \
	init \
	remote

mCmdToDo = \
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
test-all : $(mDepList) $(mBaseList) test-cmds

test-com : $(mDepList) test-com.sh
	./$@.sh

test-gitproj : $(mDepList) test-gitproj.sh $(mCore)/git-proj
	./$@.sh

test-cmds : $(mDepList)
	for tCmd in $(mCmdList); do \
		echo ./test-$${tCmd}.sh; \
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
	export gpSaveTestEnv=1; ./test-init.sh testInitMoveBinaryFiles

test-env_ProjAfterGInit.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitMkGitDir

test-env_ProjLocalDefined.tgz :
	export gpSaveTestEnv=1; ./test-init.sh testInitCreateLocalGitAuto

# --------------------
stats :
	@echo "Tests"
	@echo -n "Number of tests: "
	@grep '^test' test-*.sh |grep '()' | wc -l
	@echo -n "Number of asserts: "
	@grep assert *.sh | wc -l
	@echo -n "Total lines of test code: "
	@cat \
		Makefile *.md *.sh *.inc \
	| wc -l
	@echo -n "Lines of doc. in test code: "
	@awk '/=pod/,/=cut/ {print /bin/bash}; /=internal-pod/,/=internal-cut/ {print /bin/bash}' * | wc -l
	@echo
	@echo "Code"
	@echo -n "Total lines of code: "
	@cat \
		../VERSION \
		../config/* ../hooks/* \
		../../git-core/* \
	| wc -l
	@echo -n "Lines of doc. in code: "
	@awk '/=pod/,/=cut/ {print /bin/bash}; /=internal-pod/,/=internal-cut/ {print /bin/bash}' ../../git-core/* | wc -l
	@echo
	@echo -n "Total for all: "
	@cat \
		Makefile *.md *.sh *.inc \
		../VERSION \
		../config/* ../hooks/* \
		../../git-core/* \
	| wc -l
	@echo -n "Total lines of doc: "
	@awk '/=pod/,/=cut/ {print /bin/bash}; /=internal-pod/,/=internal-cut/ {print /bin/bash}' * ../../git-core/* | wc -l
