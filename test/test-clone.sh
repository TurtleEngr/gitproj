#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-clone.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc()
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-com.inc

test-com.inc - Common functions used in the test scripts

=head1 SYNOPSIS

    ./test-clone.sh [all] [test,test,...]

=head1 DESCRIPTION

shunit2.1 is used to run the unit tests. If no test function names are
listed, then all of the test functions will be run.

=head1 RETURN VALUE

0 - if OK

=head1 ERRORS

=head1 EXAMPLES

=head1 ENVIRONMENT

=head1 FILES

=head1 SEE ALSO

shunit2.1

=head1 NOTES

=head1 CAVEATS

=head1 DIAGNOSTICS

=head1 BUGS

=head1 RESTRICTIONS

=head1 AUTHOR

=head1 HISTORY

=cut

EOF
} # fUsage

# ========================================

# --------------------------------
NAoneTimeSetUp()
{
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-head2 Test gitproj-com.inc

=internal-head3 oneTimeSetuUp

Currently this records all of the script's expected initial global
variable settings, defined in fComSetGlobals. If different, adjust the
tests as needed.

Env Var

 HOME - this is set to the test user's home dir
 gpUnitDebug - this can be manually set to 1 in unit test functions.

Calls:

 $gpBin/gitproj-com.inc
 fComSetGlobals

=internal-cut
EOF
} # oneTimeSetUp

# --------------------------------
NAoneTimeTearDown()
{
    if [ $gpDebug -ne 0 ]; then
        fTestRmEnv
    fi
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
} # oneTimeTearDown

# --------------------------------
setUp()
{
    # Restore default global values, before each test

    unset cConfigGlobal cConfigLocal cCurDir cGetOrigin cGetTopDir \
        cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    fTestSetupEnv
    fTestCreateEnv
    . $gpBin/gitproj-clone.inc
    gpUnitDebug=0
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 setUp

Before each test runs, this restores all of the script's initial
global variable settings,

=internal-cut
EOF
} # setUp

# --------------------------------
tearDown()
{
    # fTestRmEnv
    gpUnitDebug=0
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
    cd $gpTest >/dev/null 2>&1
    return 0
} # tearDown

# ========================================

# --------------------------------
testGitProjClone()
{
    local tResult

    tResult=$($gpBin/git-proj-clone 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'Usage'

    tResult=$($gpBin/git-proj-clone -h)
    assertContains "$LINENO $tResult" "$tResult" 'DESCRIPTION'

    # git proj init [-l pDirPath] [-r] [-e pDirPath] [-h]

    cd $HOME/$cDatProj1 >/dev/null 2>&1
    assertFalse "$LINENO" "[ -d .git ]"
    cd - >/dev/null 2>&1
    return 0
} # testGitProjClone

# --------------------------------
testFirstTimeSet()
{

    # source gitproj-clone.inc calls fCloneSetGlobals, which calls
    # fCloneFirstTimeSet, which creates these files.
    #assertFalse "$LINENO" "[ -f $HOME/.gitconfig ]"
    #assertFalse "$LINENO" "[ -f $HOME/.gitproj.config.global ]"

    fCloneFirstTimeSet
    assertTrue "$LINENO" "[ -f $HOME/.gitconfig ]"
    assertTrue "$LINENO" "[ -f $HOME/.gitproj.config.global ]"
    assertTrue "$LINENO" "$(
        grep -q path $HOME/.gitconfig
        echo $?
    )"
    return 0
} # testFirstTimeSet

# --------------------------------
testCloneSetGlobals()
{
    assertTrue "$LINENO" "[ -d $HOME/$cDatProj1 ]"
    cd $HOME/$cDatProj1
    fCloneSetGlobals

    assertEquals "$LINENO" "$cGitProjVersion" "$gpVer"

    assertEquals "$LINENO" "true" "$gpSysLog"
    assertEquals "$LINENO" "user" "$gpFacility"
    assertEquals "$LINENO" "0" "$gpAuto"
    assertEquals "$LINENO" "${PWD##*/}" "$gpProjName"
    assertEquals "$LINENO" "${gpLocalTopDir}/raw" "$gpLocalRawDir"
    assertEquals "$LINENO" "10k" "$gpMaxSize"
    assertEquals "$LINENO" "false" "$gpGitFlow"
    assertNull "$LINENO" "$gpAction"

    cd - >/dev/null 2>&1
    return 0
} # testCloneSetGlobals

