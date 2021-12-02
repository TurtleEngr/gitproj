#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-push.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc()
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-push.sh

test-push.sh - test git-proj-push and gitproj-push.inc

=head1 SYNOPSIS

    ./test-push.sh [test,test,...]

=head1 DESCRIPTION

shunit2.1 is used to run the unit tests. If no test function names are
listed, then all of the test functions will be run.

=head1 RETURN VALUE

0 - if OK

=for comment =head1 ERRORS

=for comment =head1 EXAMPLES

=for comment =head1 ENVIRONMENT

=for comment =head1 FILES

=head1 SEE ALSO

shunit2.1

=for comment =head1 NOTES

=for comment =head1 CAVEATS

=for comment =head1 DIAGNOSTICS

=for comment =head1 BUGS

=for comment =head1 RESTRICTIONS

=for comment =head1 AUTHOR

=for comment =head1 HISTORY

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

=internal-head2 Test gitproj-pushinc

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

    unset cConfigGlobal cConfigLocal cConfigHost cCurDir cGetOrigin \
        cGetTopDir cGitProjVersion cInteractive cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpHardLink gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    fTestSetupEnv
    fTestCreateEnv
    cd $cTestDestDir >/dev/null 2>&1
    tar -xzf $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    cd - >/dev/null 2>&1

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    . $gpBin/gitproj-push.inc

    gpDebug=0
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
    cd $gpTest >/dev/null 2>&1
    return 0
} # tearDown

# ========================================

# --------------------------------
testComGetProjGlobals()
{
    local tResult
    
    cd $cDatHome >/dev/null 2>&1
    tResult=$(fComGetProjGlobals 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "You must be in a git workspace for this command."

    cd $cDatHome/$cDatProj3 >/dev/null 2>&1
    tResult=$(fComGetProjGlobals 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "This git workspace is not setup for gitproj, run"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComGetProjGlobals >/dev/null 2>&1
    assertTrue "$LINENO $tResult" "$?"
    assertEquals "$LINENO" "$cDatMount3/video-2020-04-02/$gpProjName.raw" "$gpRemoteRawDir"

    tResult=$(git config --get --local remote.origin.url)
    assertEquals "$LINENO" "$cDatMount3/video-2020-04-02/$gpProjName.git" "$tResult"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComSetConfig -H -k "gitproj.config.proj-name" -v "TBD"
    fComSetConfig -L -k "gitproj.config.proj-name" -v "TBD"
    tResult=$(fComGetProjGlobals 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: Unexpected: gitproj.config.proj-name should not be set to: TBD"

    return 0
} # testComGetProjGlobals

# --------------------------------
testPushIsRemoteMounted()
{
    local tResult

    fComGetProjGlobals >/dev/null 2>&1
    
    tResult=$(fPushIsRemoteMounted 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    mv $gpRemoteRawDir $gpRemoteRawDir.sav
    tResult=$(fPushIsRemoteMounted 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "was not found. Try again after mounting it or run 'git proj config' to change the remote.raw.dir location"
    mv $gpRemoteRawDir.sav $gpRemoteRawDir 
    
    return 0
} # testPushIsRemoteMounted

# --------------------------------
testPushRawFiles()
{
    local tResult

    fComGetProjGlobals >/dev/null 2>&1

    gpVerbose=2
    tResult=$(fPushRawFiles 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "x"
    
    return 0
} # testPushRawFiles


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
gpCmdName=git-proj-remote

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
fTestSetupEnv
fTestCreateEnv
cd $HOME >/dev/null 2>&1
tar -xzf $gpTest/test-env_ProjLocalDefined.tgz
cd - >/dev/null 2>&1

# Look for serious setup errors
fTestConfigSetup

fTestRun $gpTestList
