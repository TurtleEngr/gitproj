#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-gitproj.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc()
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 NAME test-pre-commit.sh

Test the pre-commit hook

=head1 SYNOPSIS

    ./test-pre-commit.sh [test,test,...]

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
NAoneTimeSetUp()
{
    return 1
} # oneTimeSetUp

NAoneTimeTearDown()
{
    return 1
} # oneTimeTearDown

# --------------------------------
setUp()
{
    local tTar1=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz
    local tTar2=$gpTest/test-env_Home3AfterCloneSummary.tgz
    # Restore default global values, before each test

    unset cConfigGlobal cConfigLocal cCurDir cGetOrigin cGetTopDir \
        cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    HOSTNAME=testserver2
    fTestSetupEnv
    fTestCreateEnv

    mkdir -p $cDatHome3/project >/dev/null 2>&1
    cd $cTestDestDir >/dev/null 2>&1
    if [ ! -r $tTar1 ]; then
        echo "Could not find: $tTar1 [$LINENO]"
        exit 1
    fi
    tar -xzf $tTar1
    cd - >/dev/null 2>&1

    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTar2 ]; then
        echo "Could not find: $tTar2 [$LINENO]"
        exit 1
    fi
    tar -xzf $tTar2

    # git proj to be cloned:
    HOME=$cDatHome3
    gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git

    cd $HOME/project/george >/dev/null 2>&1
    fComGetProjGlobals

    cp $gpDoc/hooks/pre-commit .git/hooks

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
    assertTrue "$LINENO" "[ -f $HOME/.gitproj.config.global ]"
    assertTrue "$LINENO" "[ -f .git/config ]"
    assertTrue "$LINENO" "[ -f .gitproj.config.local ]"
    assertTrue "$LINENO" "[ -f .gitproj.config.$HOSTNAME ]"
    assertTrue "$LINENO" "[ -d raw ]"
    tOrigin=$($cGetOrigin)
    assertTrue "$LINENO" $?
    assertTrue "$LINENO $tOrigin" "[ -d $tOrigin ]"
    gpRemoteRawDir=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "None")
    assertNotContains "$LINENO" "$gpRemoteRawDir" "None"
    assertTrue "$LINENO" "[ -d $gpRemoteRawDir ]"

    return 0
} # testSetup

# --------------------------------
testCheckFileNames()
{
    local tResult

    cd $HOME/project/george >/dev/null 2>&1
    echo "illegal char" >foo:bar.txt
    echo "illegal char" >bar,foo.txt
    echo "illegal char" >"bar foo.txt"
    echo "reserved woord" >CON
    echo "reserved woord" >doc/CON
    echo "trailing period" >testing.
    echo "just periods" >....

    git add -- foo:bar.txt bar,foo.txt "bar foo.txt" CON doc/CON testing. ....
    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "A  foo:bar.txt"
    assertContains "$LINENO" "$tResult" "A  bar,foo.txt"
    assertContains "$LINENO" "$tResult" 'A  "bar foo.txt"'
    assertContains "$LINENO" "$tResult" "A  CON"
    assertContains "$LINENO" "$tResult" "A  doc/CON"
    assertContains "$LINENO" "$tResult" "A  testing."
    assertContains "$LINENO" "$tResult" "A  ...."
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git commit -m "Testing" 2>&1)
    assertFalse $LINENO $?
    assertContains "$LINENO" "$tResult" "foo:bar.txt has illegal chars in name"
    assertContains "$LINENO" "$tResult" "bar,foo.txt has illegal chars in name"
    assertContains "$LINENO" "$tResult" "bar!foo.txt has illegal chars in name"
    assertContains "$LINENO" "$tResult" "CON has reserved words in name"
    assertContains "$LINENO" "$tResult" "doc/CON has reserved words in name"
    assertContains "$LINENO" "$tResult" "testing. has trailing period"
    assertContains "$LINENO" "$tResult" ".... has just periods in name"
    assertContains "$LINENO $tResult" "$tResult" "These files are not valid:"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "?? foo:bar.txt"
    assertContains "$LINENO" "$tResult" "?? bar,foo.txt"
#    assertContains "$LINENO $(ls)" "$tResult" '?? "bar foo.txt"'
    assertContains "$LINENO" "$tResult" "?? CON"
    assertContains "$LINENO" "$tResult" "?? doc/CON"
    assertContains "$LINENO" "$tResult" "?? testing."
    assertContains "$LINENO" "$tResult" "?? ...."
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    return 0
} # testCheckFileNames