# --------------------------------
checkComMustNotBeInGit()
{
    local pFun=$1

    local tResult
    local tStatus

    gpUnitDebug=0

    # This dir does not exist
    tResult=$($pFun $HOME/foo-bar 2>&1)
    tStatus=$?
    fTestDebug "Check: $HOME/foo-bar"
    fTestDebug "Check: $tResult"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $pFun $tResult" "$tResult" "does not exist"

    # Git is below this dir
    tResult=$($pFun $HOME 2>&1)
    tStatus=$?
    fTestDebug "$LINENO Check: $HOME"
    fTestDebug "$LINENO Check: $tResult"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $pFun x${HOME}x $tResult" "$tResult" "must NOT be in a git repo"

    # This is in a git dir
    tResult=$($pFun $HOME/$cDatProj3 2>&1)
    tStatus=$?
    fTestDebug "$LINENO Check: $HOME/$cDatProj3"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $pFun $tResult" "$tResult" "must NOT be in a git repo"

    # This is in a git dir
    tResult=$($pFun $HOME/$cDatProj3/edit 2>&1)
    tStatus=$?
    fTestDebug "$LINENO Check: $HOME/$cDatProj3/edit"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $pFun $tResult" "$tResult" "must NOT be in a git repo"

    # No git dir above or below this dir
    tResult=$($pFun $HOME/$cDatProj1 2>&1)
    tStatus=$?
    fTestDebug "$LINENO Check: $HOME/$cDatProj1"
    assertTrue $LINENO $tStatus
    return 0
} # checkComMustNotBeInGit

# --------------------------------
testComMustNotBeInGit()
{
    gpUnitDebug=0
    checkComMustNotBeInGit fComMustNotBeInGit
    return 0
} # testComMustNotBeInGit

# --------------------------------
checkComAllMustBeReadable()
{
    local tResult
    local tStatus

    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus

    # Create a file and make it unreadable
    touch $HOME/foo
    chmod a-r $HOME/foo
    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable"

    # Create a dir and make it unreadable
    rm $HOME/foo
    mkdir $HOME/foo
    chmod a-r $HOME/foo
    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable"

    # A dir can not be listed
    chmod a+r,a-x $HOME/foo
    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "All directories must be executable"
    return 0
} # checkComAllMustBeReadable

# --------------------------------
testComAllMustBeReadable()
{
    gpUnitDebug=0
    checkComAllMustBeReadable fComAllMustBeReadable
    return 0
} # testComAllMustBeReadable

# --------------------------------
testCloneGettingStarted()
{
    local tResult
    local tStatus

    gpUnitDebug=0

    cd $HOME/$cDatProj1
    gpAuto=1
    tResult=$(fCloneGettingStarted)
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"

    gpAuto=0
    tResult=$(fCloneGettingStarted 2>&1 < <(echo -e "\nx\ny"))
    tStatus=$?
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO $tResult" "$tResult" "is not valid"
    assertTrue $LINENO $tStatus

    tResult=$(fCloneGettingStarted 2>&1 < <(echo -e "n"))
    tStatus=$?
    assertFalse $LINENO $tStatus

    cd - >/dev/null 2>&1
    return 0
} # testCloneGettingStarted

