#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-pull.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc()
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
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-head2 Test gitproj-pull.inc

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
    local tVer=$(cat $gpDoc/VERSION)
    tVer=$(echo $tVer)
    local tConf

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
    tar -xzf $gpTest/test-env_Home2AfterPush.tgz
    cd - >/dev/null 2>&1

    # Patch the version that was set in the tar file
    for tConf in \
        $cDatHome/$cDatProj1/.gitproj.config.testserver \
        $cDatHome/$cDatProj1/.gitproj.config.local \
        $cDatHome2/$cDatProj1/.gitproj.config.testserver \
        $cDatHome2/$cDatProj1/.gitproj.config.local \
      ; do
	git config -f $tConf gitproj.config.ver $tVer
    done
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    git ci -am "Updated" >/dev/null 2>&1
    cd $cDatHome2/$cDatProj1 >/dev/null 2>&1
    git ci -am "Updated" >/dev/null 2>&1

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    . $gpBin/gitproj-pull.inc

    # --------
    # Make changes to $cDatHome2 area
    cd $cDatHome2/$cDatProj1 >/dev/null 2>&1
    HOME=$cDatHome2
    fComGetProjGlobals
    # Add a file to local raw/ and remove a file from local raw/
    echo "Make a new file in bob dir" >raw/NewFile2.txt
    rm raw/src/raw/MOV001.mp4

    # Add a file to local git, and change a file in local git
    echo "Add a file to git area, in bob dir" >doc/NewFileFromBob.txt
    git add doc/NewFileFromBob.txt >/dev/null 2>&1
    ##git status
    git commit -am Added >/dev/null 2>&1

    # Push changes to remote ($gpRemoteRawDir)
    $gpBin/git-proj-push -b -y >/dev/null 2>&1

    # --------
    # Now test "git proj pull" command and functions, from this user
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    HOME=$cDatHome
    fComGetProjGlobals

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

    assertTrue "$LINENO $cDatHome2/$cDatProj1/raw/NewFile2.txt" "[ -f $cDatHome2/$cDatProj1/raw/NewFile2.txt ]"
    assertTrue "$LINENO $gpRemoteRawDir/NewFile2.txt" "[ -f $gpRemoteRawDir/NewFile2.txt ]"

    assertTrue "$LINENO $cDatHome2/$cDatProj1/raw/src/raw/MOV001.mp4" "[ ! -f $cDatHome2/$cDatProj1/raw/src/raw/MOV001.mp4 ]"
    assertTrue "$LINENO $gpRemoteRawDir/raw/src/raw/MOV001.mp4" "[ ! -f $gpRemoteRawDir/src/raw/MOV001.mp4 ]"

    assertTrue "$LINENO $cDatHome2/$cDatProj1/doc/NewFileFromBob.txt" "[ -f $cDatHome2/$cDatProj1/doc/NewFileFromBob.txt ]"

    return 0
} # testSetUp

# --------------------------------
testPullRawFiles()
{
    local tResult

    gpVerbose=2

    # Setup a change in raw files in $cDatHome/$cDatProj1
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1

    # Pull the changes from remote

    # 1) QUIT
    tResult=$(fPullRawFiles 2>&1 < <(echo -e "1\n"))
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertContains "$LINENO $tResult" "$tResult" "src/raw: MOV001.mp4"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # 2) HELP
    tResult=$(fPullRawFiles 2>&1 < <(echo -e "2\n1\n"))
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "if the above differences look OK"
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertContains "$LINENO $tResult" "$tResult" "DRY RUN"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    # 4) No-skip-pulling
    tResult=$(fPullRawFiles 2>&1 < <(echo -e "4\n"))
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Nothing was pulled"
    assertContains "$LINENO $tResult" "$tResult" "DRY RUN"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    # 3) Yes-pull-these-files
    tResult=$(fPullRawFiles 2>&1 < <(echo -e "3\n"))
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    tResult=$(fPullRawFiles 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There are no differences found with 'raw' files"

    return 0
} # testPullRawFiles

# --------------------------------
testPullGit()
{
    local tResult

    gpVerbose=2

    # diff -rq /home/john/project/george home/bob/project/george | grep -Ev '.git|raw'
    # Only in /home/bob/project/george/doc: NewFileFromBob.txt
    # Files /home/john/project/george/README.html and /home/bob/project/george/README.html differ

    tResult=$(fPullGit 0 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "git pull origin develop"

    tResult=$(fPullGit 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "git pull origin develop"

    ##git status
    ##git diff

    tResult=$(fPullGit 1 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    assertTrue "$LINENO" "grep 'Make a change' README.html"
    assertContains "$LINENO $tResult" "$tResult" "git pull origin develop"

    # TBD: what if a current local branch has no remote branch?
    # TBD: what if the current branch has files that have not been committed?
    # TBD: merge conflicts with changed local files?

    return 0
} # testPullGit

# --------------------------------
testPullFromOrigin()
{
    local tResult

    gpVerbose=2

    tResult=$(fPullFromOrigin 1 2>&1 < <(echo -e 3))
    assertTrue "$LINENO $tResult" "$?"
    # raw
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    # git
    assertTrue "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    assertTrue "$LINENO" "grep 'Make a change' README.html"
    assertContains "$LINENO $tResult" "$tResult" "git pull origin develop"
    # debug
    ##assertContains "$LINENO $tResult" "$tResult" "xxxDisable-this-if-OK"

    return 0
} # testPullFromOrigin

# --------------------------------
testGitProjPullCLI()
{
    local tResult

    tResult=$($gpBin/git-proj-pull -b -vv -y 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    # raw
    assertContains "$LINENO $tResult" "$tResult" "NewFile2.txt"
    assertTrue "$LINENO" "[ -f raw/NewFile2.txt ]"
    assertTrue "$LINENO" "[ ! -f raw/src/raw/MOV001.mp4 ]"
    assertContains "$LINENO $tResult" "$tResult" "total size is"
    # git
    assertTrue "$LINENO" "[ -f doc/NewFileFromBob.txt ]"
    assertTrue "$LINENO" "grep 'Make a change' README.html"
    assertContains "$LINENO $tResult" "$tResult" "git pull origin develop"
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
