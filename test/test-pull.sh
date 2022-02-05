#!/bin/bash

export gpTest cTestCurDir gpTestList gpCmdName gpSaveTestEnv

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-pull.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-pull.sh

test-pull.sh - test git-proj-pull and gitproj-pull.inc

=head1 SYNOPSIS

    ./test-pull.sh [test,test,...]

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
    local tVer=$(cat $gpDoc/VERSION)
    tVer=$(echo $tVer)
    local tConf
    local tTarIn=$gpTest/test-env_Home2AfterPush.tgz

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

    # Patch the version that was set in the tar file

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComSetConfig -l -k gitproj.config.ver -v $(cat $gpDoc/VERSION)
    fComSetConfig -L -k gitproj.config.ver -v $(cat $gpDoc/VERSION)
    git ci -am "Updated" >/dev/null 2>&1

    cd $cDatHome2/$cDatProj1 >/dev/null 2>&1
    fComSetConfig -l -k gitproj.config.ver -v $(cat $gpDoc/VERSION)
    fComSetConfig -L -k gitproj.config.ver -v $(cat $gpDoc/VERSION)
    git ci -am "Updated" >/dev/null 2>&1

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    . $gpBin/gitproj-pull.inc

    # --------
    # Make changes to $cDatHome2 area

    cd $cDatHome2/$cDatProj1 >/dev/null 2>&1
    HOME=$cDatHome2
    fComGetProjGlobals

    # Add a file to local raw/ and remove a file from local raw/
    echo "Make a new file" >raw/NewFile2.txt
    rm raw/src/raw/MOV001.mp4

    # Add a file to local git, and change a file in local git
    echo "Add a file to git area" >doc/NewFileFromBob.txt
    echo 'Make a change' >>README.html
    git add doc/NewFileFromBob.txt README.html >/dev/null 2>&1
    git commit -am Updated >/dev/null 2>&1
    git push origin develop >/dev/null 2>&1

    # Push changes to remote ($gpRemoteRawOrigin)
    $gpBin/git-proj-push -d >/dev/null 2>&1 < <(echo -e "y")

    # --------
    # Now test "git proj pull" command and functions, from this user
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    HOME=$cDatHome
    fComGetProjGlobals

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
testSetUp()
{
    local tResult

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    HOME=$cDatHome
    fComGetProjGlobals

    assertFalse "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertFalse "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertFalse "$LINENO" "[ -f doc/NewFileFromBob.txt ]"

    assertTrue "$LINENO" "[ -f $gpRemoteRawOrigin/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f $gpRemoteRawOrigin/src/raw/MOV001.mp4 ]"

    cd $cDatHome2/$cDatProj1 >/dev/null 2>&1
    HOME=$cDatHome2
    fComGetProjGlobals

    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertTrue "$LINENO" "[ -f doc/NewFileFromBob.txt ]"

    assertTrue "$LINENO" "[ -f $gpRemoteRawOrigin/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f $gpRemoteRawOrigin/src/raw/MOV001.mp4 ]"

    return 0
} # testSetUp

# --------------------------------
testPullRawFilesNoDelete()
{
    local tResult

    # Setup a change in raw files in $cDatHome/$cDatProj1
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComGetProjGlobals

    # No Pull
    tResult=$(fPullRawFiles 1 2>&1 < <(echo -e "n"))
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "raw: NewFile2.txt"
    assertContains "$LINENO $tResult" "$tResult" "raw/src/raw: MOV001.mp4"
    assertContains "$LINENO $tResult" "$tResult" "Dry Run"
    assertContains "$LINENO $tResult" "$tResult" "Nothing was pulled"
    assertFalse "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertFalse "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertFalse "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    # Yes Pull, no delete
    tResult=$(fPullRawFiles 0 2>&1 < <(echo -e "y"))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "raw: NewFile2.txt"
    assertContains "$LINENO $tResult" "$tResult" "raw/src/raw: MOV001.mp4"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertFalse "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertFalse "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testPullRawFilesNoDelete

# --------------------------------
testPullRawFilesDelete()
{
    local tResult

    # Setup a change in raw files in $cDatHome/$cDatProj1
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    fComGetProjGlobals

    # Yes Pull, delete
    tResult=$(fPullRawFiles 1 2>&1 < <(echo -e "y"))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "raw: NewFile2.txt"
    assertContains "$LINENO $tResult" "$tResult" "raw/src/raw: MOV001.mp4"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertFalse "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    tResult=$(fPullRawFiles 0 2>&1 < <(echo -e "n"))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There are no differences found with 'raw' files"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testPullRawFilesDelete

# --------------------------------
testPullGit()
{
    local tResult

    # diff -rq /home/john/project/george home/bob/project/george | grep -Ev '.git|raw'
    # Only in /home/bob/project/george/doc: NewFileFromBob.txt
    # Files /home/john/project/george/README.html and /home/bob/project/george/README.html differ

    tResult=$(fPullGit 0 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "git pull --ff-only origin develop"

    tResult=$(fPullGit 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "git pull --ff-only origin develop"

    ##git status
    ##git diff

    tResult=$(fPullGit 1 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    assertTrue "$LINENO" "grep 'Make a change' README.html"
    assertContains "$LINENO $tResult" "$tResult" "2 files changed"
    assertContains "$LINENO $tResult" "$tResult" "git pull --ff-only origin develop"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    # TODO: what if a current local branch has no remote branch?
    # TODO: merge conflicts with changed local files?

    return 0
} # testPullGit

# --------------------------------
testPullFromOrigin()
{
    local tResult

    tResult=$(fPullFromOrigin 1 1 2>&1 < <(echo -e y))
    sleep 0.5
    assertTrue "$LINENO $tResult" "$?"
    # raw
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    # git
    assertTrue "$LINENO $(echo $PWD; ls -la doc)" "[ -f doc/NewFileFromBob.txt ]"
    assertTrue "$LINENO" "grep 'Make a change' README.html"
    assertContains "$LINENO $tResult" "$tResult" "git pull --ff-only origin develop"
    # debug
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testPullFromOrigin

# --------------------------------
testGitProjPullCLI()
{
    local tResult

    tResult=$($gpBin/git-proj-pull -g -d -V 3 2>&1 < <(echo -e y))
    sleep 0.5
    assertTrue "$LINENO $tResult" "$?"
    # raw
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertTrue "$LINENO $tResult" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    # git
    assertTrue "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    assertTrue "$LINENO" "grep 'Make a change' README.html"
    assertContains "$LINENO $tResult" "$tResult" "git pull --ff-only origin develop"
    # debug
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testGitProjPullCLI

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
