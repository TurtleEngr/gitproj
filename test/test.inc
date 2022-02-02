#!/bin/bash

# --------------------------------
# Test globals

export cCurDir cDatHome cDatHome2 cDatHome3 cHome cTestCurDir \
    cTestDestDir cTarIn HOSTNAME
export gpBin gpDoc gpTest HOME
export gpDebug gpUnitDebug SHUNIT_COLOR
export gpCmdName gpCmdVer

export TMOUT=10

# Relative to $HOME, cDatHome, george
export cDatProj1 cDatProj2 cDatProj3
# Relative to $HOME, cDatHome2, bob
export cDatProj4

# Relative to the corresponding $cDatProjN
export cDatProj1Big cDatProj2Big

# These are simulations of local mount points, using absolute paths.
export cDatMount1 cDatMount2 cDatMount3

# Probably experted by other inc files
export gpAction gpAuto gpFacility gpGitFlow gpLocalRawDir \
    gpLocalRawDirPat gpLocalRawSymLink gpLocalTopDir gpMaxSize \
    gpProjName gpSysLog gpTestList gpVer gpVerbose

# ========================================

# --------------------------------
fUsageGitproj-test.inc()
{
    return 0
    cat <<\EOF >/dev/null
=internal-pod

=internal-head1 gitproj-test.inc

=internal-cut

EOF
}

# --------------------------------
fTestDebug()
{
    if [ ${gpUnitDebug:-0} -ne 0 ]; then
        echo "fTestDebug: $*" 1>&2
    fi
    return 0
    cat <<EOF

=internal-head2 Com Unit Test Functions

EOF
} # fTestDebug

# --------------------------------
fTestSetupEnv()
{
    local tSrc=${BASH_SOURCE##*/}

    #    SHUNIT_COLOR=none

    gpBin=$gpTest/../git-core
    cd $gpBin >/dev/null 2>&1
    gpBin=$PWD
    cd - >/dev/null 2>&1

    gpDoc=$gpTest/../doc
    cd $gpDoc >/dev/null 2>&1
    gpDoc=$PWD
    cd - >/dev/null 2>&1

    . $gpBin/gitproj-com.inc

    cTarIn=$gpTest/test-env.tgz
    if [ ! -r $cTarIn ]; then
        echo "Error: Cannot find: $cTarIn. [$tSrc:$LINENO]"
        exit 1
    fi

    cTestDestDir=$gpTest/../..
    cd $cTestDestDir >/dev/null 2>&1
    cTestDestDir=$PWD
    cd - >/dev/null

    # These are found in the unpacked $cTarIn
    cDatHome=$cTestDestDir/test/root/home/john
    # Home dir for most tests
    cDatProj1=project/george
    # Has some binary files, main proj will be created here
    cDatProj2=project/paulb
    # Has some binary files
    cDatProj3=project/beach
    # Already has a git repo in it, but not a proj git
    cDatHome2=$cTestDestDir/test/root/home/bob
    # created by test-pull.sh, home/paul will be copied to here
    cDatHome3=$cTestDestDir/test/root/home/adric
    # created by test-clone.sh, only has project/ in it
    cDatProj1Big="src/raw/MOV001.mp4 src/raw/MOV001.MP3 src/final/george.mp4"
    cDatProj2Big="src/raw/MOV001.mp4 src/raw/MOV001.MP3 src/final/paulb.mp4"
    cDatMount1=$cTestDestDir/test/root/mnt/disk-2
    cDatMount2=$cTestDestDir/test/root/mnt/usb-misc
    # files-2021-08-12/archive/video/
    # files-2021-08-12/development/
    cDatMount3=$cTestDestDir/test/root/mnt/usb-video
    # video-2019-11-26/example.git
    # video-2019-11-26/example.raw
    # video-2019-11-26/dev/
    # video-2019-11-26/release/
    # video-2019-11-26/src/
    # video-2020-04-02 - a remote git proj will be put here

    return 0
} # fTestSetupEnv

# --------------------------------
fTestRmEnv()
{
    if [ -d $cTestDestDir/test ]; then
        find $cTestDestDir/test -type l -exec rm {} \;
        rm -rf $cTestDestDir/test
    fi
    return 0
} # fTestRmEnv

# --------------------------------
fTestConfigSetup()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ ! -d $cDatHome ]; then
        echo "Error: Test env is not setup. Missing $cDatHome [$tSrc:$LINENO]"
        exit 1
    fi
    if [ "$HOME" != "$cDatHome" ]; then
        echo "Error: Test home is not setup. $cDatHome [$tSrc:$LINENO]"
        exit 1
    fi

    return 0
} # fTestConfigSetup

