#!/bin/bash

# --------------------------------
# Test globals 
export gpTest gpUnitDebug cTestBin cTestCurDir
export cTestSrcDir cTestDestDir cTestFiles  SHUNIT_COLOR

# Relative to $HOME
export cDatProj1 cDatProj2 cDatProj3

# Relative to the corresponding $cDatProjN
export cDatProj1Big cDatProj2Big

# These are simulations of local mount points, using absolute paths.
export cDatMount1 cDatMount2 cDatMount3

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
        echo "fComUDebug: $*"
    fi
    return
    cat <<EOF

=internal-head2 Com Unit Test Functions

EOF
} # fComUDebug

# --------------------------------
fComSetupDev()
{
    cd cTestCurDir >/dev/null 2>&1
    . ../../git-core/gitproj-com.inc
    . fComSetGlobals

    cTestFiles=test-gitproj.tgz

    cTestDestDir=$cBin/../..
    cd $cTestDestDir >/dev/null 2>&1
    cTestDestDir=$PWD
    cd - >/dev/null

    if [ -r $cBin/../doc/test/$cTestFiles ]; then
        cTestSrcDir=$cBin/../doc/test
    fi
    if [ -r $cBin/../../$cTestFiles ]; then
        cTestSrcDir=$cBin/../..
    fi
    if [ -z "$cTestSrcDir" ]; then
        fError -m "Error: Cannot setup tests." -l $LINENO
    fi
    cd $cTestSrcDir >/dev/null 2>&1
    cTestSrcDir=$PWD
    cd - >/dev/null 2>&1
} # fComSetupDev

# --------------------------------
fComSetupProd()
{
    cd cTestCurDir >/dev/null 2>&1
    . /usr/lib/git-core/gitproj-com.inc
    
    echo TBD
} # fComSetupProd

# --------------------------------
fComSetup()
{
    # Look at execution location to determine if this is a production
    # environment or a development environment.
    if [ "$cTestBin" = "/usr/lib/git-core" ]; then
        fComSetupProd
    elif [ -d $cTestBin/../.git ]; then
        fComSetupDev
    else
        fError -m "Error: Cannot setup tests." -l $LINENO
    fi

    cDatProj1=project/george
    cDatProj2=project/paulb
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
    fRmTestEnv()
    cd $cTestDestDir >/dev/null
    tar -xzf $cTestSrcDir/$cTestFiles

    cd test/root/home/john >/dev/null
    cHome=$HOME
    HOME=$PWD
    cd $cBin >/dev/null
    cp $cBin/../doc/config/global.gitproj.config $HOME/.gitproj.config
    cp $cBin/../doc/config/test-dev.gitproj-test.config $HOME/.gitproj-test.config
    fComSetConfig -g -k "include.path" -v ".gitproj.config"
    fComSetConfig -g -k "include.path" -v ".gitproj-test.config" -a

    if [ -d $cBin/../.git ]; then
        # We are testing from the git clone, so set GIT_DIR
        tGitTop=$(git rev-parse --show-toplevel)
        sed -i "s;GIT_DIR;$tGitTop;g" $HOME/.gitproj-test.config
    fi
} # fCreateTestEnv

# ========================================
fComSetup
