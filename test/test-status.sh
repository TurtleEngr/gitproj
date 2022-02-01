#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-gitproj.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 NAME test-status.sh

Test git-proj-status and gitproj-status.inc

=head1 SYNOPSIS

    ./test-status.sh [test,test,...]

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
}

# ========================================

# --------------------------------
setUp()
{
    # Restore default global values, before each test

    unset cGetOrigin cGetTopDir cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    local tTarIn=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz
    local tTarIn2=$gpTest/test-env_Home3AfterCloneSummary.tgz
    local tVer
    local tConf

    HOSTNAME=testserver2
    fTestSetupEnv
    fTestCreateEnv

    mkdir -p $cDatHome3/project >/dev/null 2>&1
    cd $cTestDestDir >/dev/null 2>&1
    if [ ! -r $tTarIn ]; then
        echo "Missing: $tTarIn [$LINENO]" 1>&2
        exit 1
    fi
    tar -xzf $tTarIn
    cd - >/dev/null 2>&1

    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTarIn2 ]; then
        echo "Missing: $tTarIn2 [$LINENO]" 1>&2
        exit 1
    fi
    tar -xzf $tTarIn2

    # git proj to be cloned:
    HOME=$cDatHome3
    gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git

    tVer=$(cat $gpDoc/VERSION)
    tVer=$(echo $tVer)

    # Patch things that may have been in the tar file
    for tConf in \
        $cDatHome3/project/george/.gitproj.config.testserver \
        $cDatHome3/project/george/.gitproj.config.testserver2 \
        $cDatHome3/project/george/.gitproj.config.local \
        $cDatHome3/project/george/.*.bak \
        $cDatHome3/project/george/.*.bak.*~; do
        rm -f $fConfig >/dev/null 2>&1
    done
    git config -f $cDatHome3/project/george/.gitproj gitproj.config.ver $tVer
    git config -f $cDatHome3/project/george/.git/config gitproj.config.ver $tVer

    cd $HOME/project/george >/dev/null 2>&1
    . $gpBin/gitproj-status.inc

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

tearDown()
{
    ##    git config --global --remove-section gitproj.test >/dev/null 2>&1
    gpUnitDebug=0
    return 0
} # tearDown

# ========================================

# --------------------------------
testSetup()
{
    local tResult
    local tOrigin

    cd $HOME/project/george >/dev/null 2>&1
    assertTrue "$LINENO" "[ -f $HOME/.gitconfig ]"
    assertTrue "$LINENO" "[ -f .git/config ]"
    assertTrue "$LINENO" "[ -f .gitproj ]"
    assertTrue "$LINENO" "[ -d raw ]"
    tOrigin=$($cGetOrigin)
    assertTrue "$LINENO" $?
    assertTrue "$LINENO $tOrigin" "[ -d $tOrigin ]"
    gpRemoteRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-origin" -d "None")
    assertNotContains "$LINENO" "$gpRemoteRawOrigin" "None"
    assertTrue "$LINENO" "[ -d $gpRemoteRawOrigin ]"

    return 0
} # testSetup

# ====================
# This should be the last defined function
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

export gpTest cTestCurDir gpTestList gpCmdName

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
cd $gpTest
gpTest=$PWD
cd $cTestCurDir

# -----
# Optional input: a comma separated list of test function names
gpTestList="$*"

# -----
. $gpTest/test.inc

fTestRun $gpTestList
