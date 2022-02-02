#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-push.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc
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
oneTimeSetUp()
{
    local tTarIn=$gpTest/test-env_ProjLocalDefined.tgz

    fTestSetupEnv
    fTestCreateEnv
    cd $HOME >/dev/null 2>&1
    if [ ! -e $tTarIn ]; then
        echo "Missing: $tTarIn" 1>&2
        exit 1
    fi
    tar -xzf $tTarIn
    cd - >/dev/null 2>&1
    fTestPatchPath

    # Look for serious setup errors
    fTestConfigSetup
} # oneTimeSetUp

# --------------------------------
setUp()
{
    local tTarIn=$gpTest/test-env_TestDestDirAfterRemoteReport.tgz

    # Restore default global values, before each test

    unset cGetOrigin cGetTopDir cGitProjVersion cInteractive cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpHardLink gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    fTestSetupEnv
    fTestCreateEnv
    cd $cTestDestDir >/dev/null 2>&1
    if [ ! -e $tTarIn ]; then
        echo "Missing: $tTarIn" 1>&2
        exit 1
    fi
    tar -xzf $tTarIn
    cd - >/dev/null 2>&1
    fTestPatchPath

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComSetConfig -l -k gitproj.config.ver -v $(cat $gpDoc/VERSION)
    fComSetConfig -L -k gitproj.config.ver -v $(cat $gpDoc/VERSION)

    . $gpBin/gitproj-push.inc

    gpVerbose=3
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
    assertContains "$LINENO $tResult" "$tResult" "This git workspace is not setup for gitproj"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComGetProjGlobals >/dev/null 2>&1
    assertTrue "$LINENO $tResult" "$?"
    assertEquals "$LINENO" "$cDatMount3/video-2020-04-02/$gpProjName.raw" "$gpRemoteRawOrigin"

    tResult=$(git config --get --local remote.origin.url)
    assertEquals "$LINENO" "$cDatMount3/video-2020-04-02/$gpProjName.git" "$tResult"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComSetConfig -l -k "gitproj.config.proj-name" -v "TBD"
    fComSetConfig -L -k "gitproj.config.proj-name" -v "TBD"
    tResult=$(fComGetProjGlobals 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: Error: Unexpected: gitproj.config.proj-name should not be set to: TBD"

    return 0
} # testComGetProjGlobals

# --------------------------------
testComIsRemoteMounted()
{
    local tResult

    fComGetProjGlobals >/dev/null 2>&1

    tResult=$(fComIsRemoteMounted notice 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNull "$LINENO" "$tResult"

    mv $gpRemoteRawOrigin $gpRemoteRawOrigin.sav
    tResult=$(fComIsRemoteMounted warning 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning"
    assertContains "$LINENO $tResult" "$tResult" "was not found. Try again after mounting it or run"

    tResult=$(fComIsRemoteMounted 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertNull "$LINENO" "$tResult"
    assertNotContains "$LINENO $tResult" "$tResult" "warning"

    gpDebug=1
    tResult=$(fComIsRemoteMounted 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "debug:"
    gpDebug=0

    mv $gpRemoteRawOrigin.sav $gpRemoteRawOrigin

    return 0
} # testComIsRemoteMounted

# --------------------------------
testPushRawFiles()
{
    local tResult

    fComGetProjGlobals >/dev/null 2>&1

    gpAuto=0
    tResult=$(fPushRawFiles 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There are no differences found with 'raw' files"

    echo "Make a new file" >$gpLocalTopDir/raw/NewFile.txt

    tResult=$(fPushRawFiles 2>&1 < <(echo -e "n"))
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "NewFile.txt"
    assertContains "$LINENO $tResult" "$tResult" "diff summary"
    assertContains "$LINENO $tResult" "$tResult" "Dry Run"
    assertContains "$LINENO $tResult" "$tResult" "warning: Nothing was pushed"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    tResult=$(fPushRawFiles 2>&1 < <(echo -e "y"))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "NewFile.txt"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    assertTrue "$LINENO" "[ -f $gpRemoteRawOrigin/NewFile.txt ]"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testPushRawFiles

# --------------------------------
testPushGit()
{
    local tResult

    fComGetProjGlobals >/dev/null 2>&1

    echo "Make a change." >>README.html
    git commit -am "Updated README.html" >/dev/null 2>&1
    assertTrue "$LINENO" "$?"

    tResult=$(fPushGit 1 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "git push origin develop"

    echo "Make another change." >>README.html
    git commit -am "Updated README.html" >/dev/null 2>&1
    assertTrue "$LINENO" "$?"

    tResult=$(fPushGit 0 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "git push origin develop"

    tResult=$(fPushGit 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "git push origin develop"

    return 0
} # testPushGit

# --------------------------------
testPushToOrigin()
{
    local tResult

    fComGetProjGlobals >/dev/null 2>&1

    echo "Make a change." >>README.html
    git commit -am "Updated README.html" >/dev/null 2>&1
    assertTrue "$LINENO" "$?"
    echo "New file in raw/" >raw/newfile.txt

    gpAuto=0
    tResult=$(fPushToOrigin 1 0 2>&1 < <(echo -e y))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "git push origin develop"
    assertTrue "$LINENO" "[ -f $gpRemoteRawOrigin/newfile.txt ]"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testPushToOrigin

# --------------------------------
testGitProjPushCLI()
{
    local tResult
    local tStatus
    local tTarOut=$gpTest/test-env_Home2AfterPush.tgz

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    echo "Make a change." >>README.html
    git commit -am "Updated README.html" >/dev/null 2>&1
    assertTrue "$LINENO $tResult" "$?"
    echo "New file in raw/" >raw/newfile.txt

    tResult=$($gpBin/git-proj-push -V 3 2>&1 < <(echo -e "y"))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "newfile.txt"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    tResult=$($gpBin/git-proj-push -g -V 3 2>&1)
    tStatus=$?
    assertTrue "$LINENO $tResult" "$tStatus"
    assertContains "$LINENO $tResult" "$tResult" "There are no differences found with 'raw' files"
    assertContains "$LINENO $tResult" "$tResult" "git push origin develop"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    # ----------
    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after files pushed."
        echo -e "\tRestore $tTarOut relative to cTestDestDir"
	fTestSavePath
        mkdir -p $cDatHome2
        rsync -a $cDatHome/ $cDatHome2
        rm -rf $cDatHome2/project/beach
        rm -rf $cDatHome2/project/paulb
        cd $cTestDestDir >/dev/null 2>&1
        echo -en "\t"
        tar -czf $tTarOut test
        echo
    fi

    return 0
} # testGitProjPushCLI

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

fTestRun $gpTestList
