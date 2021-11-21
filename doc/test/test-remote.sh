#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-remote.sh

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

    ./test-remote.sh [all] [test,test,...]

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

    unset cConfigGlobal cConfigLocal cConfigHost cCurDir cGetOrigin \
        cGetTopDir cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpHardLink gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVar gpVerbose

    fTestSetupEnv
    fTestCreateEnv
    cd $HOME >/dev/null 2>&1
    tar -xzf $gpTest/test-env_ProjLocalDefined.tgz
    cd - >/dev/null 2>&1

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    . $gpBin/gitproj-remote.inc
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
testGitProjRemote()
{
    local tResult

    tResult=$($gpBin/git-proj-remote 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'Usage'

    tResult=$($gpBin/git-proj-remote -h)
    assertContains "$LINENO $tResult" "$tResult" 'DESCRIPTION'

    cd $HOME/$cDatProj1 >/dev/null 2>&1
    assertTrue "$LINENO" "[ -d .git ]"
    cd - >/dev/null 2>&1
    
    return 0
} # testGitProjInit

# --------------------------------
testComGetVer()
{
    local tResult
    local tVer

    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO" "$cGitProjVersion" "$gpVer"

    cGitProjVersion="0.1.8"
    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO" "$cGitProjVersion" "$gpVer"

    cGitProjVersion="0.2.0"
    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO" "$tResult" "warning"
    assertContains "$LINENO" "$tResult" "but installed version is"
 
    cGitProjVersion="1.3.0"
    tResult=$(fComGetVer 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: "
    assertContains "$LINENO $tResult" "$tResult" "project needs to be upgraded"

    gpDebug=2
    gpVerbose=2

    cGitProjVersion="0.2.0"
    tVer=1.3
    fComSetConfig -G -k "gitproj.config.ver" -v "$tVer"
    tResult=$(fComGetVer 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: "
    assertContains "$LINENO $tResult" "$tResult" "installed version $cGitProjVersion needs to be upgraded to $tVer or greater"
return 0

    fComUnsetConfig -G -k "gitproj.config.ver"
    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO" "$tResult" "warning"
    assertContains "$LINENO" "$tResult" "Internal"
    assertContains "$LINENO" "$tResult" "gitproj.config.ver was not found"
    assertContains "$LINENO" "$cGitProjVersion" "$gpVer"

    return 0
} # testComGetVer

# --------------------------------
testInGitProjDir()
{
    local tResult

    gpDebug=2
    
    cd $HOME >/dev/null 2>&1
    tResult=$(fRemoteSetGlobals 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "You must be in a git workspace for this command"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    mv .gitproj.config.testserver .gitproj.config.testserver.sav
    tResult=$(fRemoteSetGlobals 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Missing .gitproj.config.$HOSTNAME, run"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    mv .gitproj.config.testserver.sav .gitproj.config.testserver
    rm .gitproj.config.local
    tResult=$(fRemoteSetGlobals 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "git workspace is not setup for gitproj"

    return 0
} # testInGitProjDir

# --------------------------------
TBDtestRemoteGetMountDir()
{
    gpAuto=1
    gpMountDir=exists
    gpMountDir=not-exists

#    cDatMount1=$cTestDestDir/test/root/mnt/disk-2
#    cDatMount2=$cTestDestDir/test/root/mnt/usb-misc/files-2021-08-12
#    cDatMount3=$cTestDestDir/test/root/mnt/usb-video/video-2020-04-02
#    /media/$USER/xxxx
#    df -h | grep -E '^/dev/|/mnt'

    startSkipping
    fail "TBD"
    return 0
} # testRemoteGetMountDir

# --------------------------------
TBDtestRemoteGetRawRemoteDir()
{
    startSkipping
    fail "TBD"
    return 0
} # testRemoteGetRawRemoteDir

# --------------------------------
TBDtestRemoteCheckDir()
{
    startSkipping
    fail "TBD"
    return 0
} # testRemoteCheckDir

# --------------------------------
TBDtestRemoteCheckSpace()
{
    startSkipping
    fail "TBD"
    return 0
} # testRemoteCheckSpace

# --------------------------------
TBDtestRemoteMkRemote()
{
    startSkipping
    fail "TBD"
    return 0
} # testRemoteMkRaw

# --------------------------------
TBDtestRemoteReport()
{
    startSkipping
    fail "TBD"
    return 0
} # testRemoteReport

# --------------------------------
TBDtestRemoteCreateRemoteGit()
{
    startSkipping
    fail "TBD"
    return 0
} # testRemoteCreateRemoteGit

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
fTestSetupEnv
fTestCreateEnv
cd $HOME >/dev/null 2>&1
tar -xzf $gpTest/test-env_ProjLocalDefined.tgz
cd - >/dev/null 2>&1

# Look for serious setup errors
fTestConfigSetup

fTestRun $gpTestList
