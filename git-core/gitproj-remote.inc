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

    # Assumes we in git dir, and one that is managed by gitproj.
    if ! fComMustBeInProjRepo $PWD; then
        fUsage usage
        exit 1
    fi

    gpAuto=${gpAuto:-0}
    gpMountDir=""
    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)
    gpLocalTopDir=$(fComGetConfig -k "gitproj.config.top-dir" -d "$PWD")
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "Error")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteRawDir=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "TBD")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")
    gpRemoteMinSpace=$(fComGetConfig -k "gitproj.config.remote-min-space" -d "20g")

    return 0
} # fRemoteSetGlobals

# --------------------
fRemoteCheckDir()
{
    local pDir=$1

    local tSrc=${BASH_SOURCE##*/}

    if [ ! -d $pDir ]; then
        fError -n -m "Could not find: $pDir" -l $tSrc:$LINENO
        return 1
    fi
    if [ ! -w $pDir ]; then
        fError -n -m "$pDir is not writable for you." -l $tSrc:$LINENO
        return 1
    fi
    if [ -e $pDir/$gpProjName.git ]; then
        fError -n -m "$pDir/$gpProjName.git already exists." -l $tSrc:$LINENO
        return 1
    fi
    if [ -e $pDir/$gpProjName.raw ]; then
        fError -n -m "$pDir/$gpProjName.raw already exists." -l $tSrc:$LINENO
        return 1
    fi
    return 0
} # fRemoteCheckDir

# --------------------
fRemoteGetMountDir()
{
    local pMountDir
    
    local tSrc=${BASH_SOURCE##*/}
    local tTopDirf

    fComGetVer
    tTopDir=$($cGetTopDir)
    
    if [ "$gpLocalStatus" != "defined" ]; then
        fError -m "It looks like 'git proj init' did not finish. local-status should be 'defined' " -l $tSrc:$LINENO
    fi

    if [ "$gpLocalTopDir" != "$tTopDir" ]; then
        fError -m "This repo appears to be configured for $gpLocalTopDir, not $tTopDir. Run 'git proj config' to reconfigure." -l $tSrc:$LINENO
    fi

    if [ "$gpRemoteStatus" = "defined" ]; then
        # if auto error
        fLog -p warning "It looks like a remote has already been setup with: $gpRemoteRawDir. Currently gitproj can handle only one remote." -l $tSrc:$LINENO
	if [ $gpAuto -ne 0 ]; then
	    fError -m "You in in 'automatic' mode, so exiting. To force an 'override' set 'remote-status = not-defined' in $cConfigHost" -l $tSrc:$LINENO
	fi
    fi

    if [ $gpAuto -ne 0 ]; then
        if [ -z "$gpMountDir" ]; then
            fError -m "With the -a option the -d option is required." -l $tSrc:$LINENO
        fi
# TBD Stopped here, for "raw" refactor Thu Nov 18 21:47:59 PST 2021 
	gpRemoteRawDir=$gpMountDir/$gpProjName.raw
        if ! fRemoteCheckDir $gpMountDir; then
            return 1
        fi
        return 0
    fi

#    cDatMount1=$cTestDestDir/test/root/mnt/disk-2
#    cDatMount2=$cTestDestDir/test/root/mnt/usb-misc/files-2021-08-12
#    cDatMount3=$cTestDestDir/test/root/mnt/usb-video/video-2020-04-02
#    df -h | grep -E '^/dev/|/mnt'

    return 0
} # fRemoteGetMountDir

# --------------------
fRemoteCheckDirSpace()
{
    local pDir=$1
    local pMinSpace=$2

    local tSrc=${BASH_SOURCE##*/}
    local tAvail
    local tGitSize
    local tRawSize
    local tDesiredSize

    tAvail=$(df -BM $pDir --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm .git | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm raw | awk '{print $1}')
    let tRawSize*=2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tDesiredSize -gt $tAvail ]; then
        fError -m "There is not enough space at $pDir. $pDir has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB" -l $tSrc:$LINENO
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
    local pDir=$1

    local tSrc=${BASH_SOURCE##*/}
    local tSrcPath=$PWD
    local tProjName=${PWD##*/}

    fLog -p info -m "git clone" -l $tSrc:$LINENO
    cd $pDir >/dev/null 2>&1
    git clone --bare --local --no-hardlinks --progress -v $tSrcPath/.git
    cd - >/dev/null 2>&1

    fLog -p info -m "Set remote origin" -l $tSrc:$LINENO
    git remote add origin $pDir/$tProjName.git

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
Then you can use: "git push origin BRANCH" and "git pull origin BRANCH"
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

    # fRemoteSetGlobals() has verified we are in a git proj repo

    # -d pMountPath
    fRemoteGetMountDir $gpMountDir

    # -s pMinSpace
    fRemoteGetMinSpace $gpRemoteMinSpace

    # -r pRawRemotePath
    fGetRemoteRawDir $gpRemoteRawDir

    fRemoteMkRemote $gpPath?
    fRemoteReport

    return 0
} # fRemoteCreateRemoteGit

# ========================================
export tSrc=${BASH_SOURCE##*/}
fRemoteSetGlobals