# --------------------------------
testCloneValidLocalPath()
{
    local tResult
    local tStatus

    gpUnitDebug=0

    checkComMustNotBeInGit fCloneValidLocalPath
    checkComAllMustBeReadable fCloneValidLocalPath

    tResult=$(fCloneValidLocalPath $HOME/$cDatProj1 2>&1)
    tStatus=$?
    gpUnitDebug=0
    fTestDebug "Check: $HOME/$cDatProj1"
    fTestDebug "tResult: $tResult"
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    # Called again, so that the global vars will be defined.
    fCloneValidLocalPath $HOME/$cDatProj1 >/dev/null 2>&1
    assertEquals $LINENO "$HOME/$cDatProj1" "$gpLocalTopDir"
    assertEquals $LINENO "${cDatProj1##*/}" "$gpProjName"
    return 0
} # testCloneValidLocalPath

# --------------------------------
testCloneGetLocalPath()
{
    local tResult
    local tStatus

    gpAuto=1
    # Auto

    # Auto fail
    tResult=$(fCloneGetLocalPath $HOME/foo-bar 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "does not exist"

    # Auto success
    tResult=$(fCloneGetLocalPath $HOME/$cDatProj1 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    gpAuto=0
    # No auto, prompt/response

    # Just Enter, i.e. defaultt. Look for success
    tResult=$(fCloneGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "\n"))
    tStatus=$?
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Define the existing project directory"
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    # Enter "quit". Look for quit
    tResult=$(fCloneGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "quit"))
    tStatus=$?
    gpUnitDebug=0
    fTestDebug "tResult=$tResult"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Enter $HOME; $HOME/$cDatProj3; quit
    tResult=$(fCloneGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "$HOME\n$HOME/$cDatProj3\nquit\nq"))
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Enter $HOME; $HOME/$cDatProj1
    tResult=$(fCloneGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "$HOME\n$HOME/$cDatProj1"))
    tStatus=$?
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    # Called again, so that the global vars will be defined.
    fCloneGetLocalPath $HOME/$cDatProj1 < <(echo -e "$HOME\n$HOME/$cDatProj1") >/dev/null 2>&1
    assertTrue $LINENO $tStatus
    assertEquals $LINENO "$HOME/$cDatProj1" "$gpLocalTopDir"
    assertEquals $LINENO "${cDatProj1##*/}" "$gpProjName"
    return 0
} # testCloneGetLocalPath

# --------------------------------
testCloneValidSize()
{
    local tResult
    local tStatus
    declare -l tLower

    tResult=$(fCloneValidSize "12K" 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus

    fCloneValidSize "12K"
    assertEquals $LINENO "12k" "$gpMaxSize"

    for i in 3b 34k 8m 2g 3B 34k 8M 2G; do
        fCloneValidSize "$i"
        tLower=$i
        assertEquals "$LINENO $i" "$tLower" "$gpMaxSize"
    done

    for i in 3x k 8 2 K2; do
        tResult=$(fCloneValidSize "$i" 2>&1)
        tStatus=$?
        assertFalse "$LINENO $i" $tStatus
        assertContains "$LINENO $tResult" "$tResult" "Size must be numbers followed by"
    done

    return 0
} # testCloneValidSize

# --------------------------------
testCloneGetSize()
{
    local tResult
    local tStatus
    declare -l tLower

    tResult=$(fCloneGetSize 2>&1 < <(echo -e "12K"))
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Define the size for large binary files"

    fCloneGetSize >/dev/null 2>&1 < <(echo -e "12K")
    assertTrue $LINENO $?
    assertEquals "$LINENO" "12k" "$gpMaxSize"

    tResult=$(fCloneGetSize 2>&1 < <(echo -e "42\n66x\nq"))
    tStatus=$?
    assertFalse "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Size must be numbers followed by"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    return 0
}

# --------------------------------
testCloneGetBinaryFiles()
{
    local tResult

    gpMaxSize="18k"
    gpLocalTopDir=$HOME/$cDatProj1

    tResult=$(fCloneGetBinaryFiles 2>&1)
    assertTrue "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "src/final/george.mp4"
    assertContains "$LINENO $tResult" "$tResult" "src/raw/MOV001.mp4"
    assertNotContains "$LINENO $tResult" "$tResult" "src/raw/MOV001.mp3"

    gpMaxSize="2g"
    tResult=$(fCloneGetBinaryFiles 2>&1)
    assertFalse "$LINENO" $?

    return 0
} # testCloneGetBinaryFiles

# --------------------------------
testCloneGetMoveFiles()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1

    gpMaxSize="24m"
    tResult=$(fCloneGetMoveFiles 2>&1 < <(echo -e "q"))
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "No large binary files were found"

    gpMaxSize="18k"
    tResult=$(fCloneGetMoveFiles 2>&1 < <(echo -e "x\nquit"))
    tStatus=$?
    assertFalse "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "The listed files can be moved"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpMaxSize="18k"
    fCloneGetMoveFiles >/dev/null 2>&1 < <(echo -e "x\nn")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "0" "$gpAutoMove"

    gpMaxSize="18k"
    fCloneGetMoveFiles >/dev/null 2>&1 < <(echo -e "y")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "1" "$gpAutoMove"

    return 0
} # testCloneGetMoveFiles

