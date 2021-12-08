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
    return 1

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
    return 1
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
    cd $cTestDestDir >/dev/null 2>&1
    tar -xzf $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    cd - >/dev/null 2>&1
    # git proj to be cloned:
    # $cDatMount3/video-2020-04-02/george.git
    # $cDatMount3/video-2020-04-02/george.raw

    mkdir -p $cDatHome3/project >/dev/null 2>&1
    HOME=$cDatHome3
    HOSTNAME=testserver2
    cd $cDatHome3/project >/dev/null 2>&1
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
testGitProjCloneUsage()
{
    local tResult

    tResult=$($gpBin/git-proj-clone 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'Usage'

    tResult=$($gpBin/git-proj-clone -h 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'DESCRIPTION'

    # git proj init [-l pDirPath] [-r] [-e pDirPath] [-h]

    cd $cDatHome3 >/dev/null 2>&1
    assertEquals "$LINENO" "$HOME" "$cDatHome3"
    assertFalse "$LINENO" "[ -d .git ]"
    cd - >/dev/null 2>&1
    return 0
} # testGitProjCloneUsage

testCloneGettingStarted()
{
    local tResult
    local tStatus
#TBD
return 1
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
testCloneValidRemoteDir()
{
    local tResult
    return 1
} # testCloneValidRemoteDir

# --------------------------------
testCloneSummary()
{
    local tResult
    local tStatus
return 1

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
testCloneMkGitDir()
{
    local tResult
    local tStatus
    local tTop
return 1

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
testCloneMkRawDir()
{
    local tResult
return 1

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
} # testCloneMkRawDir

# --------------------------------
testCloneMkHostConfig()
{
# TBD
return 1
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
testCloneSaveVars()
{
return 1
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
} # testCloneSaveVars

# --------------------------------
testCloneFromRemoteDir()
{
return 1
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
} # testCloneFromRemoteDir

# --------------------------------
testGetProjCloneCLI()
{
return 1
    startSkipping
    fail "TBD"
    return 0

    cd $HOME/$cDatProj1
    $gpBin/git-proj-clone local -a
} # testGetProjCloneCLI

# --------------------------------
NAtestCloneGetGitFlow()
{
return 1
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
NAtestCloneMkGitFlow()
{
return 1
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
