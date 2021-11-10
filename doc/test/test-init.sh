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

    ./test-init.sh [all] [test,test,...]

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
    unset gpBin cCurDir cPID gpCmdVer gErr gpDebug gpFacility gpSysLog gpVerbose
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
    if [ $gpDebug -ne 0 ]; then
        fTestRmEnv
    fi
    gpUnitDebug=0
    cd $gpTest >/dev/null 2>&1
 return 0
} # tearDown

# ========================================

# --------------------------------
testGitProjInit()
{
    local tResult

    tResult=$($gpBin/git-proj-init 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'Usage'

    tResult=$($gpBin/git-proj-init -h)
    assertContains "$LINENO $tResult" "$tResult" 'DESCRIPTION'

    # git proj init [-l pDirPath] [-r] [-e pDirPath] [-h]

    cd $HOME/$cDatProj1 >/dev/null 2>&1
    assertFalse "$LINENO" "[ -d .git ]"
    cd - >/dev/null 2>&1
    return 0
} # testGitProjInit

testFirstTimeSet()
{
    assertFalse "$LINENO" "[ -f $HOME/.gitconfig ]"
    assertFalse "$LINENO" "[ -f $HOME/.gitproj.config.global ]"

    fInitFirstTimeSet
    assertTrue "$LINENO" "[ -f $HOME/.gitconfig ]"
    assertTrue "$LINENO" "[ -f $HOME/.gitproj.config.global ]"
    assertTrue "$LINENO" "$(grep -q path $HOME/.gitconfig; echo $?)"
    return 0
} # testFirstTimeSet

testIniitSetGlobals()
{
    assertTrue "$LINENO" "[ -d $HOME/$cDatProj1 ]"
    cd $HOME/$cDatProj1

    fInitSetGlobals
    assertEquals "$LINENO" "1.1"  "$gpVer"
    assertEquals "$LINENO" "true"  "$gpSysLog"
    assertEquals "$LINENO" "user" "$gpFacility"
    assertEquals "$LINENO" "0" 	  "$gpAuto"
    assertEquals "$LINENO" ".."   "$gpLocalRawDirPat"
    assertEquals "$LINENO" "raw" "$gpLocalRawSymLink"
    assertEquals "$LINENO" "${PWD##*/}" "$gpProjName"
    assertEquals "$LINENO" "${gpLocalRawDirPat}/${gpProjName}.raw" "$gpLocalRawDir"
    assertEquals "$LINENO" "1k" "$gpMaxSize"
    assertEquals "$LINENO" "0" 	"$gpGitFlow"
    assertNull "$LINENO" "$gpAction"

    cd - >/dev/null 2>&1
    return 0
} # testIniitSetGlobals

checkComMustNotBeInGit()
{
    local pFun=$1

    local tResult
    local tStatus

    local tResult
    local tStatus

    gpUnitDebug=0

    # This dir does not exist
    tResult=$($pFun $HOME/foo-bar 2>&1)
    tStatus=$?
    fTestDebug "Check: $HOME/foo-bar"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "does not exist"

    # Git is below this dir
    tResult=$($pFun $HOME 2>&1)
    tStatus=$?
    fTestDebug "Check: $HOME"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"

    # This is in a git dir
    tResult=$($pFun $HOME/$cDatProj3 2>&1)
    tStatus=$?
    fTestDebug "Check: $HOME/$cDatProj3"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"

    # This is in a git dir
    tResult=$($pFun $HOME/$cDatProj3/edit 2>&1)
    tStatus=$?
    fTestDebug "Check: $HOME/$cDatProj3/edit"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"

    # No git dir above or below this dir
    tResult=$($pFun $HOME/$cDatProj1 2>&1)
    tStatus=$?
    fTestDebug "Check: $HOME/$cDatProj1"
    assertTrue $LINENO $tStatus
    return 0
} # checkComMustNotBeInGit

testComMustNotBeInGit()
{
    gpUnitDebug=0
    checkComMustNotBeInGit fComMustNotBeInGit
    return 0
} # testComMustNotBeInGit

checkComAllMustBeReadable()
{
    local tResult
    local tStatus

    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus

    # Create a file and make it unreadable
    touch $HOME/foo
    chmod a-r $HOME/foo
    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable"

    # Create a dir and make it unreadable
    rm $HOME/foo
    mkdir $HOME/foo
    chmod a-r $HOME/foo
    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable"

    # A dir can not be listed
    chmod a+r,a-x $HOME/foo
    tResult=$(fComAllMustBeReadable $HOME 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "All directories must be executable"
    return 0
} # checkComAllMustBeReadable

testComAllMustBeReadable()
{
    gpUnitDebug=0
    checkComAllMustBeReadable fComAllMustBeReadable
    return 0
} # testComAllMustBeReadable

testInitGettingStarted()
{
    local tResult
    local tStatus

    gpUnitDebug=0

    cd $HOME/$cDatProj1
    gpAuto=1
    tResult=$(fInitGettingStarted)
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"

    gpAuto=0
    tResult=$(fInitGettingStarted 2>&1 < <(echo -e "\nx\ny"))
    tStatus=$?
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO $tResult" "$tResult" "is not valid"
    assertTrue $LINENO $tStatus

    tResult=$(fInitGettingStarted 2>&1 < <(echo -e "n"))
    tStatus=$?
    assertFalse $LINENO $tStatus

    cd - >/dev/null 2>&1
    return 0
} # testInitGettingStarted

testInitValidLocalPath()
{
    local tResult
    local tStatus

    gpUnitDebug=1

    checkComMustNotBeInGit fInitValidLocalPath
    checkComAllMustBeReadable fInitValidLocalPath

    tResult=$(fInitValidLocalPath  $HOME/$cDatProj1 2>&1)
    tStatus=$?
    gpUnitDebug=0
    fTestDebug "Check: $HOME/$cDatProj1"
    fTestDebug "tResult: $tResult"
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    # Called again, so that the global vars will be defined.
    fInitValidLocalPath  $HOME/$cDatProj1 >/dev/null 2>&1
    assertEquals $LINENO "$HOME/$cDatProj1" "$gpLocalTopDir"
    assertEquals $LINENO "${cDatProj1##*/}" "$gpProjName"
    return 0
} # testInitValidLocalPath

testInitGetLocalPath()
{
    local tResult
    local tStatus

    gpAuto=1
    # Auto

    # Auto fail
    tResult=$(fInitGetLocalPath $HOME/foo-bar 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "does not exist"

    # Auto success
    tResult=$(fInitGetLocalPath $HOME/$cDatProj1 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    gpAuto=0
    # No auto, prompt/response
    
    # Just Enter, i.e. defaultt. Look for success
    tResult=$(fInitGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "\n"))
    tStatus=$?
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Define the existing project directory"
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"


    # Enter "quit". Look for quit
    tResult=$(fInitGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "quit"))
    tStatus=$?
    gpUnitDebug=0
    fTestDebug "tResult=$tResult"
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Enter $HOME; $HOME/$cDatProj3; quit
    tResult=$(fInitGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "$HOME\n$HOME/$cDatProj3\nquit"))
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Enter $HOME; $HOME/$cDatProj1
    tResult=$(fInitGetLocalPath $HOME/$cDatProj1 2>&1 < <(echo -e "$HOME\n$HOME/$cDatProj1"))
    tStatus=$?
    assertTrue $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "project Name will be: ${cDatProj1##*/}"

    # Called again, so that the global vars will be defined.
    fInitGetLocalPath $HOME/$cDatProj1 < <(echo -e "$HOME\n$HOME/$cDatProj1") >/dev/null 2>&1
    assertTrue $LINENO $tStatus
    assertEquals $LINENO "$HOME/$cDatProj1" "$gpLocalTopDir"
    assertEquals $LINENO "${cDatProj1##*/}" "$gpProjName"
    return 0
} # testInitGetLocalPath

testInitValidLocalRawDirPat()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}

    tResult=$(fInitValidLocalRawDirPat ".." 2>&1)
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "README.txt will be updated"
    assertContains "$LINENO $tResult" "$tResult" "../${gpProjName}.raw"

    tResult=$(fInitValidLocalRawDirPat "doc" 2>&1)
    tStatus=$?
    assertFalse "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "works best if it is relative to"
    assertContains "$LINENO $tResult" "$tResult" "Raw directory cannot be in"

    tResult=$(fInitValidLocalRawDirPat "$HOME" 2>&1)
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "works best if it is relative to"

    tResult=$(fInitValidLocalRawDirPat "../foo-bar" 2>&1)
    tStatus=$?
    assertFalse "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "not found relative to"

    mkdir $gpLocalTopDir/../$gpProjName.raw
    tResult=$(fInitValidLocalRawDirPat ".." 2>&1)
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Raw directory already exists"
    assertContains "$LINENO $tResult" "$tResult" "README.txt will be updated"

    return 0
} # testInitValidLocalRawDirPat