# --------------------------------
testCloneGetGitFlow()
{
    local tResult
    local tStatus

    tResult=$(fCloneGetGitFlow 2>&1 < <(echo -e "\nquit"))
    assertFalse "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    fCloneGetGitFlow >/dev/null 2>&1 < <(echo -e "yes")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "true" "$gpGitFlow"

    fCloneGetGitFlow >/dev/null 2>&1 < <(echo -e "No")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "false" "$gpGitFlow"

    gpGitFlowPkg=foo-bar
    tResult=$(fCloneGetGitFlow 2>&1 < <(echo -e "\n"))
    assertTrue "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "git-flow is not installed"

    return 0
} # testCloneGetGitFlow

# --------------------------------
testCloneSummary()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="10k"

    tResult=$(fCloneSummary 2>&1 < <(echo -e "foo\nn"))
    assertFalse $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Continue with creating"
    assertContains "$LINENO $tResult" "$tResult" "Invalid answer:"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpUnitDebug=0
    fTestDebug "No result = $tResult"

    tResult=$(fCloneSummary 2>&1 < <(echo -e "y"))
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Continue with creating"

    gpUnitDebug=0
    fTestDebug "Yes result = $tResult"

    return 0
} # testCloneSummary

# --------------------------------
testCloneMkRaw()
{
    local tResult

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    gpProjName=${cDatProj1##*/}
    gpLocalRawDir=$gpLocalTopDir/raw
    gpGitFlow="true"
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0
    gpVerbose=2

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkRaw 2>&1)
    assertTrue $LINENO $?
    assertTrue $LINENO "[ -d $gpLocalRawDir ]"
    assertTrue $LINENO "[ -f $gpLocalRawDir/README.txt ]"
    assertTrue $LINENO "$(
        grep -q 'Do NOT remove these files' $gpLocalRawDir/README.txt
        echo $?
    )"
    assertContains "$LINENO $tResult" "$tResult" "mkdir"
    assertContains "$LINENO $tResult" "$tResult" "Create: raw/README.txt"

    gpUnitDebug=0
    fTestDebug "$tResult"

    return 0
} # testCloneMkRaw

