#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-init.sh

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

    ./test.com.sh [all] [test,test,...]

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

$Revision: 1.2 $ $Date: 2021/09/08 01:39:35 $ GMT 

=cut

EOF
}

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

 $cBin/gitproj-com.inc
 fComSetGlobals

=internal-cut
EOF
} # oneTimeSetUp

NAoneTimeTearDown()
{
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
} # oneTimeTearDown

# --------------------------------
setUp()
{
    # Restore default global values, before each test
    unset cBin cCurDir cPID cVer gErr gpDebug gpFacility gpLog gpVerbose
    fTestSetupEnv
    fCreateTestEnv
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
    if [ $gpDebug -ne 0 ]; then
        fRmTestEnv
    fi
    gpUnitDebug=0
    return 0
} # tearDown

# ========================================

# --------------------------------
testGitProjInit()
{
    local tResult

    tResult=$($cBin/git-proj-init 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'Usage'

    tResult=$($cBin/git-proj-init -h)
    assertContains "$LINENO $tResult" "$tResult" 'DESCRIPTION'

    # git proj init [-l pDirPath] [-r] [-e pDirPath] [-h]

    cd $HOME/$cDatProj1
    assertFalse "$LINENO" "[ -d .git ]"
    tResult=$($cBin/git-proj-init -l $PWD)
    assertTrue "$LINENO" "[ -d .git ]"

    assertTrue "$LINENO" "[ -f .gitignore ]"

    # check for git-flow settings
    # git is already in project dir

    # git-flow not installed
} # testGitProjInit

# ====================
# This should be the last defined function
fTestRun()
{
    if [ ! -x $cTest/shunit2.1 ]; then
        echo "Error: Missing: $cTest/shunit2.1"
        exit 1
    fi
    shift $#
    if [ -z "$gpTest" ]; then
        # shellcheck disable=SC1091
        . $cTest/shunit2.1
        exit $?
    fi

    # shellcheck disable=SC1091
    . $cTest/shunit2.1 -- $gpTest
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

export cTest cTestCurDir gpTest cName

cName=${BASH_SOURCE##*/}

# -------------------
# Set current directory location in PWD and cTestCurDir
if [ -z "$PWD" ]; then
    PWD=$(pwd)
fi
cTestCurDir=$PWD

# -------------------
# Define the location of this script
cTest=${0%/*}
if [ "$cTesBin" = "." ]; then
    cTest=$PWD
fi
cd $cTest
cTest=$PWD
cd $cTestCurDir

# -----
# Optional input: a comma separated list of test function names
gpTest="$*"

# -----
. $cTest/test.inc
fCreateTestEnv
. $cBin/gitproj-init.inc
fSetupTestConfig

fTestRun $gpTest