testInitGetRawLocalDirPat()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}

    tResult=$(fInitGetLocalRawDirPat 2>&1< <(echo -e "\n"))
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Set the location for large binary files."
    assertContains "$LINENO $tResult" "$tResult" "../$gpProjName.raw will be created."
    assertContains "$LINENO $tResult" "$tResult" "README.txt will be updated"

    tResult=$(fInitGetLocalRawDirPat 2>&1< <(echo -e ".."))
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "../$gpProjName.raw will be created."
    assertContains "$LINENO $tResult" "$tResult" "README.txt will be updated"

    tResult=$(fInitGetLocalRawDirPat 2>&1 < <(echo -e "foo-bar\nquit"))
    tStatus=$?
    assertFalse "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "does not exist"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpAuto=1
    fInitGetLocalRawDirPat ".." >/dev/null 2>&1
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "true" "$gpHardLink"

    return 0
} # testInitGetRawLocalDirPat

testInitValidSymLink()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}

    tResult=$(fInitValidSymLink "doc" 2>&1)
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "already exists"

    tResult=$(fInitValidSymLink "raw" 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus

    fInitValidSymLink "raw"
    assertEquals "$LINENO" "raw" "$gpLocalRawSymLink"

    return 0
} # testInitValidSymLink

testInitGetSymLink()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}

    tResult=$(fInitGetSymLink 2>&1 < <(echo -e "doc\nquit"))
    tStatus=$?
    assertFalse $LINENO $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Define the symlink name that will point to the Raw dir"
    assertContains "$LINENO $tResult" "$tResult" "already exists"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    return 0
} # testInitGetSymLink