# --------------------------------
testCloneMoveBinaryFiles()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    gpProjName=${cDatProj1##*/}
    gpLocalRawDir=$gpLocalTopDir/raw
    gpGitFlow="true"
    gpAutoMove=true
    gpAuto=0
    gpVerbose=2

    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneMkRaw >/dev/null 2>&1
    cd $gpLocalTopDir >/dev/null 2>&1
    assertTrue $LINENO "[ -d $gpLocalRawDir ]"

    cd $gpLocalTopDir >/dev/null 2>&1
    gpMaxSize="10k"
    tResult=$(fCloneMoveBinaryFiles 2>&1)
    tStatus=$?
    assertTrue "$LINENO $tResult" "$tStatus"
    assertNotContains "$LINENO" "$tResult" "Binary files were found"
    assertNotContains "$LINENO" "$tResult" "Could not create:"
    assertNotContains "$LINENO" "$tResult" "Could not move:"
    assertNotContains "$LINENO" "$tResult" "Could not create symlink for:"
    assertContains "$LINENO" "$tResult" "Moving large binary file"
    assertContains "$LINENO" "$tResult" "Exists:"
    assertContains "$LINENO" "$tResult" "Created link"
    assertContains "$LINENO" "$tResult" "Version and use the file symlinks,"
    assertTrue $LINENO "[ -d $gpLocalRawDir ]"
    assertTrue $LINENO "[ ! -f raw/edit/george.kdenlive ]"
    assertTrue $LINENO "[ -f raw/src/raw/MOV001.mp4 ]"
    assertTrue $LINENO "[ -f raw/src/raw/MOV001.MP3 ]"
    assertTrue $LINENO "[ -f raw/src/final/george.mp4 ]"
    assertTrue $LINENO "[ -L src/raw/MOV001.mp4 ]"
    assertTrue $LINENO "[ -L src/raw/MOV001.MP3 ]"
    assertTrue $LINENO "[ -L src/final/george.mp4 ]"

    gpUnitDebug=0
    fTestDebug "Size 10k: $tResult"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after files have been moved."
        echo -e "\tRestore test-env_HomeAfterBMove.tgz relative to env cDatHome"
        cd $HOME >/dev/null 2>&1
        echo -en "\t"
        tar -cvzf $gpTest/test-env_HomeAfterBMove.tgz .
        echo
    fi

    return 0
} # testCloneMoveBinaryFiles

# --------------------------------
testCloneMkGitFlow()
{
    local tResult

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0

    tResult=$(fCloneMkGitFlow 2>&1)
    assertTrue $LINENO $?
    assertEquals $LINENO "main" "$(git config --get --global gitflow.branch.main)"
    assertEquals $LINENO "develop" "$(git config --get --global gitflow.branch.develop)"
    assertEquals $LINENO "feature/" "$(git config --get --global gitflow.prefix.feature)"
    assertEquals $LINENO "bug/" "$(git config --get --global gitflow.prefix.bugfix)"
    assertEquals $LINENO "release/" "$(git config --get --global gitflow.prefix.release)"
    assertEquals $LINENO "hotfix/" "$(git config --get --global gitflow.prefix.hotfix)"
    assertEquals $LINENO "support/" "$(git config --get --global gitflow.prefix.support)"

    return 0
} # testCloneMkGitFlow

# --------------------------------
testCloneMkGitDir()
{
    local tResult
    local tStatus
    local tTop

    gpLocalTopDir=$HOME/$cDatProj1
    cd $HOME >/dev/null 2>&1
    tar -xzf $gpTest/test-env_HomeAfterBMove.tgz

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1

    tResult=$(fCloneMkGitDir 2>&1)
    tStatus=$?
    assertTrue $LINENO "$tStatus"
    assertTrue $LINENO "[ -d $gpLocalTopDir/.git ]"
    assertTrue $LINENO "[ -f $gpLocalTopDir/.gitignore ]"
    assertTrue $LINENO "$(
        grep -q raw $gpLocalTopDir/.gitignore
        echo $?
    )"

    tTop=$($cGetTopDir)
    assertContains $LINENO "$tTop" "$gpLocalTopDir"

    tResult=$(git branch 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "develop"
    assertContains "$LINENO $tResult" "$tResult" "main"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after git init."
        echo -e "\tRestore test-env_ProjAfterGClone.tgz relative to cDatHome/project"
        cd $HOME/project >/dev/null 2>&1
        tar -cvzf $gpTest/test-env_ProjAfterGClone.tgz $gpProjName
    fi

    assertTrue "$LINENO not exec" "[ -x $tTop/.git/hooks/pre-commit ]"
    assertTrue "$LINENO diff" "diff $gpDoc/hooks/pre-commit $tTop/.git/hooks/pre-commit"

    return 0
} # testCloneMkGitDir