# --------------------------------
fTestCheckConfig2Var()
{
    local pFile="$1"
    local pKey="$2"
    local pVarName="$3"
    local pExpect="$4"
    local pLine="$5"

    local tSrc=${BASH_SOURCE##*/}
    local tResult
    local tStatus
    local tValue

    assertTrue "$pLine:$LINENO $pFile" "[ -f $pFile ]"
    tResult=$(git config --file $pFile $pKey)
    tStatus=$?
    assertTrue "$pLine:$LINENO $pFile $pKey" "$tStatus"
    tValue=$(eval echo \$$pVarName)
    if [ -z "$tValue" ]; then
        tValue="${pVarName}-is-undefined"
    fi
    assertContains "$pLine:$LINENO $pFile $pVarName $tResult" "$tResult" "$tValue"
    assertContains "$pLine:$LINENO $pFile $pVarName $tResult" "$tResult" "$pExpect"

    return $tStatus
} # fTestCheckConfig2Var

# --------------------------------
fTestSavePath()
{
    # Call this just before creating tar files.
    # saved-path.tgz should be saved with the other tgz files.

    # This assumes gpTest is set, and the top test dir is at
    # $gpTest/../../test/

    local tSrc=${BASH_SOURCE##*/}
    local tCurDir=$PWD

    if [ -z "$gpTest" ]; then
        echo "Error: gpTest is not set [$tSrc:$LINENO]" 1>&2
        exit 1
    fi
    cd $gpTest/../.. >/dev/null 2>&1
    if [ ! -d test ]; then
        echo "Error: $PWD/test does not exist [$tSrc:$LINENO]" 1>&2
        exit 1
    fi

    echo "export cSavedPath=$PWD/test" >test/saved-path.inc
    chmod a+rx test/saved-path.inc
    tar -czf saved-path.tgz test/saved-path.inc

    cd $gpTest >/dev/null 2>&1
    ln -sf ../../saved-path.tgz .

    cd $tCurDir >/dev/null 2>&1
} # fTestSavePath

# --------------------------------
fTestPatchPath()
{
    # Call this after untaring files.  It may need to be called
    # multiple times, after git clone or git checkouts, because the
    # old path may be "in" the tared git repos.

    local tSrc=${BASH_SOURCE##*/}
    local tCurDir=$PWD
    local tFile
    local tNewPath=$PWD/test

    if [ -z "$gpTest" ]; then
        echo "Error: gpTest is not set [$tSrc:$LINENO]" 1>&2
        exit 1
    fi
    if [ ! -r $gpTest/saved-path.tgz ]; then
        echo "Error: Missing: $gpTest/saved-path.tgz [$tSrc:$LINENO]" 1>&2
        exit 1
    fi

    cd $gpTest/../.. >/dev/null 2>&1
    if [ ! -r saved-path.tgz ]; then
        echo "Error: Missing: saved-path.tgz [$tSrc:$LINENO]" 1>&2
        exit 1
    fi
    tar -xzf saved-path.tgz
    if [ ! -e test/saved-path.inc ]; then
        echo "Error: Missing: test/saved-path.inc [$tSrc:$LINENO]" 1>&2
        exit 1
    fi

    . test/saved-path.inc
    if [ -z "$cSavedPath" ]; then
        echo "Error: cSavedPath is not set [$tSrc:$LINENO]" 1>&2
        exit 1
    fi
    for tFile in $(find test -type f | grep -E '\.gitconfig|\.gitproj|config'); do
        sed -i "s;$cSavedPath;$tNewPath;g" $tFile
    done

    cd $tCurDir >/dev/null 2>&1
} # fTestPatchPath

# --------------------------------
fTestCreateEnv()
{
    local tSrc=${BASH_SOURCE##*/}
    local tCurDir=$PWD

    fTestRmEnv
    cd $cTestDestDir >/dev/null 2>&1

    if [ ! -f $cTarIn ]; then
        echo "Missing $cTarIn [$tSrc:$LINENO]"
        exit 1
    fi

    tar -xzf $cTarIn
    fTestPatchPath

    cHome=$HOME
    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD

    cd $tCurDir >/dev/null 2>&1
    return 0
} # fTestCreateEnv

# ========================================
HOSTNAME=testserver
fTestSetupEnv
