#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fPushIsRemoteMounted()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ -d $gpRemoteRawDir ]; then
        return 0
    fi

    fError -m "$gpRemoteRawDir was not found. Try again after mounting it or run 'git proj config' to change the remote.raw.dir location." -l $tSrc:$LINENO
} # fPushIsRemoteMounted

# --------------------
fPushRawFiles()
{
    local tVerbose
    local tSrc=${BASH_SOURCE##*/}
    local tOpt="-azC --delete-after"
    local tStatus
    
    if [ $gpVerbose -ge 2 ]; then
        tVervose=-viP
    fi

    fLog -p notice -m "'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpLocalTopDir/raw/* $gpRemoteRawDir"

    'rsync' $tOpt $tVerbose --dry-run -iv $gpLocalTopDir/raw/ $gpRemoteRawDir
    echo -n "Continue (y/n)? "
    read
    if [ "$REPLY" != 'y' ]; then
        fLog -p warning -m "Nothing was pushed." -l $tSrc:$LINENO
	exit 1
    fi

    'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawDir
    tStatus=$?

    return $tStatus
} # fPushRawFiles()

# --------------------
fPushGit()
{
    local pPushBranch="$1"
    local tSrc=${BASH_SOURCE##*/}
    
    if [ $pPushBranch -eq 0 ]; then
        return 0
    fi

    # tBranch = current branch
    # git push origin $tBranch

    return 0
} # fPushGit()

# --------------------
fPushToOrigin()
{
    local pPushBranch="$1"
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-push [-b]

    if [ $cInteractive -eq 0 ]; then
        fError -m "You need to be 'interactive' for this command." -l $tSrc:$LINENO
    fi

    fComGetProjGlobals
    fPushIsRemoteMounted
    fPushRawFiles
    fPushGit

    return 0
} # fPushToOrigin

# ========================================
# Don't forget to call fComGetProjGlobals
# Not done here, so some functions can be tested "stand-alone"
