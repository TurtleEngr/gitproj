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
    fRemoteSetGlobals
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
testComGetVer()
{
    local tResult

    gpDebug=2
    gpVerbose=2

    cd $cDatHome >/dev/null 2>&1
    tResult=$(fComGetVer 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "You must be in a git workspace for this command"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1

    cGitProjVersion="0.1.0"
    fComSetConfig -H -k "gitproj.config.ver" -v "0.1.0"
    fComGetVer 2>&1
    assertTrue "$LINENO" "$?"
    assertEquals "$LINENO" "$cGitProjVersion" "$gpVer"

    cGitProjVersion="0.1.8"
    fComSetConfig -H -k "gitproj.config.ver" -v "0.1.0"
    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO" "$cGitProjVersion" "${gpVer%.*}"

    cGitProjVersion="1.1.4+120"
    fComSetConfig -H -k "gitproj.config.ver" -v "2.3.1-rc+28"
    tResult=$(fComGetVer 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: "
    assertContains "$LINENO" "$tResult" "The installed version $cGitProjVersion needs to be upgraded"

    cGitProjVersion="2.3.1-rc+28"
    fComSetConfig -H -k "gitproj.config.ver" -v "1.1.4+120"
    tResult=$(fComGetVer 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: "
    assertContains "$LINENO $tResult" "$tResult" "project needs to be upgraded"

    cGitProjVersion="1.2.7-rc.3+5"
    fComSetConfig -H -k "gitproj.config.ver" -v "1.3.0"
    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning: "
    assertContains "$LINENO $tResult" "$tResult" "Minor version difference. Expected version"

    cGitProjVersion="1.3.0"
    fComSetConfig -H -k "gitproj.config.ver" -v "1.2.7-rc.3+5"
    tResult=$(fComGetVer 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning: "
    assertContains "$LINENO $tResult" "$tResult" "Minor version difference. Expected version"

    fComUnsetConfig -L -k "gitproj.config.ver"
    fComUnsetConfig -H -k "gitproj.config.ver"
    tResult=$(fComGetVer 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning: "
    assertContains "$LINENO $tResult" "$tResult" "gitproj.config.ver was not found"

    cGitProjVersion=1.0.0
    fComGetVer >/dev/null 2>&1
    assertFalse "$LINENO" "$?"
    assertEquals "$LINENO" "$cGitProjVersion" "$gpVer"

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
testRemoteVerifyState()
{
    local tResult
    local tLocalTopDir

    gpLocalStatus=not-defined
    tResult=$(fRemoteVerifyState 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "It looks like 'git proj init' did not finish"

    gpLocalStatus=defined
    tLocalTopDir=$gpLocalTopDir
    gpLocalTopDir=$gpTest
    tResult=$(fRemoteVerifyState 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "This repo appears to be configured for"
    # rest for next test
    gpLocalTopDir=$tLocalTopDir

    gpRemoteStatus=defined
    gpAuto=0
    tResult=$(fRemoteVerifyState 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning"
    assertContains "$LINENO $tResult" "$tResult" "It looks like a remote has already been setup with"

    gpRemoteStatus=defined
    gpAuto=1
    tResult=$(fRemoteVerifyState 2>&1)
    tResult=$(fRemoteVerifyState 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning"
    assertContains "$LINENO $tResult" "$tResult" "It looks like a remote has already been setup with"
    assertContains "$LINENO $tResult" "$tResult" "You are in 'automatic' mode, so exiting"

    gpRemoteStatus=not-defined
    gpRemoteRawDir=foo
    tResult=$(fRemoteVerifyState 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Config problem remote-status=not-defined, but remote-raw-dir is set to"

    gpRemoteStatus=not-defined
    gpRemoteRawDir="TBD"
    tResult=$(fRemoteVerifyState 2>&1)
    assertTrue "$LINENO" "$?"

    gpRemoteStatus=not-defined
    gpRemoteRawDir=""
    tResult=$(fRemoteVerifyState 2>&1)
    assertTrue "$LINENO" "$?"

    return 0
} # testRemoteValidateState

# --------------------------------
testRemoteCheckDir()
{
    local tResult

    tResult=$(fRemoteCheckDir /tmp/foo 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Could not find"

    mkdir /tmp/foo
    chmod a-w /tmp/foo
    tResult=$(fRemoteCheckDir /tmp/foo 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "is not writable for you"

    chmod a+w /tmp/foo
    touch /tmp/foo/$gpProjName.git
    tResult=$(fRemoteCheckDir /tmp/foo 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" ".git already exists"

    rm /tmp/foo/$gpProjName.git
    touch /tmp/foo/$gpProjName.raw
    tResult=$(fRemoteCheckDir /tmp/foo 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" ".raw already exists"

    rm -rf /tmp/foo
    return 0
} # testRemoteCheckDir

# --------------------------------
testRemoteGetMountDirAuto()
{
    local tResult

    gpAuto=1
    tResult=$(fRemoteGetMountDir "" 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "The -d option is required with -a auto option"

    gpAuto=1
    tResult=$(fRemoteGetMountDir "/tmp/foo" 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Could not find: /tmp/foo"
    assertContains "$LINENO $tResult" "$tResult" "Cannot continue with -a mode"

    gpAuto=1
    gpRemoteRawDir=TBD
    if [ -n "$gpMountDir" ]; then
        fail "$LINENO gpMountDir is not empty: ${gpMountDir}"
    fi
    fRemoteGetMountDir "$cDatMount1"
    assertTrue $LINENO "$?"
    assertEquals "$LINENO" "$cDatMount1" "$gResponse"

    return 0
} # testRemoteGetMountDirAuto

# --------------------------------
testRemoteGetDirList()
{
    gpAuto=0
    fRemoteGetDirList "$cDatMount1"
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$gResponse" "$cDatMount1"

    fRemoteGetDirList "$cDatMount3"
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$gResponse" "$cDatMount3"
    assertContains "$LINENO $tResult" "$gResponse" "src"

    return 0
} # testRemoteGetDirList

# --------------------------------
testRemoteSelect()
{
    local tResult
    local tDirList
    local tPrompt
    local tHelp

    gpAuto=0
    fRemoteGetDirList "$cDatMount3"
    tDirList=$gResponse
    tPrompt="Select a mount point: "
    tHelp="Just select by number."
    tResult=$(fRemoteSelect "$pPrompt" "$tDirList" "$tHelp" 2>&1 < <(echo -e "1\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "QUIT"
    assertContains "$LINENO $tResult" "$tResult" "warning"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    tResult=$(fRemoteSelect "$pPrompt" "$tDirList" "$tHelp" 2>&1 < <(echo -e "2\n1\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "QUIT"
    assertContains "$LINENO $tResult" "$tResult" "Just select by number"

    fRemoteSelect "$pPrompt" "$tDirList" "$tHelp" 2>&1 < <(echo -e "6\n") >/dev/null 2>&1
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $gResponse" "$gResponse" "/mnt/usb-video/video-2019-11-26/dev"

    return 0
} # testRemoteSelect

testRemoteGetAnotherMountDir()
{
    local tMountDir
    local tResult

    # 2GB
    gpRemoteMinSpace=2147483648

    tMountDir=$cDatMount1
    tResult=$(fRemoteGetAnotherMountDir "$tMountDir" "2024" 2>&1 < <(echo -e "\nq\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO tResult" "$tResult" "Quitting"

    tResult=$(fRemoteGetAnotherMountDir "/tmp/foo" "2024" 2>&1 < <(echo -e "/tmp/foo\n/tmp/bar\nquit\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Could not find: /tmp/foo"
    assertContains "$LINENO $tResult" "$tResult" "Could not find: /tmp/bar"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpProjName=example
    tResult=$(fRemoteGetAnotherMountDir "/tmp/foo" "2024" 2>&1 < <(echo -e "$cDatMount3/video-2019-11-26\nquit\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "$gpProjName.git already exists"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    fRemoteGetAnotherMountDir "/tmp/foo" "2024" 2>&1 < <(echo -e "$cDatMount2\n4\n") >/dev/null
    assertTrue "$LINENO" "$?"
    assertEquals "$LINENO $gResponse" "$cDatMount2" "$gResponse"

    # 1000 terabytes 1,073,741,824,000,000
    gpRemoteMinSpace=1073741824000000
    tResult=$(fRemoteGetAnotherMountDir "/tmp/foo" "$gpRemoteMinSpace" 2>&1 < <(echo -e "$cDatMount3/video-2020-04-02\n\n"))
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There is not enough space"

    return 0
} # testRemoteGetAnotherMountDir

testRemoteGetMountDirManual()
{
    local tResult
    local tMountDir

    tAuto=0

    tResult=$(fRemoteGetMountDir "$cDatMount3" < <(echo -e "1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    tResult=$(fRemoteGetMountDir "" < <(echo -e "1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    tResult=$(fRemoteGetMountDir "" < <(echo -e "2\n1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Select by number, the location for the remote git and raw files"
    assertContains "$LINENO $tResult" "$tResult" "This is a list of dirs under the -d pMountDir"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    tResult=$(fRemoteGetMountDir "" < <(echo -e "xx\n10\n1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Invalid selection"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpProjName=example
    tResult=$(fRemoteGetMountDir "$cDatMount3" < <(echo -e "5\n1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "example.git already exists"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    tResult=$(fRemoteGetMountDir "$cDatMount3" < <(echo -e "2\n1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "This is a list of dirs under the -d pMountDir"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Select 3 to define another dir, return 1, echo OTHER
    fRemoteGetMountDir "$cDatMount3" < <(echo -e "3\n$cDatMount2\nq\n4\n") >/dev/null 2>&1
    assertTrue "$LINENO" "$?"
    assertEquals "$LINENO" "$cDatMount2" "$gResponse"

    # The selected mount dir checked space: enough/not-enough
    # 1000 terabytes 1,073,741,824,000,000
    gpRemoteMinSpace=1073741824000000

    return 0
} # testRemoteGetMountDirManual

# --------------------------------
testRemoteGetRemoteRawDir()
{
    local tResult

    gpMountDir=/tmp/foo
    gpProjName=bar
    mkdir $gpMountDir
    touch $gpMountDir/$gpProjName.raw
    tResult=$(fRemoteGetRemoteRawDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "raw already exists"
    assertContains "$LINENO $tResult" "$tResult" "Internal"

    rm $gpMountDir/$gpProjName.raw
    fRemoteGetRemoteRawDir
    assertTrue "$LINENO" "$?"
    assertEquals "$LINENO" "$gpMountDir/$gpProjName.raw" "$gResponse"
    rmdir $gpMountDir

    return 0
} # testRemoteGetRemoteRawDir

# --------------------------------
testRemoteMkRemote()
{
    local tResult

    gpAuto=1
    gpVerbose=2

    gpLocalTopDir=$cDatHome/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    gpProjName=george
    gpMountDir=$cDatMount3/video-2020-04-02
    gpRemoteRawDir=$gpMountDir/$gpProjName.raw

    # Mount dir has already been valided
    tResult=$(fRemoteMkRemote $cDatMount3/video-2020-04-02 2>&1)
    tStatus=$?
    assertTrue "$LINENO" "$tStatus"
    assertContains "$LINENO $tResult" "$tResult" "git clone to $gpMountDir"
    assertContains "$LINENO $tResult" "$tResult" "Cloning into bare repository 'george.git'"
    assertContains "$LINENO $tResult" "$tResult" "'rsync' -rlptz"
    assertContains "$LINENO $tResult" "$tResult" "/$gpProjName.raw"

    # ----------
    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of test env after fRemoteMkRemote is run."
        echo -e "\tRestore test-env_HomeAfterMkRemote.tgz relative to env cTestDestDir"
        cd $cTestDestDir >/dev/null 2>&1
        echo -en "\t"
        tar -cvzf $gpTest/test-env_TestDestDirAfterMkRemote.tgz test
        echo
    fi

    return 0
} # testRemoteMkRemote

# --------------------------------
testRemoteReport()
{
    local tResult

    cd $cTestDestDir >/dev/null 2>&1
    tar -xzf $gpTest/test-env_TestDestDirAfterMkRemote.tgz

    gpAuto=1
    gpVerbose=2
    gpLocalTopDir=$cDatHome/$cDatProj1
    gpProjName=george
    gpMountDir=$cDatMount3/video-2020-04-02
    gpRemoteRawDir=$gpMountDir/$gpProjName.raw

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fRemoteReport 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Be sure the disk is mounted and"

    return 0
} # testRemoteReport

# --------------------------------
testRemoteCreateRemoteGit()
{
    local tResult
    local tTopDir

    gpAuto=1
    gpMountDir=""
    tResult=$(fRemoteCreateRemoteGit "$gpMountDir" 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "The -d option is required with -a auto option"

    gpMountDir=$cDatMount3/video-2020-04-02

    cd $cTestDestDir >/dev/null 2>&1
    tResult=$(fRemoteCreateRemoteGit "$gpMountDir" 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: You must be in a git workspace for this command"

    tTopDir=$cDatHome/$cDatProj1
    cd $tTopDir >/dev/null 2>&1
    fComSetGlobals
    fRemoteSetGlobals
    assertEquals "$LINENO" "$gpLocalTopDir" "$tTopDir"

    gpMountDir=$cDatMount3/video-2020-04-02
    chmod a-w $gpMountDir
    tResult=$(fRemoteCreateRemoteGit "$gpMountDir" 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "video-2020-04-02 is not writable for you"

    chmod ug+w $gpMountDir
    tResult=$(fRemoteCreateRemoteGit "$gpMountDir" 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Remote origin is now set to:"
    assertContains "$LINENO $tResult" "$tResult" "$gpMountDir/$gpProjName.git"
    assertContains "$LINENO $tResult" "$tResult" "Be sure the disk is mounted and that"

    return 0
} # testRemoteCreateRemoteGit

# --------------------------------
testGitProjRemoteCLIAuto()
{
    local tResult
    local tTopDir
    local tMountDir=""

    # Not in a git dir
    cd $cTestDestDir >/dev/null 2>&1
    tResult=$($gpBin/git-proj-remote -a -d $PWD 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: You must be in a git workspace for this command"
    assertContains "$LINENO $tResult" "$tResult" "Usage:"

    # Not in a proj dir
    cd $cDatHome/$cDatProj3 >/dev/null 2>&1
    tResult=$($gpBin/git-proj-remote -a -d $PWD 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: This git workspace is not setup for gitproj, run"
    assertContains "$LINENO $tResult" "$tResult" "Usage:"

    # -d required
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    tResult=$($gpBin/git-proj-remote -a 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "git-proj-remote: crit: The -d option is required with -a auto option"
    assertContains "$LINENO $tResult" "$tResult" "Usage:"

    # Works
    tMountDir=$cDatMount3/video-2020-04-02
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    tResult=$($gpBin/git-proj-remote -a -d $tMountDir 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Cloning into bare repository 'george.git'"
    assertContains "$LINENO $tResult" "$tResult" "Remote origin is now set to:"
    assertContains "$LINENO $tResult" "$tResult" "$tMountDir/george.git"
    assertContains "$LINENO $tResult" "$tResult" "If the mount path is changed or you are on a different system"

    return 0
} # testGitProjRemoteCLIAuto

# --------------------------------
testGitProjRemoteCLIManual()
{
    local tResult
    local tTopDir
    local tMountDir=""

    # Usage
    tResult=$($gpBin/git-proj-remote -h 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "NAME git proj remote"
    assertContains "$LINENO $tResult" "$tResult" "SYNOPSIS"
    assertContains "$LINENO $tResult" "$tResult" "DESCRIPTION"

    # Start manual mode, then quit
    tResult=$($gpBin/git-proj-remote 2>&1 < <(echo -e "1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Select by number, the location for the remote git and raw files"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Start manual mode, then HELP, then QUIT
    tResult=$($gpBin/git-proj-remote 2>&1 < <(echo -e "2\n1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "This is a list of dirs under the"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Problem george.git is aleady there
    tMountDir=$cDatMount3
    # video-2019-11-26 is item #5
    mkdir $tMountDir/video-2019-11-26/george.git $tMountDir/video-2019-11-26/george.raw
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    tResult=$($gpBin/git-proj-remote -d $tMountDir 2>&1 < <(echo -e "5\n1\n") 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "george.git already exists"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    # Works
    tMountDir=$cDatMount3
    # video-2020-04-02 was item #9, before the two dirs were add above
    # video-2020-04-02 is item #11
    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    tResult=$($gpBin/git-proj-remote -d $tMountDir 2>&1 < <(echo -e "11\n") 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Cloning into bare repository 'george.git'"
    assertContains "$LINENO $tResult" "$tResult" "Remote origin is now set to:"
    assertContains "$LINENO $tResult" "$tResult" "$tMountDir/video-2020-04-02/george.git"
    assertContains "$LINENO $tResult" "$tResult" "If the mount path is changed or you are on a different system"

    return 0
} # testGitProjRemoteCLIManual

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
