#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-com2.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc()
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

NAoneTimeTearDown()
{
    return 0
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
} # oneTimeTearDown

# --------------------------------
setUp()
{
    unset cConfigGlobal cConfigLocal cCurDir cGetOrigin cGetTopDir \
        cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpHardLink gpLocalRawDir \
        gpLocalRawDirPat gpLocalRawSymLink gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    fTestSetupEnv
    fTestCreateEnv

    cd $HOME >/dev/null 2>&1
    tar -xzf $gpTest/test-env_ProjLocalDefined.tgz

    cp $gpDoc/config/gitconfig $gpDoc/config/gitconfig.sav

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
    gpUnitDebug=0
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
    cd $gpTest >/dev/null 2>&1
    mv $gpDoc/config/gitconfig.sav $gpDoc/config/gitconfig
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
    #	  2) -G $HOME/$cConfigGlobal ($HOME/.gitproj.config.global)
    # Relative to $HOME/$cDatProj1 ($cDatHome/$cDatProj1)
    #     3) -l .git/config (include.path = ../.gitproj.config.HOSTNAME)
    #	  4) -L .gitproj.config.local ($cConfigLocal)
    #	  5) -H $cConfigLocal ($cConfigHost,
    #	  	 	    include-path=$cConfigLocal)

    gpVerbose=2

    # ----------
    tResult=$(fComSetConfig -g -k com.test.gvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor gvar $HOME/.gitconfig"

    # ----------
    tResult=$(fComSetConfig -G -k com.test.Gvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor Gvar $HOME/$cConfigGlobal"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -l -k com.test.lvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor lvar .git/config"

    # ----------
    cd $HOME
    tResult=$(fComSetConfig -l -k com.test.lvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "You are not in a git dir"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -L -k com.test.Lvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor Lvar $cConfigLocal"

    # ----------
    cd $HOME
    tResult=$(fComSetConfig -L -k com.test.Lvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "You are not in a git dir"

    # ----------
    cd $HOME/$cDatProj1
    tResult=$(fComSetConfig -H -k com.test.Hvar -v "defined" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO" "fLookFor Hvar $cConfigHost"

    # ----------
    cd $HOME
    tResult=$(fComSetConfig -H -k com.test.Hvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "You are not in a git dir"

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
    assertContains "$LINENO $tResult" "$tResult" "crit: Internal:"
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
    rm $HOME/$cConfigGlobal
    tResult=$(fComSetConfig -G -k com.test.Gvar -v "defined" 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Could not find"

    return 0
} # testComSetConfigMore

# --------------------------------
testComConfigSetupGlobal()
{
    local tResult
    local tSrc=$gpDoc/config/gitconfig
    local tDst=$HOME/.gitconfig
    local tKey
    local tValue
    local tList
    local tKeyList

    # fComConfigSetupGlobal [source] [dest]
    #    if no dest
    # 	     cp source dest
    #	     return 0
    # 	 if dest
    #        cp --backup=t dest dest.bak
    #        cp-content source dest - (only copy if var not set in dest)
    # Note these gitproj.config vars should always be TBD:
    # local-status, proj-name, remote-status, remote-raw-dir
    # In UI, after call, do a "diff dist.bak dist"

    #----------
    # tDst file is missing so just cp tSrc
    assertTrue "$LINENO $tSrc" "[ -f $tSrc ]"
    assertTrue "$LINENO $tDst" "[ -f $tDst ]"
    rm $tDst
    assertTrue "$LINENO $tDst" "[ ! -f $tDst ]"

    tResult=$(fComConfigSetupGlobal $tSrc $tDst 2>&1)
    assertTrue "$LINENO $tResult $?" "$?"
    assertTrue "$LINENO $tDst" "[ -f $tDst ]"

    #----------
    # Missing values in tDst file s/b replaced from tSrc
    assertTrue "$LINENO $tDst" "[ -f $tDst ]"
    git config -f $tDst --unset alias.br
    git config -f $tDst --unset alias.st
    assertFalse "$LINENO br" "grep 'br = branch' $tDst"
    assertFalse "$LINENO st" "grep 'st = status' $tDst"

    tResult=$(fComConfigSetupGlobal $tSrc $tDst 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO $tResult br" "grep 'br = branch' $tDst"
    assertTrue "$LINENO st" "grep 'st = status' $tDst"

    #----------
    # Values in tList s/b TBD in tDst file
    tList="local-status proj-name remote-status remote-raw-dir"
    tKeyList=""
    for tKey in $tList; do
        git config -f $tDst gitproj.config.$tKey false
	tKeyList="$tKeyList gitproj.config.$tKey"
    done

    tResult=$(fComConfigSetupGlobal $tSrc $tDst "$tKeyList" 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    for tVar in $tList; do
        tValue=$(git config -f $tDst --get gitproj.config.$tKey)
	assertTrue "$LINENO $tVar" "$?"
	assertEquals "$LINENO $tVar" "TBD" "$tValue"
    done

    return 0
} # testComConfigSetupGlobal

testComConfigSetupLocal()
{
    local tResult
    local tSrc=$HOME/gitconfig
    local tDst1=$HOME/$cDatProj1/.gitproj
    local tDst2=$HOME/$cDatProj1/.git/config

    # PROJ = $HOME/$cDatProj1

    # fComConfigSetupLocal [force] [source] [dest-1] [dest-2]
    #    force  = 0|1
    #    source = $HOME/gitconfig
    #    dest-1   = PROJ/.gitproj
    #    dest-2   = PROJ/.git/config
    # 	 if not in a gitdir
    #        return 1
    #    if no dest-1
    #	     cp-content source dest1
    # 	     * copy [gitproj] sections
    #    cp --backup=t dest-2 dest-2.bak
    #    if force=1
    #	     cp-force dest-1 dest-2
    #	     * copy all vars from dest-1, but
    #	     * skip remote-raw-dir, if set in dist-2
    #    else
    #	     cp-content dest-1 dest-2
    #	     * copy all vars from dest-1, where dest-2 var not set
    #    In UI, do a "diff dist-2.bak dist-2"

    cp $gpDoc/config/gitconfig $tSrc

    return 1
} # testComConfigSetupLocal

testfComConfigUpdateLocal()
{
    local tResult

    # PROJ = $HOME/$cDatProj1

    # fComConfigUpdateLocal [source] [dest]
    #	 source = PROJ/.git/config
    # 	 dest   = PROJ/.gitproj
    #    cp --backup=t dest dest.bak
    #    cp-force source dest
    # 	 * only copy the gitproj sections
    # In UI "diff dist.bak dist"

    startSkipping

} # testfComConfigUpdateLocal

# --------------------------------
testComGetConfigMore()
{
    startSkipping
    fail "TBD"
    # untar a git env., test in and out of git dir
} # testComGetConfigMore

# --------------------------------
testComUnsetConfigMore()
{
    startSkipping
    fail "TBD"
    # untar a git env., test in and out of git dir
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
