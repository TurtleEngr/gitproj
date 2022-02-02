#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-infra.sh

    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-infra.sh

test-infra.sh - test the functions in test.inc

=head1 SYNOPSIS

    ./test.infra.sh [test,test,...]

=for comment =head1 DESCRIPTION

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
}

# ========================================

# --------------------------------
setUp()
{
    # Restore default global values, before each test
    unset gpBin cPID gpCmdVer gErr gpFacility gpSysLog gpVerbose
    fTestSetupEnv
    fTestCreateEnv

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
    git config --global --remove-section gitproj.testit >/dev/null 2>&1
    gpUnitDebug=0
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
    return 0
} # tearDown

# ========================================

# --------------------------------
testSetup()
{
    assertTrue "$LINENO" "[ -x $gpBin/gitproj-com.inc ]"
    assertTrue "$LINENO" "[ -x $gpBin/git-proj ]"

    assertTrue "$LINENO" "[ -r $cTestFiles ]"
    assertTrue "$LINENO" "[ -d $cTestSrcDir ]"
    assertTrue "$LINENO" "[ -d $cTestDestDir ]"
    assertNotEquals "$LINENO" "$cHome" "$HOME"
    assertFalse "$LINENO" "[ -r $HOME/.gitconfig ]"

    for i in $cDatMount1 $cDatMount2 $cDatMount3; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i##*/}" "[ -d $i ]"
    done
    for i in $cDatProj1 $cDatProj2; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -d $HOME/$i ]"
    done
    for i in $cDatProj1Big; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -r $HOME/$cDatProj1/$i ]"
    done
    for i in $cDatProj2Big $cDatProj2Big; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -r $HOME/$cDatProj2/$i ]"
    done
} # testSetup

# ========================================
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

gpCmdName=test-com.sh

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
