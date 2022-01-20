#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-clone.sh

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
NAoneTimeSetUp()
{
    return 1

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
    return 1
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
    local tTar=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz

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
    if [ ! -r $tTar ]; then
        echo "Could not find: $tTar [$LINENO]"
        return 1
    fi
    tar -xzf $tTar
    cd - >/dev/null 2>&1

    # git proj to be cloned:
    HOME=$cDatHome3
    gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git

    cd $HOME/project >/dev/null 2>&1
    . $gpBin/gitproj-clone.inc
    gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    gpProjName=${gpRemoteGitDir##*/}
    gpProjName=${gpProjName%.git}

    find $gpRemoteGitDir $gpRemoteRawDir -exec chmod a+r {} \;
    find $gpRemoteGitDir $gpRemoteRawDir -exec chmod ug+w {} \;
    find $gpRemoteGitDir $gpRemoteRawDir -type d -exec chmod a+rx {} \;

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
    assertEquals "$LINENO gpRemoteRawDir" "${gpRemoteGitDir%.git}.raw" "$gpRemoteRawDir"
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

    #gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    #gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    #gpProjName=${gpRemoteGitDir##*/}
    #gpProjName=${gpProjName%.git}

    cd $cDatHome/$cDatProj1 >/dev/null 2>&1
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "must NOT be in a git repo"

    cd $HOME/project >/dev/null 2>&1

    chmod a-w $HOME/project
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Current dir is not writable"
    chmod ug+w $HOME/project

    tSave=$gpRemoteGitDir
    gpRemoteGitDir=$cDatMount3/video-2020-04-xx/george.git
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "$gpRemoteGitDir does not exist"
    gpRemoteGitDir=$tSave

    tSave=$gpRemoteRawDir
    gpRemoteRawDir=$cDatMount3/video-2020-04-xx/george.git
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "$gpRemoteRawDir does not exist"
    gpRemoteRawDir=$tSave

    chmod a-r $gpRemoteGitDir/objects
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable, under $gpRemoteGitDir"
    chmod -R a+r $gpRemoteGitDir/objects

    chmod a-x $gpRemoteGitDir/objects
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories must be executable, under $gpRemoteGitDir"
    chmod a+x $gpRemoteGitDir/objects

    chmod a-r $gpRemoteRawDir/src
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories and files must be readable, under $gpRemoteRawDir"
    chmod -R a+r $gpRemoteRawDir/src

    chmod a-x $gpRemoteRawDir/src/raw
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "All directories must be executable, under $gpRemoteRawDir"
    chmod a+x $gpRemoteRawDir/src/raw

    cd $HOME/project >/dev/null 2>&1
    touch $gpProjName
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "A $gpProjName already exists in this dir"
    rm $gpProjName

    # Test for not enough room. Mock "df" to return a small value.
    df()
    {
        echo "2M"
    }
    tResult=$(df -BM $PWD --output=avail | tail -n1)
    assertEquals "$LINENO" "2M" "$tResult"
    tResult=$(fCloneValidRemoteDir 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There is not enough space in current directory. Project '$gpProjName' needs 6MB"
    unset -f df

    return 0
} # testCloneValidRemoteDir

testCloneGettingStarted()
{
    local tResult

    gpDebug=2
    cd $HOME/project >/dev/null 2>&1
    gpYesNo=No
    tResult=$(fCloneGettingStarted 2>&1)
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"
    assertContains "$LINENO $tResult" "$tResult" "Not continuing"

    gpYesNo=Yes
    tResult=$(fCloneGettingStarted 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Be sure you are"
    assertContains "$LINENO $tResult" "$tResult" "Cloning: $gpProjName"

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

    #tResult=$(grep remote-raw-dir $c ConfigHost 2>&1)
    #assertContains "$LINENO $tResult" "$tResult" "$gpRemoteRawDir"
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
    # gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    assertContains "$LINENO $HOME" $HOME "adric"
    assertContains "$LINENO $gpRemoteGitDir" $gpRemoteGitDir 'video-2020-04-02/george.git'
    assertContains "$LINENO gpRemoteRawDir=$gpRemoteRawDir" $gpRemoteRawDir 'video-2020-04-02/george.raw'
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
    local tTar=$gpTest/test-env_Home3AfterCloneMkGit.tgz

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    assertContains "$LINENO $HOME" $HOME "adric"
    assertContains "$LINENO $gpRemoteGitDir" $gpRemoteGitDir 'video-2020-04-02/george.git'
    assertContains "$LINENO gpRemoteRawDir=$gpRemoteRawDir" $gpRemoteRawDir 'video-2020-04-02/george.raw'
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
    assertTrue $LINENO "[ -f .gitproj ]"
    assertTrue $LINENO "[ -f $HOME/.gitconfig ]"
    assertTrue $LINENO "[ -d .git ]"
    assertTrue $LINENO "[ -x .git/hooks/pre-commit ]"
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
        echo -e "\tRestore $tTar relative to env cDatHome3=$cDatHome3"
        cd $cDatHome3 >/dev/null 2>&1
        echo -en "\t"
        tar -cvzf $tTar .
        echo
    fi

    return 0
} # testCloneMkGitDirPass

# --------------------------------
testCloneMkRawDirFail()
{
    local tResult
    local tTar=$gpTest/test-env_Home3AfterCloneMkGit.tgz

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTar ]; then
        fail "Could not find $tTar [$LINENO]"
        return 1
    fi
    tar -xzf $tTar

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
    local tTar=$gpTest/test-env_Home3AfterCloneMkGit.tgz

    # Assumes setUp has run
    # $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    #                           cTestDestDir=$gpTest/../..
    # $cDatHome3/project        $cTestDestDir/test/root/home/adric/project
    # HOME=$cDatHome3           $cTestDestDir/test/root/home/adric
    # HOSTNAME=testserver2
    #                 cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    # gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    # gpProjName=george

    # Setup
    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTar ]; then
        fail "Could not find $tTar [$LINENO]"
        return 1
    fi
    tar -xzf $tTar
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

    tResult=$(fComGetConfig -l -k "gitproj.config.remote-raw-dir")
    assertTrue $LINENO "$?"
    assertContains "$LINENO $tResult" $tResult "$gpRemoteRawDir"

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
    local tTar=$gpTest/test-env_Home3AfterCloneMkGit.tgz
    local tTar2=$gpTest/test-env_Home3AfterCloneSummary.tgz

    # Setup
    cd $cDatHome3 >/dev/null 2>&1
    if [ ! -r $tTar ]; then
        fail "Could not find $tTar [$LINENO]"
        return 1
    fi
    tar -xzf $tTar
    gpLocalTopDir=$HOME/project/george
    gpLocalRawDir=$HOME/project/george/raw
    gpVerbose=3
    gpYesNo=Yes

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkRawDir 2>&1)
    assertTrue "$LINENO $tResult" $?

    # Tests

    gpYesNo=No
    tResult=$(fCloneSummary 2>&1)
    assertFalse "$LINENO $tResult" $?
    assertContains "$LINENO $tResult" "$tResult" "Not continuing"

    gpYesNo=Yes
    tResult=$(fCloneSummary 2>&1)
    tStatus=$?
    assertTrue "$LINENO $tResult" $tStatus
    assertContains "$LINENO $tResult" "$tResult" "Committing changes"
    #assertContains "$LINENO $tResult" "$tResult" "Added .gitproj.config.testserver2"
    assertContains "$LINENO $tResult" "$tResult" "nothing to commit"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after fCloneSummary."
        echo -e "\tRestore $tTar2 relative to env cDatHome3=$cDatHome3"
        cd $cDatHome3 >/dev/null 2>&1
        echo -en "\t"
        tar -cvzf $tTar2 .
        echo
    fi

    return 0
} # testCloneSummary

# --------------------------------
testCloneFromRemoteDir()
{
    local tResult

    # Setup
    cd $cDatHome3 >/dev/null 2>&1
    gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    gpProjName=george
    gpYesNo=Yes
    gpVerbose=3
    cd $HOME/project >/dev/null 2>&1

    tResult=$(fCloneFromRemoteDir 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "All subcommands will output"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    return 0
} # testCloneFromRemoteDir

# --------------------------------
testGetProjCloneCLI()
{
    local tResult

    cd $cDatHome3/project >/dev/null 2>&1
    tResult=$($gpBin/git-proj-clone -y -vv -d $cDatMount3/video-2020-04-02/george.git -y 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "All subcommands will output"
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
. $gpBin/gitproj-clone.inc

# Look for serious setup errors
fTestConfigSetup

fTestRun $gpTestList
