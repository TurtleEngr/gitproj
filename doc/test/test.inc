#!/bin/bash

# --------------------------------
# Test globals

export PWD cBin cCurDir cName cPID cVer cHome
export gErr gpDebug gpFacility gpLog gpVerbose
export cDoc cTest cRawDir cGetOrigin cGetTopDir cConfigGlobal cConfigLocal
export cTestCurDir cTestSrcDir cTestDestDir cTestFiles
export gpTest gpUnitDebugSHUNIT_COLOR

# Relative to $HOME
export cDatProj1 cDatProj2 cDatProj3

# Relative to the corresponding $cDatProjN
export cDatProj1Big cDatProj2Big

# These are simulations of local mount points, using absolute paths.
export cDatMount1 cDatMount2 cDatMount3

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
    cBin=$cTest/../../git-core
    cd $cBin >/dev/null 2>&1
    cBin=$PWD
    cd - >/dev/null 2>&1

    cDoc=${cTest%/test}

    . $cBin/gitproj-com.inc

    cTestSrcDir=$cTest
    cTestFiles=$cTestSrcDir/test-env.tgz
    if [ ! -r $cTestFiles ]; then
        echo "Error: Cannot find: $cTestFiles. [test.inc:$LINENO]"
        exit 1
    fi

    cTestDestDir=$cTest/../../..
    cd $cTestDestDir >/dev/null 2>&1
    cTestDestDir=$PWD
    cd - >/dev/null

    # These are found in the unpacked $cTestFiles
    cDatHome=$cTestDestDir/test/root/home/john
    cDatProj1=project/george
    cDatProj2=project/paulb
    cDatProj3=project/beach
    cDatProj1Big="src/raw/MOV001.mp4 src/raw/MOV001.MP3 src/final/george.mp4"
    cDatProj2Big="src/raw/MOV001.mp4 src/raw/MOV001.MP3 src/final/paulb.mp4"
    cDatMount1=$cTestDestDir/test/root/mnt/disk-2
    cDatMount2=$cTestDestDir/test/root/mnt/usb-misc/files-2021-08-12
    cDatMount3=$cTestDestDir/test/root/mnt/usb-video/video-2020-04-02

    return
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
    local tInclude
    local tTopDir

    if [ ! -d $cDatHome ]; then
        echo "Error: Test env is not setup. Missing $cDatHome [test.inc:$LINENO]"
        exit 1
    fi
    if [ "$HOME" != "$cDatHome" ]; then
	echo "Error: Test home is not setup. $cDatHome [test.inc:$LINENO]"
	exit 1
    fi

    return 0
} # fTestConfigSetup

# --------------------------------
fTestCreateEnv()
{
    fTestRmEnv
    cd $cTestDestDir >/dev/null 2>&1
    tar -xzf $cTestFiles

    cHome=$HOME
    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD
    cd - >/dev/null 2>&1
    return 0
} # fTestCreateEnv

# ========================================
fTestSetupEnv