testInitValidSize()
{
    local tResult
    local tStatus
    declare -l tLower

    tResult=$(fInitValidSize "12K" 2>&1)
    tStatus=$?
    assertTrue $LINENO $tStatus

    fInitValidSize "12K"
    assertEquals $LINENO "12k" "$gpMaxSize"

    for i in 3b 34k 8m 2g 3B 34k 8M 2G; do
        fInitValidSize "$i"
	tLower=$i
        assertEquals "$LINENO $i" "$tLower" "$gpMaxSize"
    done

    for i in 3x k 8 2 K2; do
        tResult=$(fInitValidSize "$i" 2>&1)
    	tStatus=$?
    	assertFalse "$LINENO $i" $tStatus
    	assertContains "$LINENO $tResult" "$tResult" "Size must be numbers followed by"
    done

    return 0
} # testInitValidSize

testInitGetSize()
{
    local tResult
    local tStatus
    declare -l tLower

    tResult=$(fInitGetSize 2>&1 < <(echo -e "12K"))
    tStatus=$?
    assertTrue "$LINENO $i" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Define the size for large binary files"
    
    fInitGetSize >/dev/null 2>&1 < <(echo -e "12K")
    assertTrue $LINENO $?
    assertEquals "$LINENO $i" "12k" "$gpMaxSize"

    tResult=$(fInitGetSize 2>&1 < <(echo -e "42\n66x\nq"))
    tStatus=$?
    assertFalse "$LINENO $i" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Size must be numbers followed by"    
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    return 0
}

testInitGetBinaryFiles()
{
    local tResult
    local tStatus

    gpMaxSize="18k"
    gpLocalTopDir=$HOME/$cDatProj1

    tResult=$(fInitGetBinaryFiles 2>&1)
    tStatus=$?
    assertTrue "$LINENO $i" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "These binary files are greater than $gpMaxSize"
    assertContains "$LINENO $tResult" "$tResult" "src/final/george.mp4"
    assertContains "$LINENO $tResult" "$tResult" "src/raw/MOV001.mp4"
    assertNotContains "$LINENO $tResult" "$tResult" "src/raw/MOV001.mp3"

    return 0
} # testInitGetBinaryFiles

