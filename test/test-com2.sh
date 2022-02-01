#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-com2.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-com.inc

test-com2.inc - Common functions used in the test scripts

=head1 SYNOPSIS

    ./test.com2.sh [test,test,...]

=head1 DESCRIPTION

test-com2.sh uses test-env.tgz to create a test environment with a
HOME defined, so that existing config files in a real HOME directory
are not affected. (This was derived from from setUp in test-init.sh)

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
    unset cGetOrigin cGetTopDir cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpHardLink gpLocalRawDir \
        gpLocalRawDirPat gpLocalRawSymLink gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    local tTarIn=$gpTest/test-env_ProjLocalDefined.tgz

    fTestSetupEnv
    fTestCreateEnv

    cd $HOME >/dev/null 2>&1
    if [ ! -r $tTarIn ]; then
        echo "Missing: $tTarIn [$LINENO]" 1>&2
	exit 1
    fi
    tar -xzf $tTarIn

    cp $gpDoc/config/gitconfig /tmp/gitconfig.sav

    gpVerbose=3
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
    # fTestRmEnv
    cp /tmp/gitconfig.sav $gpDoc/config/gitconfig
    gpUnitDebug=0
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
    cd $gpTest >/dev/null 2>&1
    return 0
} # tearDown

# ========================================

fLookFor()
{
    local pVar=$1
    local pFile=$2

    if ! grep -q '\[com "test"\]' $pFile; then
        return 1
    fi
    timeout 2s grep -q "$pVar = defined" $pFile
    return $?
} # fLookFor

# --------------------------------
testComSetConfigMore()
{
    local tResult

    # Files:
    # Relative to $HOME ($cDatHome)
    #     1) -g ~/.gitconfig (include.path = .gitproj.config.global)
    # Relative to $HOME/$cDatProj1 ($cDatHome/$cDatProj1)
    #     2) -l .git/config
    #     X) -L .gitproj - with clone, updates --local
    #        after that it is updated from --local

    # ----------
    tResult=$(fComSetConfig -g -k com.test.gvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor gvar $HOME/.gitconfig"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -l -k com.test.lvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor lvar .git/config"

    # ----------
    cd $HOME
    tResult=$(fComSetConfig -l -k com.test.lvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "err: Error: You must be in a git workspace for this command"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -L -k com.test.Lvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor Lvar .gitproj"

    # ----------
    cd $HOME
    tResult=$(fComSetConfig -L -k com.test.Lvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "err: Error: You must be in a git workspace for this command"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -l -k com.test.lvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor lvar $c .git/config"

    # ----------
    cd $HOME
    tResult=$(fComSetConfig -l -k com.test.Hvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "err: Error: You must be in a git workspace for this command"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -f $HOME/test-config -k com.test.fvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor fvar $HOME/test-config"
    assertContains "$LINENO" "$tResult" "warning"
    assertContains "$LINENO" "$tResult" "created"

    # ----------
    cd $HOME/$cDatProj1
    mkdir $HOME/readonly
    chmod a-w $HOME/readonly
    tResult=$(fComSetConfig -f $HOME/readonly/test-fail -k com.test.fvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "[ ! -f $HOME/readonly/test-fail ]"
    assertContains "$LINENO" "$tResult" "warning"
    assertContains "$LINENO" "$tResult" "created"
    assertContains "$LINENO" "$tResult" "Could not create"

    # ----------
    tResult=$(fComSetConfig -g -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "fComSetConfig: missing key"
    assertContains "$LINENO $tResult" "$tResult" "err: Internal: Error: fComSetConfig: missing key"
    assertContains "$LINENO $tResult" "$tResult" "Stack trace"

    # ----------
    tResult=$(fComSetConfig -g -k com.test.gvar "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "fComSetConfig: missing value"

    # ----------
    tResult=$(fComSetConfig -g -k com.test.gvar -v 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Value required for option"

    # ----------
    tResult=$(fComSetConfig -W -g -k com.test.gvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "fComSetConfig:: Unknown option"

    # ----------
    #rm $HOME/$c ConfigGlobal
    tResult=$(fComGetConfig -e -g -k com.test.Xvar 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: Error: Unexpected: com.test.Xvar is not defined!"

    return 0
} # testComSetConfigMore

# --------------------------------
testComConfigCopy()
{
    local tResult
    local tSrc=$gpDoc/config/gitconfig
    local tDstDir=$HOME/project/test
    local tDst=$HOME/project/test/config.test

    # fComConfigCopy [-f] [-s pSrc] [-d pDst] [-i pInclPat] [-e pExclPat]

    tResult=$(fComConfigCopy 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "is missing or not readable"

    tResult=$(fComConfigCopy -s foo 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "foo is missing or not readable"

    tResult=$(fComConfigCopy -s $tSrc 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "is missing or not writable"

    tResult=$(fComConfigCopy -s $tSrc -d $tDst 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "$tDst is missing or not writable"

    mkdir $tDstDir
    touch $tDst

    tResult=$(fComConfigCopy -s $tSrc -d $tDst 2>&1)
    assertTrue "$LINENO" "$?"
    assertTrue "$LINENO" "[ -f $tDst.bak ]"
    assertTrue "$LINENO $(diff -ibBwZ $tSrc $tDst)" "diff -qibBwZ $tSrc $tDst"

} # testComConfigCopy

testComConfigUpdateLocal()
{
    local tResult
    local tValue
    local tGlobal=$gpDoc/config/gitconfig
    local tGitProj=$HOME/$cDatProj1/.gitproj
    local tLocal=$HOME/$cDatProj1/.git/config

    cd $HOME
    tResult=$(fComConfigUpdateLocal 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "fComConfigUpdateLocal"

    cd $HOME/$cDatProj1
    mv .gitproj .gitproj.sav
    tResult=$(fComConfigUpdateLocal 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "fComConfigUpdateLocal"
    sleep 1

    cd $HOME/$cDatProj1
    # Restore
    mv .gitproj.sav .gitproj
    # Clean
    rm .gitproj.bak* 2>/dev/null
    # Change
    fComSetConfig -l -k "gitproj.config.verbose" -v 3
    # Check
    tValue=$(fComGetConfig -L -k "gitproj.config.verbose")
    assertEquals "$LINENO" "2" "$tValue"
    # Test
    tResult=$(fComConfigUpdateLocal 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO $tResult" "[ -f .gitproj.bak ]"
    assertContains "$LINENO $tResult" "$tResult" "1 file changed"
    tValue=$(fComGetConfig -L -k "gitproj.config.verbose")
    assertEquals "$LINENO" "3" "$tValue"
    sleep 1

    cd $HOME/$cDatProj1
    tResult=$(fComConfigUpdateLocal 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO $tResult $(echo ; stat  -c %y ./.git/config ./.gitproj )" "[ ./.git/config -ot ./.gitproj ]"
    assertNull "$LINENO $tResult $(echo ; stat  -c %y ./.git/config ./.gitproj )" "$tResult"
    ##assertContains "$LINENO $tResult $(echo ; stat  -c %y ./.git/config ./.gitproj )" "$tResult" "uncomment to show"

    return 0
} # testComConfigUpdateLocal

# --------------------------------
testComGetConfigMore()
{
    startSkipping
    fail "TODO"
    # untar a git env., test in and out of git dir
    return 0
} # testComGetConfigMore

# --------------------------------
testComUnsetConfigMore()
{
    startSkipping
    fail "TODO"
    return 0
} #testComUnsetConfigMore

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
