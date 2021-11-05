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

test : $(mDepList) $(mBaseList) test-cmds

test-com : $(mDepList) test-com.sh
	./$@.sh

test-gitproj : $(mDepList) test-gitproj.sh $(mCore)/git-proj
	./$@.sh

test-cmds : $(mDepList)
	for tCmd in $(mCmdList); do \
		./test-$${tCmd}.sh; \
	done
