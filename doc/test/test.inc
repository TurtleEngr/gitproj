#!/bin/bash

# --------------------------------
# Test globals

export cTest cTestCurDir gpTest gpUnitDebug
export cTestSrcDir cTestDestDir cTestFiles cTestConfig SHUNIT_COLOR

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
fComUDebug()
{
    if [ ${gpUnitDebug:-0} -ne 0 ]; then
        echo "fComUDebug: $*" 1>&2
    fi
    return 0
    cat <<EOF

=internal-head2 Com Unit Test Functions

EOF
} # fComUDebug

# --------------------------------
fComSetupTestEnv()
{
    cBin=$cTest/../../git-core
    cd $cBin >/dev/null 2>&1
    cBin=$PWD
    cd - >/dev/null 2>&1

    cDoc=${cTest%/test}

    cName=gitproj-com.inc
    . $cBin/gitproj-com.inc

    cTestSrcDir=$cTest
    cTestFiles=$cTestSrcDir/test-env.tgz
    if [ ! -r $cTestFiles ]; then
        echo "Error: Cannot find: $cTestFiles. [$LINENO]"
        exit 1
    fi

    cTestConfig=$cDoc/config/gitproj.config.test-dev

    cTestDestDir=$cTest/../..
    cd $cTestDestDir >/dev/null 2>&1
    cTestDestDir=$PWD
    cd - >/dev/null

    # These are found in the unpacked $cTestFiles
    cDatHome=$cTestDestDir/test/root/home/john
    cDatProj1=project/george
    cDatProj2=project/paulb
    cDatProj3=project/paulb
    cDatProj1Big="src/raw/MOV001.mp4 src/raw/MOV001.MP3 src/final/george.mp4"
    cDatProj2Big="src/raw/MOV001.mp4 src/raw/MOV001.MP3 src/final/paulb.mp4"
    cDatMount1=$cTestDestDir/test/root/mnt/disk-2
    cDatMount2=$cTestDestDir/test/root/mnt/usb-misc/files-2021-08-12
    cDatMount3=$cTestDestDir/test/root/mnt/usb-video/video-2020-04-02

    return
} # fComSetup

# --------------------------------
fRmTestEnv()
{
    if [ -d $cTestDestDir/test ]; then
        find $cTestDestDir/test -type l -exec rm {} \;
        rm -rf $cTestDestDir/test
    fi
    return 0
}
# --------------------------------
fCreateTestEnv()
{
    fRmTestEnv
    cd $cTestDestDir >/dev/null 2>&1
    tar -xzf $cTestFiles

    cHome=$HOME
    cd $cDatHome >/dev/null 2>&1
    HOME=$PWD
    cd - >/dev/null 2>&1

} # fCreateTestEnv

fSetupTestConfig()
{
    if [ ! -d $cDatHome ]; then
        echo "Error: Test env is not seetup. Missing $cDatHome [$LINENO]"
        exit 1
    fi
    cp $cDoc/config/gitproj.config.global $HOME/.gitproj.config.global
    cp $cTestConfig $HOME/.gitproj.config.test
    fComSetConfig -a -g -k "include.path" -v ".gitproj.config"
    fComSetConfig -a -g -k "include.path" -v ".gitproj.test.config"

    if [ -d $cBin/../.git ]; then
        # We are testing from the git clone, so set GIT_DIR
        tGitTop=$(git rev-parse --show-toplevel)
        sed -i "s;GIT_DIR;$tGitTop;g" $HOME/.gitproj.test.config
    fi
    return 0
} # fSetupTestConfig

# ========================================
fComSetupTestEnv