# --------------------------------
testCheckWhiteSpace()
{
    local tResult

    # Make some files with trailing whitespace and "add" them
    cd $HOME/project/george >/dev/null 2>&1
    cat <<EOF >test-wsp.txt
Testing whitespace

lots of extra
     jsjdfs
     
EOF
    cp test-wsp.txt test2-wsp.txt
    git add test-wsp.txt test2-wsp.txt

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "A  test-wsp.txt"
    assertContains "$LINENO" "$tResult" "A  test2-wsp.txt"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git commit -m "Testing" 2>&1)
    assertFalse $LINENO $?
    assertContains "$LINENO" "$tResult" "test-wsp.txt trailing whitespace"
    assertContains "$LINENO" "$tResult" "test2-wsp.txt trailing whitespace"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "?? test-wsp.txt"
    assertContains "$LINENO" "$tResult" "?? test2-wsp.txt"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    # ----------
    # Remove the trailing space, not add and check
    $gpTest/rm-trailing-sp test-wsp.txt test2-wsp.txt
    git add test-wsp.txt test2-wsp.txt

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "A  test-wsp.txt"
    assertContains "$LINENO" "$tResult" "A  test2-wsp.txt"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git commit -am "Testing OK" 2>&1)
    assertTrue $LINENO $?
    assertNotContains "$LINENO" "$tResult" "test-wsp.txt trailing whitespace"
    assertNotContains "$LINENO" "$tResult" "test2-wsp.txt trailing whitespace"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git log test-wsp.txt 2>&1 | head -n 10)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "Testing OK"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    return 0
} # testCheckWhiteSpace

# --------------------------------
testCheckNotRaw()
{
    local tResult

    # Put some files in raw/ dir
    cd $HOME/project/george >/dev/null 2>&1
    echo "test1" >raw/test1.txt
    echo "test2" >raw/test2.txt
    echo "test3" >raw/src/test3.txt
    git add -f raw/test1.txt raw/test2.txt raw/src/test3.txt

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "A  raw/test1.txt"
    assertContains "$LINENO" "$tResult" "A  raw/test2.txt"
    assertContains "$LINENO" "$tResult" "A  raw/src/test3.txt"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git commit -m "Testing" 2>&1)
    assertFalse $LINENO $?
    assertContains "$LINENO" "$tResult" "raw/test1.txt Do not commit files in raw/"
    assertContains "$LINENO" "$tResult" "raw/test2.txt Do not commit files in raw/"
    assertContains "$LINENO" "$tResult" "raw/src/test3.txt Do not commit files in raw/"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git status -s --ignored 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "!! raw"
    assertNotContains "$LINENO" "$tResult" "raw/test1.txt"
    assertNotContains "$LINENO" "$tResult" "raw/test2.txt"
    assertNotContains "$LINENO" "$tResult" "raw/src/test3.txt"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    return 0
} # testCheckNotRaw

# --------------------------------
testCheckBigFiles()
{
    local tResult

    # Create some big binary files
    cd $HOME/project/george >/dev/null 2>&1
    ln raw/src/raw/MOV001.mp4 src/final/test-MOV001.mp4
    cp raw/src/raw/MOV001.MP3 edit/test-MOV001.MP3
    cp raw/src/final/george.mp4 test-george.mp4
    git add src/final/test-MOV001.mp4 edit/test-MOV001.MP3 test-george.mp4

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "A  src/final/test-MOV001.mp4"
    assertContains "$LINENO" "$tResult" "A  edit/test-MOV001.MP3"
    assertContains "$LINENO" "$tResult" "A  test-george.mp4"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git commit -m "Testing" 2>&1)
    assertFalse $LINENO $?
    assertContains "$LINENO" "$tResult" "edit/test-MOV001.MP3 size 15360 > 10240"
    assertContains "$LINENO" "$tResult" "src/final/test-MOV001.mp4 size 20480 > 10240"
    assertContains "$LINENO" "$tResult" "test-george.mp4 size 1048576 > 10240"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    tResult=$(git status -s 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "?? edit/test-MOV001.MP3"
    assertContains "$LINENO" "$tResult" "?? src/final/test-MOV001.mp4"
    assertContains "$LINENO" "$tResult" "?? test-george.mp4"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    # Disable the big file chack and add the files
    git config --bool gitproj.hook.check-for-big-files false
    git add src/final/test-MOV001.mp4 edit/test-MOV001.MP3 test-george.mp4

    tResult=$(git commit -m "Testing" 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO" "$tResult" "create mode 100644 edit/test-MOV001.MP3"
    assertContains "$LINENO" "$tResult" "create mode 100644 src/final/test-MOV001.mp4"
    assertContains "$LINENO" "$tResult" "create mode 100644 test-george.mp4"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to view result"

    return 0
} # testCheckBigFiles

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
