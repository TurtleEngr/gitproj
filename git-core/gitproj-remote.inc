#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose

# ========================================

# --------------------
fRemoteSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTrim

    # TBD, first are we in git dir?

    gpVer=$(fComGetConfig -k "gitproj.config.ver")
    # Only the first two # are important
    tTrim=${gpVer#*.*.}
    gpVer=${gpVer%.$tTrim}

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpAuto=0

    gpLocalTopDir=$(fComGetConfig -k "gitproj.config.top-dir" -d "$PWD")
    # TBD, could different, if on another system, fix up and save in
    # host config. Use cGetOrigin

    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "Error")

    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")

    # TBD: should be "not-defined". If defined. see if they want:
    # multiple-defined?
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")

    gpRemoteMinSpace=$(fComGetConfig -k "gitproj.config.remote-min-space" -d "20g")
    gpRemoteRawDir=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "TBD")
    gpMountDir=TBD

    return 0
} # fRemoteSetGlobals

# --------------------
fRemoteCheckDir()
{
    local pPath=$1

    local tSrc=${BASH_SOURCE##*/}

    if [ ! -d $pPath ]; then
        fError -n -m "Could not find: $pPath" -l $tSrc:$LINENO
        return 1
    fi
    if [ ! -w $pPath ]; then
        fError -n -m "$pPath is not writable for you." -l $tSrc:$LINENO
        return 1
    fi
    if [ -e $pPath/$gpProjName.git ]; then
        fError -n -m "$pPath/$gpProjName.git already exists." -l $tSrc:$LINENO
        return 1
    fi
    return 0
} # fRemoteCheckDir

# --------------------
fRemoteGetMountDir()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ $gpAuto -ne 0 ]; then
        if [ -z "$gpMountDir" ]; then
            fError -m "The -d option is required." -l $tSrc:$LINENO
        fi
        if ! fRemoteCheckDir $gpMountDir; then
            return 1
        fi
        return 0
    fi

    return 0
} # fRemoteGetMountDir

# --------------------
fRemoteCheckDirSpace()
{
    local pPath=$1
    local pMinSpace=$2

    local tSrc=${BASH_SOURCE##*/}
    local tAvail
    local tGitSize
    local tRawSize
    local tDesiredSize

    tAvail=$(df -BM $pPath --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm .git | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm raw | awk '{print $1}')
    let tRawSize*=2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tDesiredSize -gt $tAvail ]; then
        fError -m "There is not enough space at $pPath. $pPath has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi
    return 0
} # fRemoteCheckDirSpace

# --------------------
fRemoteGetMinSpace()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ $gpAuto -ne 0 ]; then
        if ! fRemoteCheckDirSpace $gpMountDir $gpRemoteMinSpace; then
            echo TBD
            return 1
        fi
        return 0
    fi
    return 0
} # fRemoteGetMinSpace

# --------------------
fRemoteGetRemoteRawDir()
{
    local tSrc=${BASH_SOURCE##*/}

    echo TBD
    #$gpRemoteRawDir

    return 0
} # fRemoteGetRawRemotePath

# --------------------
fRemoteMkRemote()
{
    local pPath=$1

    local tSrc=${BASH_SOURCE##*/}
    local tSrcPath=$PWD
    local tProjName=${PWD##*/}

    fLog -p info -m "git clone" -l $tSrc:$LINENO
    cd $pPath >/dev/null 2>&1
    git clone --bare --local --no-hardlinks --progress -v $tSrcPath/.git
    cd - >/dev/null 2>&1

    fLog -p info -m "Set remote origin" -l $tSrc:$LINENO
    git remote add origin $pPath/$tProjName.git

    return 0
} # fRemoteMkRemote

# --------------------
fRemoteReport()
{
    local tSrc=${BASH_SOURCE##*/}

    echo 'Remote origin is now set to:'
    $cGetOrigin
    cat <<EOF
Be sure the disk is mounted and $gpPath exists.
Then you can use: "git push origin develop"
If the mount path is changed, use "git proj set -e" to update the origin.
EOF
    return 0
} # fRemoteReport

# --------------------
fRemoteCreateRemoteGit()
{
    # Called by: "git proj remote -d pMountPath"
    local tSrc=${BASH_SOURCE##*/}

    # Prompt/Response or if -a, validate the options

    if ! fComMustBeInGitRepo $PWD; then
        fUsage usage
    fi

    # TBD: check for if already defined

    # -d pMountPath
    fRemoteGetMountDir $gpMountDir

    # -s pMinSpace
    fRemoteGetMinSpace $gpRemoteMinSpace

    # -r pRawRemotePath
    fGetRemoteRawDir $gpRemoteRawDir

    fRemoteMkRemote $gpPath
    fRemoteReport

    return 0
} # fRemoteCreateRemoteGit

# ========================================
export tSrc=${BASH_SOURCE##*/}
fRemoteSetGlobals