# --------------------------------
testCloneMkLocalConfig()
{
    local tSrc=${BASH_SOURCE##*/}
    local tResult

    if [ ! -f $gpTest/test-env_HomeAfterBMove.tgz ]; then
        fail "Missing test-env_HomeAfterBMove.tgz [$tSrc:$LINENO]"
        return 1
    fi
    if [ ! -f $gpTest/test-env_ProjAfterGClone.tgz ]; then
        fail "Missing test-env_ProjAfterGClone.tgz [$tSrc:$LINENO]"
        return 1
    fi

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    cd $HOME >/dev/null 2>&1
    tar -xzf $gpTest/test-env_HomeAfterBMove.tgz
    cd $gpTest

    cd $HOME/project >/dev/null 2>&1
    tar -xzf $gpTest/test-env_ProjAfterGClone.tgz
    cd $gpTest

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="10k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1
    assertFalse $LINENO "[ -f $cConfigLocal ]"
    assertFalse $LINENO "[ -f $cConfigHost ]"
    assertFalse $LINENO "$(grep -q gitproj.config .git/config >/dev/null 2>&1); echo $?)"

    tResult=$(fCloneMkLocalConfig 2>&1)
    assertTrue $LINENO "$?"
    assertTrue $LINENO "[ -f $cConfigLocal ]"
    assertTrue $LINENO "[ -f $cConfigHost ]"

    tResult=$(git config --local --list --show-origin --includes 2>&1)
    assertTrue $LINENO "$?"
    assertContains "$LINENO $tResult" "include.path=../$cConfigHost"

} # testCloneMkLocalConfig

# --------------------------------
testCloneSaveVarsToConfigs()
{
    local tSrc=${BASH_SOURCE##*/}
    local tResult
    local tFile
    local tS

    if [ ! -f $gpTest/test-env_ProjAfterGClone.tgz ]; then
        fail "Missing test-env_ProjAfterGClone.tgz [$tSrc:$LINENO]"
        return 1
    fi
    cd $HOME/project >/dev/null 2>&1
    tar -xzf $gpTest/test-env_ProjAfterGClone.tgz
    cd $gpTest

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkLocalConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue $LINENO "[ -f $cConfigLocal ]"
    assertTrue $LINENO "[ -f $cConfigHost ]"

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="10k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneSaveVarsToConfigs 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    # TBD refactor to use for loops with hash array maps?
    # cMapConf[gpBin]=gitproj.config.bin
    # generated from cMapConf: cMapVar[gitproj.config.bin]=gpBin
    # cMapFile[gitproj.config.bin]=global
    # cMapFile[gitproj.config.proj-name]=local
    # if cMapFile[key] is undefined, then assume it is global and local

    tFile=~/.gitproj.config.global
    tS=gitproj.config
    fTestCheckConfig2Var $tFile $tS.proj-status gpProjStatus ${gpProjStatus} $LINENO
    fTestCheckConfig2Var $tFile $tS.bin gpBin "${gBin#$gpLocalTopDir/}" $LINENO
    fTestCheckConfig2Var $tFile $tS.doc gpDoc "${gDoc#$gpLocalTopDir/}" $LINENO
    fTestCheckConfig2Var $tFile $tS.test gpTest "${gTest#$gpLocalTopDir/}" $LINENO
    fTestCheckConfig2Var $tFile $tS.facility gpFacility user $LINENO
    fTestCheckConfig2Var $tFile $tS.syslog gpSysLog true $LINENO

    for tFile in $gpLocalTopDir/$cConfigLocal \
        $gpLocalTopDir/$cConfigHost; do
        tS=gitproj.config
        fTestCheckConfig2Var $tFile $tS.local-status gpLocalStatus not-defined $LINENO
        fTestCheckConfig2Var $tFile $tS.remote-status gpRemoteStatus not-defined $LINENO
        fTestCheckConfig2Var $tFile $tS.proj-name gpProjName $gpProjName $LINENO
    done

    for tFile in ~/.gitproj.config.global $gpLocalTopDir/$cConfigLocal; do
        tS=gitproj.config
        fTestCheckConfig2Var $tFile $tS.git-flow-pkg gpGitFlow true $LINENO
        tS=gitproj.hook
        fTestCheckConfig2Var $tFile $tS.auto-move gpAutoMove true $LINENO
        fTestCheckConfig2Var $tFile $tS.binary-file-size-limit gpMaxSize 10k $LINENO
        fTestCheckConfig2Var $tFile $tS.check-file-names gpCheckFileNames true $LINENO
        fTestCheckConfig2Var $tFile $tS.check-for-big-files gpCheckForBigFiles true $LINENO
        fTestCheckConfig2Var $tFile $tS.pre-commit-enabled gpPreCommitEnabled true $LINENO
        fTestCheckConfig2Var $tFile $tS.source gpHookSource hooks/pre-commit $LINENO
    done

    return 0
} # testCloneSaveVarsToConfigs

# --------------------------------
testCloneCreateLocalGitAuto()
{
    local tSrc=${BASH_SOURCE##*/}
    local tResult
    local tStatus
    local tFile
    local tS

    gpGitFlow="true"
    gpMaxSize="10k"
    gpAutoMove=true

    gpAuto=1
    gpSysLog=true
    gpVerbose=3
    gpDebug=10

    # Create local git and local raw
    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneCreateLocalGit 2>&1)
    tStatus=$?
    assertTrue "$LINENO $tResult" "$tStatus"
    assertTrue "$LINENO" "[ -d $gpLocalTopDir/raw ]"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after git init."
        echo -e "\tRestore test-env_ProjLocalDefined.tgz relative to HOME/project"
        cd $HOME >/dev/null 2>&1
        tar -cvzf $gpTest/test-env_ProjLocalDefined.tgz .gitconfig .gitproj.config.global $cDatProj1
    fi

    return 0
} # testCloneCreateLocalGitAuto

# --------------------------------
TBDtestCloneCreateLocalGitPrompted()
{
    startSkipping
    fail "TBD"
    return 0
} # testCloneCreateLocalGitPrompted

# --------------------------------
TBDtestGetProjCloneLocalAutoCLI()
{
    startSkipping
    fail "TBD"
    return 0

    cd $HOME/$cDatProj1
    $gpBin/git-proj-clone local -a
} # testGetProjCloneLocalAutoCLI

# --------------------------------
TBDtestGetProjCloneLocalPromptedCLI()
{
    startSkipping
    fail "TBD"
    return 0

    cd $HOME/$cDatProj1
    $gpBin/git-proj-clone local -a
} # testGetProjCloneLocalPromptedCLI

# ========================================
# This should be the last defined function

# --------------------------------
fTestRun()
{
    if [ ! -x $gpTest/shunit2.1 ]; then
        echo "Error: Missing: $gpTest/shunit2.1"
        exit 1
    fi
    shift $#
    if [ -z "$gpTestList" ]; then
        # shellcheck disable=SC1091
        . $gpTest/shunit2.1
        exit $?
    fi

    gpTestList=$(echo $gpTestList | tr "," " ")
    # shellcheck disable=SC1091
    . $gpTest/shunit2.1 -- $gpTestList
    exit $?

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 fTestRun

Run unit tests for the common functions.

=internal-cut
EOF
} # fTestRun

# ====================
# Main

export gpTest cTestCurDir gpTestList gpCmdName gpSaveTestEnv

gpCmdName=${BASH_SOURCE##*/}

# -------------------
# Set current directory location in PWD and cTestCurDir
if [ -z "$PWD" ]; then
    PWD=$(pwd)
fi
cTestCurDir=$PWD

# -------------------
# Define the location of this script
gpTest=${0%/*}
if [ "$gpTest" = "." ]; then
    gpTest=$PWD
fi
cd $gpTest >/dev/null 2>&1
gpTest=$PWD
cd - >/dev/null 2>&1

# -----
# Optional input: a comma separated list of test function names
gpTestList="$*"

# -----
. $gpTest/test.inc
fTestCreateEnv
. $gpBin/gitproj-clone.inc

# Look for serious setup errors
fTestConfigSetup

fTestRun $gpTestList