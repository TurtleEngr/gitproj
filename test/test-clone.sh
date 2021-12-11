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
    # Restore default global values, before each test

    unset cConfigGlobal cConfigLocal cCurDir cGetOrigin cGetTopDir \
        cGitProjVersion cPID gErr

    unset gpAction gpAuto gpAutoMove gpBin \
        gpDoc gpFacility gpGitFlow gpLocalRawDir \
        gpLocalTopDir gpMaxSize \
        gpPath gpProjName gpSysLog gpVer gpVerbose

    fTestSetupEnv
    fTestCreateEnv
    cd $cTestDestDir >/dev/null 2>&1
    tar -xzf $gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    cd - >/dev/null 2>&1

    mkdir -p $cDatHome3/project >/dev/null 2>&1

    cd $HOME/project >/dev/null 2>&1
    . $gpBin/gitproj-clone.inc

    # git proj to be cloned:
    HOME=$cDatHome3
    HOSTNAME=testserver2
    gpRemoteGitDir=$cDatMount3/video-2020-04-02/george.git
    gpRemoteRawDir=${gpRemoteGitDir%.git}.raw
    gpProjName=${gpRemoteGitDir##*/}
    gpProjName=${gpProjName%.git}

    find $gpRemoteGitDir $gpRemoteRawDir -exec chmod a+r {} \;
    find $gpRemoteGitDir $gpRemoteRawDir -exec chmod ug+w {} \;
    find $gpRemoteGitDir $gpRemoteRawDir -type d -exec chmod a+rx {} \;

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
    cConfigHost=.gitproj.config.$HOSTNAME

    # Start tests
    tResult=$(fCloneCheckLocalConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    gpDebug=2
    rm $cConfigLocal
    tResult=$(fCloneCheckLocalConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Missing file: $cConfigLocal It should have been versioned. Will try to recreate it from a host config file"
    assertContains "$LINENO $tResult" "$tResult" "cp $cConfigHost $cConfigLocal"

    tResult=$(grep remote-raw-dir $cConfigHost 2>&1)
    assertContains "$LINENO $tResult" "$tResult" "$gpRemoteRawDir"

    rm $cConfigLocal $cConfigHost
    tResult=$(fCloneCheckLocalConfig 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There are not host file to copy from"

    return 0
} # testCloneCheckLocalConfig

# --------------------------------
testCloneCheckHostConfigNoHost()
{
    local tResult

    # SetUp
    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD
    cd $cDatProj1 >/dev/null 2>&1
    gpLocalTopDir=$PWD
    gpProjName=george
    HOSTNAME=testserver
    cConfigHost=.gitproj.config.$HOSTNAME

    # Start tests
    tResult=$(fCloneCheckHostConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    rm $cConfigHost
    gpDebug=2
    tResult=$(fCloneCheckHostConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Missing host file: $cConfigHost  It will be created for this new host"
    assertContains "$LINENO $tResult" "$tResult" "cp $cConfigLocal $cConfigHost"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    tResult=$(grep remote-raw-dir $cConfigHost 2>&1)
    assertContains "$LINENO $tResult" "$tResult" "$gpRemoteRawDir"
    
    return 0
} # testCloneCheckHostConfigNoHost


# --------------------------------
testCloneCheckHostConfigManyHosts()
{
    local tResult

    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD
    cd $cDatProj1 >/dev/null 2>&1
    gpLocalTopDir=$PWD
    gpProjName=george
    HOSTNAME=testserver2
    cConfigHost=.gitproj.config.$HOSTNAME

    cp .gitproj.config.testserver .gitproj.config.testserver-1
    echo '#gitproj.config.testserver-1' >>.gitproj.config.testserver-1
    
    cp .gitproj.config.testserver .gitproj.config.testserver-2
    echo '#gitproj.config.testserver-2' >>.gitproj.config.testserver-2

    touch .gitproj.config.testserver-2
    sleep 0.25
    touch .gitproj.config.local

    gpDebug=2
    tResult=$(fCloneCheckHostConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue "$LINENO $tResult" "[ -f $cConfigHost ]"
    assertContains "$LINENO $tResult" "$tResult" "Missing host file: $cConfigHost  It will be created for this new host"
    assertContains "$LINENO $tResult" "$tResult" "cp .gitproj.config.testserver-2 $cConfigHost"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    tResult=$(grep remote-raw-dir $cConfigHost 2>&1)
    assertContains "$LINENO $tResult" "$tResult" "$gpRemoteRawDir"
    
    return 0
} # testCloneCheckHostConfigManyHosts

# --------------------------------
testCloneCheckProjConfig()
{
    local tResult

    # SetUp
    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD
    cd $cDatProj1 >/dev/null 2>&1
    gpLocalTopDir=$PWD
    gpProjName=george
    HOSTNAME=testserver
    cConfigHost=.gitproj.config.$HOSTNAME

    # Start tests
    tResult=$(fCloneCheckProjConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    gpDebug=2
    rm $cConfigLocal $cConfigHost
    tResult=$(fCloneCheckProjConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "There are no host project config files for this project. They should have been versioned! Will try to create them, but they could have bad values"
    assertContains "$LINENO $tResult" "$tResult" "Uncomment to check"

    assertTrue "$LINENO $tResult" "[ -f $cConfigLocal ]"
    assertTrue "$LINENO $tResult" "[ -f $cConfigHost ]"
 
    return 0
} # testCloneCheckProjConfig

# --------------------------------
testCloneMkGitDir()
{
    local tResult
    local tStatus
    local tTop
return 1

    gpLocalTopDir=$HOME/$cDatProj1
    cd $HOME >/dev/null 2>&1
    tar -xzf $gpTest/test-env_HomeAfterBMove.tgz

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1

    tResult=$(fCloneMkGitDir 2>&1)
    tStatus=$?
    assertTrue $LINENO "$tStatus"
    assertTrue $LINENO "[ -d $gpLocalTopDir/.git ]"
    assertTrue $LINENO "[ -f $gpLocalTopDir/.gitignore ]"
    assertTrue $LINENO "$(
        grep -q raw $gpLocalTopDir/.gitignore
        echo $?
    )"

    tTop=$($cGetTopDir)
    assertContains $LINENO "$tTop" "$gpLocalTopDir"

    tResult=$(git branch 2>&1)
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "develop"
    assertContains "$LINENO $tResult" "$tResult" "main"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after git init."
        echo -e "\tRestore test-env_ProjAfterGClone.tgz relative to cDatHome/project"
        cd $HOME/project >/dev/null 2>&1
        tar -cvzf $gpTest/test-env_ProjAfterGClone.tgz $gpProjName
    fi

    assertTrue "$LINENO not exec" "[ -x $tTop/.git/hooks/pre-commit ]"
    assertTrue "$LINENO diff" "diff $gpDoc/hooks/pre-commit $tTop/.git/hooks/pre-commit"

    return 0
} # testCloneMkGitDir

# --------------------------------
testCloneMkRawDir()
{
    local tResult
return 1

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    gpProjName=${cDatProj1##*/}
    gpLocalRawDir=$gpLocalTopDir/raw
    gpGitFlow="true"
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0
    gpVerbose=2

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkRaw 2>&1)
    assertTrue $LINENO $?
    assertTrue $LINENO "[ -d $gpLocalRawDir ]"
    assertTrue $LINENO "[ -f $gpLocalRawDir/README.txt ]"
    assertTrue $LINENO "$(
        grep -q 'Do NOT remove these files' $gpLocalRawDir/README.txt
        echo $?
    )"
    assertContains "$LINENO $tResult" "$tResult" "mkdir"
    assertContains "$LINENO $tResult" "$tResult" "Create: raw/README.txt"

    gpUnitDebug=0
    fTestDebug "$tResult"

    return 0
} # testCloneMkRawDir

# --------------------------------
testCloneMkHostConfig()
{
# TBD
return 1
    local tSrc=${BASH_SOURCE##*/}
    local tResult

    if [ ! -f $gpTest/test-env_HomeAfterBMove.tgz ]; then
        fail "Missing test-env_HomeAfterBMove.tgz [$tSrc:$LINENO]"
        return 1
    fi
    if [ ! -f $gpTest/test-env_ProjAfterGClone.tgz ]; then
        fail "Missing test-env_ProjAfterGClone.tgz [$tSrc:$LINENO]"
        return 1
    fi

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    cd $HOME >/dev/null 2>&1
    tar -xzf $gpTest/test-env_HomeAfterBMove.tgz
    cd $gpTest

    cd $HOME/project >/dev/null 2>&1
    tar -xzf $gpTest/test-env_ProjAfterGClone.tgz
    cd $gpTest

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="10k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1
    assertFalse $LINENO "[ -f $cConfigLocal ]"
    assertFalse $LINENO "[ -f $cConfigHost ]"
    assertFalse $LINENO "$(grep -q gitproj.config .git/config >/dev/null 2>&1); echo $?)"

    tResult=$(fCloneMkLocalConfig 2>&1)
    assertTrue $LINENO "$?"
    assertTrue $LINENO "[ -f $cConfigLocal ]"
    assertTrue $LINENO "[ -f $cConfigHost ]"

    tResult=$(git config --local --list --show-origin --includes 2>&1)
    assertTrue $LINENO "$?"
    assertContains "$LINENO $tResult" "include.path=../$cConfigHost"

} # testCloneMkLocalConfig

# --------------------------------
testCloneSaveVars()
{
return 1
    local tSrc=${BASH_SOURCE##*/}
    local tResult
    local tFile
    local tS

    if [ ! -f $gpTest/test-env_ProjAfterGClone.tgz ]; then
        fail "Missing test-env_ProjAfterGClone.tgz [$tSrc:$LINENO]"
        return 1
    fi
    cd $HOME/project >/dev/null 2>&1
    tar -xzf $gpTest/test-env_ProjAfterGClone.tgz
    cd $gpTest

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneMkLocalConfig 2>&1)
    assertTrue "$LINENO $tResult" "$?"
    assertTrue $LINENO "[ -f $cConfigLocal ]"
    assertTrue $LINENO "[ -f $cConfigHost ]"

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="10k"
    gpAutoMove=true
    gpAuto=0

    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneSaveVarsToConfigs 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    # TBD refactor to use for loops with hash array maps?
    # cMapConf[gpBin]=gitproj.config.bin
    # generated from cMapConf: cMapVar[gitproj.config.bin]=gpBin
    # cMapFile[gitproj.config.bin]=global
    # cMapFile[gitproj.config.proj-name]=local
    # if cMapFile[key] is undefined, then assume it is global and local

    tFile=~/.gitproj.config.global
    tS=gitproj.config
    fTestCheckConfig2Var $tFile $tS.proj-status gpProjStatus ${gpProjStatus} $LINENO
    fTestCheckConfig2Var $tFile $tS.bin gpBin "${gBin#$gpLocalTopDir/}" $LINENO
    fTestCheckConfig2Var $tFile $tS.doc gpDoc "${gDoc#$gpLocalTopDir/}" $LINENO
    fTestCheckConfig2Var $tFile $tS.test gpTest "${gTest#$gpLocalTopDir/}" $LINENO
    fTestCheckConfig2Var $tFile $tS.facility gpFacility user $LINENO
    fTestCheckConfig2Var $tFile $tS.syslog gpSysLog true $LINENO

    for tFile in $gpLocalTopDir/$cConfigLocal \
        $gpLocalTopDir/$cConfigHost; do
        tS=gitproj.config
        fTestCheckConfig2Var $tFile $tS.local-status gpLocalStatus not-defined $LINENO
        fTestCheckConfig2Var $tFile $tS.remote-status gpRemoteStatus not-defined $LINENO
        fTestCheckConfig2Var $tFile $tS.proj-name gpProjName $gpProjName $LINENO
    done

    for tFile in ~/.gitproj.config.global $gpLocalTopDir/$cConfigLocal; do
        tS=gitproj.config
        fTestCheckConfig2Var $tFile $tS.git-flow-pkg gpGitFlow true $LINENO
        tS=gitproj.hook
        fTestCheckConfig2Var $tFile $tS.auto-move gpAutoMove true $LINENO
        fTestCheckConfig2Var $tFile $tS.binary-file-size-limit gpMaxSize 10k $LINENO
        fTestCheckConfig2Var $tFile $tS.check-file-names gpCheckFileNames true $LINENO
        fTestCheckConfig2Var $tFile $tS.check-for-big-files gpCheckForBigFiles true $LINENO
        fTestCheckConfig2Var $tFile $tS.pre-commit-enabled gpPreCommitEnabled true $LINENO
        fTestCheckConfig2Var $tFile $tS.source gpHookSource hooks/pre-commit $LINENO
    done

    return 0
} # testCloneSaveVars

# --------------------------------
testCloneSummary()
{
    local tResult
    local tStatus
return 1

    gpLocalTopDir=$HOME/$cDatProj1
    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="10k"

    tResult=$(fCloneSummary 2>&1 < <(echo -e "foo\nn"))
    assertFalse $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Continue with creating"
    assertContains "$LINENO $tResult" "$tResult" "Invalid answer:"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    gpUnitDebug=0
    fTestDebug "No result = $tResult"

    tResult=$(fCloneSummary 2>&1 < <(echo -e "y"))
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Continue with creating"

    gpUnitDebug=0
    fTestDebug "Yes result = $tResult"

    return 0
} # testCloneSummary

# --------------------------------
testCloneFromRemoteDir()
{
return 1
    local tSrc=${BASH_SOURCE##*/}
    local tResult
    local tStatus
    local tFile
    local tS

    gpGitFlow="true"
    gpMaxSize="10k"
    gpAutoMove=true

    gpAuto=1
    gpSysLog=true
    gpVerbose=3
    gpDebug=10

    # Create local git and local raw
    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    tResult=$(fCloneCreateLocalGit 2>&1)
    tStatus=$?
    assertTrue "$LINENO $tResult" "$tStatus"
    assertTrue "$LINENO" "[ -d $gpLocalTopDir/raw ]"

    if [ ${gpSaveTestEnv:-0} -ne 0 ] && [ $tStatus -eq 0 ]; then
        echo -e "\tCapture state of project after git init."
        echo -e "\tRestore test-env_ProjLocalDefined.tgz relative to HOME/project"
        cd $HOME >/dev/null 2>&1
        tar -cvzf $gpTest/test-env_ProjLocalDefined.tgz .gitconfig .gitproj.config.global $cDatProj1
    fi

    return 0
} # testCloneFromRemoteDir

# --------------------------------
testGetProjCloneCLI()
{
return 1
    startSkipping
    fail "TBD"
    return 0

    cd $HOME/$cDatProj1
    $gpBin/git-proj-clone local -a
} # testGetProjCloneCLI

# --------------------------------
NAtestCloneGetGitFlow()
{
return 1
    local tResult
    local tStatus

    tResult=$(fCloneGetGitFlow 2>&1 < <(echo -e "\nquit"))
    assertFalse "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    fCloneGetGitFlow >/dev/null 2>&1 < <(echo -e "yes")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "true" "$gpGitFlow"

    fCloneGetGitFlow >/dev/null 2>&1 < <(echo -e "No")
    assertTrue "$LINENO" $?
    assertEquals "$LINENO" "false" "$gpGitFlow"

    gpGitFlowPkg=foo-bar
    tResult=$(fCloneGetGitFlow 2>&1 < <(echo -e "\n"))
    assertTrue "$LINENO" $?
    assertContains "$LINENO $tResult" "$tResult" "git-flow is not installed"

    return 0
} # testCloneGetGitFlow

# --------------------------------
NAtestCloneMkGitFlow()
{
return 1
    local tResult

    gpLocalTopDir=$HOME/$cDatProj1
    cd $gpLocalTopDir >/dev/null 2>&1
    fCloneFirstTimeSet

    gpProjName=${cDatProj1##*/}
    gpGitFlow="true"
    gpMaxSize="1k"
    gpAutoMove=true
    gpAuto=0

    tResult=$(fCloneMkGitFlow 2>&1)
    assertTrue $LINENO $?
    assertEquals $LINENO "main" "$(git config --get --global gitflow.branch.main)"
    assertEquals $LINENO "develop" "$(git config --get --global gitflow.branch.develop)"
    assertEquals $LINENO "feature/" "$(git config --get --global gitflow.prefix.feature)"
    assertEquals $LINENO "bug/" "$(git config --get --global gitflow.prefix.bugfix)"
    assertEquals $LINENO "release/" "$(git config --get --global gitflow.prefix.release)"
    assertEquals $LINENO "hotfix/" "$(git config --get --global gitflow.prefix.hotfix)"
    assertEquals $LINENO "support/" "$(git config --get --global gitflow.prefix.support)"

    return 0
} # testCloneMkGitFlow

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