testInitGetMoveFiles()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1

    gpMaxSize="24m"
    tResult=$(fInitGetMoveFiles 2>&1 < <(echo -e "q"))
    tStatus=$?
    assertTrue "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "No large binary files were found"

    gpMaxSize="18k"
    tResult=$(fInitGetMoveFiles 2>&1 < <(echo -e "x\nquit"))
    tStatus=$?
    assertFalse "$LINENO" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "The listed files can be moved"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpMaxSize="18k"
    fInitGetMoveFiles 2>&1 < <(echo -e "x\nn")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "0" "$gpAutoMove"

    gpMaxSize="18k"
    fInitGetMoveFiles 2>&1 < <(echo -e "y")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "1" "$gpAutoMove"

    return 0
} # testInitGetMoveFiles

testInitGetGetFlow()
{
    local tResult
    local tStatus

    tResult=$(fInitGetGetFlow 2>&1 < <(echo -e "\nquit"))
    assertFalse "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "Quitting"
    
    fInitGetGetFlow >/dev/null 2>&1 < <(echo -e "yes")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "1" "$gpGitFlow"
    
    fInitGetGetFlow >/dev/null 2>&1 < <(echo -e "No")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "0" "$gpGitFlow"

    gpGitFlowPkg=foo-bar
    tResult=$(fInitGetGetFlow 2>&1 < <(echo -e "\n"))
    assertTrue "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "git-flow is not installed"

    return 0
} # testInitGetGetFlow

testInitSummary()
{
    local tResult
    local tStatus

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}
    gpHardLink="true"
    gpGitFlow=1
    gpMaxSize="1k"

    tResult=$(fInitSummary 2>&1)
    assertTrue "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "Summary"
    assertContains "$LINENO $tResult" "$tResult" "auto-move"

    gpUnitDebug=1
    fTestDebug "$tResult"

    return 0
} # testInitSummary

testInitMkGitDir()
{
    local tResult
    local tStatus
    local tTop

    cd $gpLocalTopDir >/dev/null 2>&1
    fInitFirstTimeSet

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}
    gpHardLink="true"
    gpGitFlow=1
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fInitMkGitDir 2>&1 < <(echo -e "n"))
    assertFalse $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fInitMkGitDir 2>&1 < <(echo -e "y"))
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Continue with creating"

    gpUnitDebug=1
    fTestDebug "$tResult"

    cd $gpLocalTopDir >/dev/null 2>&1

    assertTrue $LINENO "[ -d $gpLocalTopDir/.git ]"
    assertTrue $LINENO "[ -f $gpLocalTopDir/.gitignore ]"
    assertTrue $LINENO "grep -q core $gpLocalTopDir/.gitignore; echo $?"

    tTop=$($cGetTopDir)
    assertEquals $LINENO "$gpLocalTopDir" "$tTop"

    tResult=$(git branch 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "develop"
    assertContains "$LINENO $tResult" "$tResult" "main"

    return 0
} # testInitMkGitDir

testInitMkRaw()
{
    local tResult
    local tStatus

    startSkipping
    fail "TBD"
    return 0
} # testInitMkRaw

testInitCreateLocalGit()
{
    local tResult
    local tStatus

    startSkipping
    fail "TBD"
    return 0
} # testInitCreateLocalGit

# ========================================

testInitGetMountPath()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitGetMountPath

testInitGetRawRemotePath()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitGetRawRemotePath

testInitCheckPath()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitCheckPath

testInitCheckSpace()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitCheckSpace

testInitMkRemote()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitMkRaw

testInitReport()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitReport

testInitCreateRemoteGit()
{
    startSkipping
    fail "TBD"
    return 0
} # testInitCreateRemoteGit

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
cd $gpTest >/dev/null 2>&1
gpTest=$PWD
cd - >/dev/null 2>&1

# -----
# Optional input: a comma separated list of test function names
gpTestList="$*"

# -----
. $gpTest/test.inc
fTestCreateEnv
. $gpBin/gitproj-init.inc

# Look for serious setup errors
fTestConfigSetup

fTestRun $gpTestList
