#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-clone.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-com.inc

test-com.inc - Common functions used in the test scripts

=head1 SYNOPSIS

    ./test-clone.sh [all] [test,test,...]

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
setUp()
{
    local tTarIn=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz

    # Restore default global values, before each test

    unset cGetOrigin cGetTopDir cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    HOSTNAME=testserver2
    fTestSetupEnv
    fTestCreateEnv

    mkdir -p $cDatHome3/project >/dev/null 2>&1
    cd $cTestDestDir >/dev/null 2>&1
    if [ ! -r $tTarIn ]; then
        echo "Could not find: $tTarIn [$LINENO]" 1>&2
        exit 1
    fi
    tar -xzf $tTarIn
    cd - >/dev/null 2>&1
    fTestPatchPath

    # git proj to be cloned:
    HOME=$cDatHome3
    gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git

    cd $HOME/project >/dev/null 2>&1
    . $gpBin/gitproj-clone.inc >/dev/null 2>&1
    gpRemoteRawOrigin=${gpRemoteGitDir%.git}.raw
    gpProjName=${gpRemoteGitDir##*/}
    gpProjName=${gpProjName%.git}

    find $gpRemoteGitDir $gpRemoteRawOrigin -exec chmod a+r {} \;
    find $gpRemoteGitDir $gpRemoteRawOrigin -exec chmod ug+w {} \;
    find $gpRemoteGitDir $gpRemoteRawOrigin -type d -exec chmod a+rx {} \;

    gpVerbose=3
    gpMaxLoop=5
    gpAuto=0
    gpYesNo=""
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
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
    cd $gpTest >/dev/null 2>&1
    return 0
} # tearDown

# ========================================

# --------------------------------
testGitProjCloneUsage()
{
    local tResult

    cd $HOME
    assertEquals "$LINENO" "$PWD" "$HOME"

    assertEquals "$LINENO gpRemoteGitDir" "$cDatMount3/video-2020-04-02/george.git" "$gpRemoteGitDir"
    assertEquals "$LINENO gpRemoteRawOrigin" "${gpRemoteGitDir%.git}.raw" "$gpRemoteRawOrigin"
    assertEquals "$LINENO gpProjName" "george" "$gpProjName"
    assertEquals "$LINENO HOSTNAME" "testserver2" "$HOSTNAME"

    tResult=$($gpBin/git-proj-clone 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'Usage'

    tResult=$($gpBin/git-proj-clone -h 2>&1)
    assertContains "$LINENO $tResult" "$tResult" 'DESCRIPTION'

    # git proj init [-l pDirPath] [-r] [-e pDirPath] [-h]

    cd $cDatHome3 >/dev/null 2>&1
    assertEquals "$LINENO" "$HOME" "$cDatHome3"
    assertFalse "$LINENO" "[ -d .git ]"
    cd - >/dev/null 2>&1

    return 0
} # testGitProjCloneUsage

# --------------------------------
testCloneValidRemoteDir()
{
    local tResult
    local tSave
    local tRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    local tRemoteRawOrigin=$cDatMount3/video-2020-04-02/george.raw
    local tProjName=george

    gpRemoteGitDir=""
    gpRemoteRawOrigin=""
    gpProjName=""

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"

    cd $HOME/project >/dev/null 2>&1

    chmod a-w $HOME/project
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Current dir is not writable"
    chmod ug+w $HOME/project

    tSave=$tRemoteGitDir
    tRemoteGitDir=$cDatMount3/video-2020-04-xx/george.git
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "$tRemoteGitDir does not exist"
    tRemoteGitDir=$tSave

    tSave=$tRemoteGitDir
    tRemoteGitDir=$cDatMount3/video-2020-04-xx/george.git
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "$tRemoteGitDir does not exist"
    tRemoteGitDir=$tSave

    chmod a-r $tRemoteGitDir/objects
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable, under $tRemoteGitDir"
    chmod -R a+r $tRemoteGitDir/objects

    chmod a-x $tRemoteGitDir/objects
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories must be executable, under $tRemoteGitDir"
    chmod a+x $tRemoteGitDir/objects

    rm $tRemoteRawOrigin/$cRemoteProjFile >/dev/null 2>&1
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "test/root/mnt/usb-video/video-2020-04-02/george.raw/.remote.proj"

    echo "bob.git" >$tRemoteRawOrigin/$cRemoteProjFile
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "crit: Error: Mismatch:"

    echo "$tRemoteGitDir" >$tRemoteRawOrigin/$cRemoteProjFile

    chmod a-r $tRemoteRawOrigin/src
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable, under $tRemoteRawOrigin"
    chmod -R a+r $tRemoteRawOrigin/src

    chmod a-x $tRemoteRawOrigin/src/raw
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories must be executable, under $tRemoteRawOrigin"
    chmod a+x $tRemoteRawOrigin/src/raw

    cd $HOME/project >/dev/null 2>&1
    touch $tProjName
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "A $tProjName already exists in this dir"
    rm $tProjName

    # Test for not enough room. Mock "df" to return a small value.
    df()
    {
        echo "2M"
    }
    tResult=$(df -BM $PWD --output=avail | tail -n1)
    assertEquals "$LINENO" "2M" "$tResult"
    tResult=$(fCloneValidRemoteDir $tRemoteGitDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning: The current directory should have 2048MB available"
    assertContains "$LINENO $tResult" "$tResult" "2MB is not enough space in current directory. Project '$tProjName' needs 6MB"
    ##assertContains "$LINENO $tResult" "$tResult" "uncomment to see"
    unset -f df

    df()
    {
        echo "19M"
    }

    fCloneValidRemoteDir $tRemoteGitDir >/tmp/test-clone.tmp 2>&1
    assertTrue "$LINENO" "$?"
    tResult=$(cat /tmp/test-clone.tmp)
#    assertContains "$LINENO $tResult" "$tResult" "warning: The current directory should have 20480MB available"
    assertEquals "$LINENO" "$tRemoteGitDir" "$gpRemoteGitDir"
    assertEquals "$LINENO" "$RemoteRawOrigin" "$RemoteRawOrigin"
    assertEquals "$LINENO" "$tProjName" "$gpProjName"
    unset -f df

    df()
    {
        echo "30000M"
    }

    fCloneValidRemoteDir $tRemoteGitDir >/tmp/test-clone.tmp 2>&1
    assertTrue "$LINENO" "$?"
    tResult=$(cat /tmp/test-clone.tmp)
    assertNotContains "$LINENO $tResult" "$tResult" "warning: The current directory should have 20480MB available"
    assertEquals "$LINENO" "$tRemoteGitDir" "$gpRemoteGitDir"
    assertEquals "$LINENO" "$RemoteRawOrigin" "$RemoteRawOrigin"
    assertEquals "$LINENO" "$tProjName" "$gpProjName"
    unset -f df

    rm /tmp/test-clone.tmp
    return 0
} # testCloneValidRemoteDir

testCloneGettingStarted()
{
    local tResult

    gpDebug=2
    cd $HOME/project >/dev/null 2>&1
    gpAuto=1
    gpYesNo=n
    tResult=$(fCloneGettingStarted 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"
    assertContains "$LINENO $tResult" "$tResult" "Not continuing"

    gpAuto=1
    gpYesNo=y
    tResult=$(fCloneGettingStarted 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"
    assertContains "$LINENO $tResult" "$tResult" "Cloning: $gpProjName"

    gpAuto=0
    gpYesNo=""
    tResult=$(fCloneGettingStarted 2>&1 < <(echo y))
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"
    assertContains "$LINENO $tResult" "$tResult" "Cloning: $gpProjName"

    return 0
} # testCloneGettingStarted

# --------------------------------
testCloneCheckLocalConfig()
{
    local tResult

    # SetUp
    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD
    cd $cDatProj1 >/dev/null 2>&1
    gpLocalTopDir=$PWD
    gpProjName=george
    HOSTNAME=testserver
    #c ConfigHost=.gitproj.config.$HOSTNAME

    # Start tests
    tResult=$(fCloneCheckLocalConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    gpDebug=2
    rm .gitproj
    tResult=$(fCloneCheckLocalConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "warning: Missing file: .gitproj It should have been versioned! Will try to recreate it from ~/.gitconfig"

    #tResult=$(grep remote-raw-origin $c ConfigHost 2>&1)
    #assertContains "$LINENO $tResult" "$tResult" "$tRemoteRawOrigin"
    return 0
    rm .gitproj
    #rm $c ConfigHost
    tResult=$(fCloneCheckLocalConfig 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There are not host file to copy from"

    return 0
} # testCloneCheckLocalConfig

# --------------------------------
testCloneMkGitDirFail()
{
    local tResult
    local tStatus
    local tTop

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawOrigin=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    assertContains "$LINENO $HOME" $HOME "adric"
    assertContains "$LINENO $gpRemoteGitDir" $gpRemoteGitDir 'video-2020-04-02/george.git'
    assertContains "$LINENO gpRemoteRawOrigin=$gpRemoteRawOrigin" $gpRemoteRawOrigin 'video-2020-04-02/george.raw'
    assertTrue $LINENO "[ -r $HOME/.gitconfig ]"

    # Force a failure
    cd $HOME/project >/dev/null 2>&1

    chmod a-w .
    tResult=$(fCloneMkGitDir 2>&1)
    assertFalse $LINENO "$?"
    assertContains $LINENO "$tResult" "The above command should have worked"

    return 0
} # testCloneMkGitDirFail

# --------------------------------
testCloneMkGitDirPass()
{
    local tResult
    local tStatus
    local tTop
    local tTarOut=$gpTest/test-env_Home3AfterCloneMkGit.tgz

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawOrigin=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    assertContains "$LINENO $HOME" $HOME "adric"
    assertContains "$LINENO $gpRemoteGitDir" $gpRemoteGitDir 'video-2020-04-02/george.git'
    assertContains "$LINENO gpRemoteRawOrigin=$gpRemoteRawOrigin" $gpRemoteRawOrigin 'video-2020-04-02/george.raw'
    assertEquals $LINENO george $gpProjName
    assertTrue $LINENO "[ -r $HOME/.gitconfig ]"

    cd $HOME/project >/dev/null 2>&1
    fCloneMkGitDir >/dev/null 2>&1
    tStatus=$?
    assertTrue $LINENO "$tStatus"
    assertEquals $LINENO george $gpProjName
    assertEquals $LINENO $HOME/project/george $gpLocalTopDir
    assertTrue "$LINENO $HOME/project/$gpProjName" "[ -d $HOME/project/$gpProjName ]"

    cd $HOME/project/$gpProjName >/dev/null 2>&1
    assertTrue "$LINENO" "[ -f .gitproj ]"
    assertTrue "$LINENO" "[ -f $HOME/.gitconfig ]"
    assertTrue "$LINENO" "[ -d .git ]"
    assertTrue "$LINENO" "[ -x .git/hooks/pre-commit ]"
    assertTrue "$LINENO" "[ -x .pre-commit ]"
    assertTrue "$LINENO" "[ -x ~/.pre-commit ]"
    assertTrue "$LINENO diff" "diff $gpDoc/hooks/pre-commit .git/hooks/pre-commit"

    assertEquals $LINENO testserver2 $HOSTNAME
    assertTrue $LINENO "grep local-host.=.$HOSTNAME .git/config"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    tResult=$(git branch 2>&1)
    assertTrue $LINENO "$?"
    assertContains "$LINENO $tResult" "$tResult" "main"
    assertContains "$LINENO $tResult" "$tResult" "develop"
    ##assertContains "$LINENO tResult=$tResult" "$tResult" "Uncomment to check"

    tResult=$(git config --get --includes gitproj.config.local-status)
    assertTrue $LINENO "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "not-defined"

    tResult=$(git config --get --includes gitproj.config.remote-status)
    assertTrue $LINENO "$?"
    assertNotContains "$LINENO $tResult" "$tResult" "not-defined"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after CloneMkGitDir."
        echo -e "\tRestore $tTarOut relative to env cDatHome3=$cDatHome3"
	fTestSavePath
        cd $cDatHome3 >/dev/null 2>&1
        echo -en "\t"
        tar -cvzf $tTarOut .
        echo
    fi

    return 0
} # testCloneMkGitDirPass

# --------------------------------
testCloneMkRawDirFail()
{
    local tResult
    local tTarIn=$gpTest/test-env_Home3AfterCloneMkGit.tgz

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawOrigin=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTarIn ]; then
        fail "Could not find $tTarIn [$LINENO]"
        exit 1
    fi
    tar -xzf $tTarIn
    fTestPatchPath

    gpLocalTopDir=$HOME/project/george
    cd $gpLocalTopDir >/dev/null 2>&1

    gpVerbose=3
    mkdir -p raw/src/final/george.mp4
    chmod -R a-wx raw 2>/dev/null
    tResult=$(fCloneMkRawDir 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    chmod -R ug+wx raw 2>/dev/null
    assertContains "$LINENO $tResult" "$tResult" "already exists"
    assertContains "$LINENO $tResult" "$tResult" "Run:"
    assertContains "$LINENO $tResult" "$tResult" "There may be a problem with this rsync"
    ##assertContains "$LINENO tResult=$tResult" "$tResult" "Uncomment to check"

    return 0
} # testCloneMkRawDirFail

# --------------------------------
testCloneMkRawDirPass()
{
    local tResult
    local tTarIn=$gpTest/test-env_Home3AfterCloneMkGit.tgz

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawOrigin=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    # Setup
    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTarIn ]; then
        fail "Could not find $tTarIn [$LINENO]"
        return
    fi
    tar -xzf $tTarIn
    fTestPatchPath
    gpLocalTopDir=$HOME/project/george
    gpLocalRawDir=$HOME/project/george/raw
    gpVerbose=3

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkRawDir 2>&1)
    assertTrue "$LINENO" "$?"
    assertTrue $LINENO "[ -f $gpLocalRawDir/README.txt ]"
    assertContains "$LINENO $tResult" "$tResult" "mkdir"
    assertTrue "$LINENO" "[ -f $gpLocalRawDir/src/final/george.mp4 ]"
    ##assertContains "$LINENO tResult=$tResult" "$tResult" "Uncomment to check"

    tResult=$(fComGetConfig -l -k "gitproj.config.remote-raw-origin")
    assertTrue $LINENO "$?"
    assertContains "$LINENO $tResult" $tResult "$gpRemoteRawOrigin"

    tResult=$(fComGetConfig -l -k "gitproj.config.remote-status")
    assertTrue $LINENO "$?"
    assertContains "$LINENO $tResult" "$tResult" "defined"

    return 0
} # testCloneMkRawDirPass

# --------------------------------
testCloneSummary()
{
    local tResult
    local tStatus
    local tTarIn=$gpTest/test-env_Home3AfterCloneMkGit.tgz
    local tTarOut=$gpTest/test-env_Home3AfterCloneSummary.tgz

    # Setup
    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTarIn ]; then
        fail "Could not find $tTarIn [$LINENO]"
        return
    fi
    tar -xzf $tTarIn
    fTestPatchPath
    gpLocalTopDir=$HOME/project/george
    gpLocalRawDir=$HOME/project/george/raw
    gpVerbose=3
    gpAuto=1
    gpYesNo=y

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkRawDir 2>&1)
    assertTrue "$LINENO $tResult" $?

    # Tests

    gpYesNo=n
    tResult=$(fCloneSummary 2>&1)
    assertFalse "$LINENO $tResult" $?
    assertContains "$LINENO $tResult" "$tResult" "Not continuing"

    gpYesNo=y
    tResult=$(fCloneSummary 2>&1)
    tStatus=$?
    assertTrue "$LINENO $tResult" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Committing changes"
    #assertContains "$LINENO $tResult" "$tResult" "Added .gitproj.config.testserver2"
    assertContains "$LINENO $tResult" "$tResult" "nothing to commit"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after fCloneSummary."
        echo -e "\tRestore $tTarOut relative to env cDatHome3=$cDatHome3"
	fTestSavePath
        cd $cDatHome3 >/dev/null 2>&1
        echo -en "\t"
        tar -cvzf $tTarOut .
        echo
    fi

    return 0
} # testCloneSummary

# --------------------------------
testCloneFromRemoteDir()
{
    local tResult
    local tRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    local tRemoteRawOrigin=$cDatMount3/video-2020-04-02/george.raw
    local tProjName=george

    # Setup
    gpRemoteGitDir=""
    gpRemoteRawOrigin=""
    gpProjName=""
    gpAuto=1
    gpYesNo=y
    gpVerbose=3
    cd $HOME/project >/dev/null 2>&1

    # TODO remove this when cRemoteProjFile is added to test-env files (remote)
    echo "$tRemoteGitDir" >$tRemoteRawOrigin/$cRemoteProjFile

    tResult=$(fCloneFromRemoteDir $tRemoteGitDir 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "You now have a local git repository project"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    return 0
} # testCloneFromRemoteDir

# --------------------------------
testGetProjCloneCLI()
{
    local tResult
    local tRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    local tRemoteRawOrigin=$cDatMount3/video-2020-04-02/george.raw

    # TODO remove this when cRemoteProjFile is added to test-env files (remote)
    echo "$tRemoteGitDir" >$tRemoteRawOrigin/$cRemoteProjFile

    cd $cDatHome3/project >/dev/null 2>&1
    tResult=$($gpBin/git-proj-clone -a -V 1 -d $tRemoteRawOrigin -y 2>&1)
    assertFalse $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "george.raw is not a git repo"
    #assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    cd $cDatHome3/project >/dev/null 2>&1
    tResult=$($gpBin/git-proj-clone -a -V 1 -d $tRemoteGitDir -y 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "You now have a local git repository project"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    return 0
} # testGetProjCloneCLI

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
. $gpBin/gitproj-clone.inc >/dev/null 2>&1

# Look for serious setup errors
fTestConfigSetup

fTestRun $gpTestList
