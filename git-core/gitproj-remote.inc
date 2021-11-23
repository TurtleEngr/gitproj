#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gSelectResponse

# ========================================

# --------------------
fRemoteSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}

##    gpCmdName=git-proj-remote
    
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
fRemoteVerifyState()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir

    fComGetVer

    if [ "$gpLocalStatus" != "defined" ]; then
        fError -m "It looks like 'git proj init' did not finish. Config local-status should be 'defined' " -l $tSrc:$LINENO
    fi

    tTopDir=$($cGetTopDir)
    if [ "$gpLocalTopDir" != "$tTopDir" ]; then
        fError -m "This repo appears to be configured for $gpLocalTopDir, not $tTopDir. Run 'git proj config' to reconfigure for a different host." -l $tSrc:$LINENO
    fi
    
    if [ "$gpRemoteStatus" = "defined" ]; then
        fLog -p warning -m "It looks like a remote has already been setup with: $gpRemoteRawDir. Currently gitproj can handle only one remote." -l $tSrc:$LINENO
	if [ $gpAuto -ne 0 ]; then
	    fError -m "You are in 'automatic' mode, so exiting. To force an 'override' set 'remote-status = not-defined' in $cConfigHost" -l $tSrc:$LINENO
	fi
    fi

    if [ "$gpRemoteStatus" = "not-defined" ] && \
       [ "$gpRemoteRawDir" != "TBD" ] && \
       [ -n "$gpRemoteRawDir" ]; then
        fError -i -m "Config problem remote-status=not-defined, but remote-raw-dir is set to $gpRemoteRawDir. It should be undefined or set to TBD" -l $tSrc:$LINENO
    fi

    return 0
} # fRemoteVerifyState

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
fRemoteCheckDirSpace()
{
    local pMountDir=$1
    local pMinSpace=$2

    local tSrc=${BASH_SOURCE##*/}
    local tAvail
    local tGitSize
    local tRawSize
    local tDesiredSize

    tAvail=$(df -BM $pMountDir --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm $gpLocalTopDir/.git | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm $gpLocalTopDir/raw | awk '{print $1}')
    let tRawSize*=2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tDesiredSize -gt $tAvail ]; then
        fError -m "There is not enough space at $pMountDir. $pMountDir has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi

    return 0
} # fRemoteCheckDirSpace

# --------------------
fRemoteGetDirList()
{
    local pMountDir="$1"

    local tSrc=${BASH_SOURCE##*/}

    if [ -n "$pMountDir" ]; then
        find $pMountDir $pMountDir/* $pMountDir/*/* -prune -type d 2>/dev/null | grep -v ' '
    fi
    for i in $(
	    df -h | \
	    grep -E '^/dev/|/mnt|/media' | \
	    grep -Ev '/$|/boot' | \
	    awk '{print $6}'
	); do
	find $i $i/* $i/*/* -prune -type d 2>/dev/null | grep -v ' '
    done | sort -f
    echo
    return 0
} # fRemoteGetDirList

# --------------------
fRemoteSelect()
{
    PS3="$1"
    local pDirList="$2"
    local pHelp="$3"

    local tSrc=${BASH_SOURCE##*/}
    local tResponse
    export gSelectResponse

    # Loop until a valid selection, or QUIT or OTHER
    select tResponse in QUIT HELP OTHER $pDirList; do
        case $tResponse in
	    QUIT)
	        fLog -p warning -m "Quitting." -l $tSrc:$LINENO
	        tStatus=1
		break
	        ;;
	    HELP)
		echo  1>&2
	        echo "$pHelp" 1>&2
		echo  1>&2
		continue
	        ;;
	    OTHER)
	        tStatus=1
		break
	        ;;
	esac
	if [ -n "$tResponse" ]; then
	    tStatus=0
	    break
	fi
    done

    PS3=""
    gSelectResponse=$tResponse
    return $tStatus
} # fRemoteSelect

# --------------------
fRemoteGetAnotherMountDir()
{
    local pMountDir=$1
    
    local tSrc=${BASH_SOURCE##*/}

    # Loop until the selection is OK, or quit
    while true; do
        echo -n "Mount Path [$pMountDir | q]? "
        read -r pMountDir
	if [ "$pMountDir" = "quit" ] || [ "$pMountDir" = "q" ]; then
      	    fLog -p warning -m "Quitting" -l $tSrc:$LINENO
	    exit 1
	fi
        if [ ! fRemoteCheckDir $pMountDir ]; then
	    continue
        fi
        if ! fRemoteCheckDirSpace "$pMountDir" "$gpRemoteMinSpace"; then
	    continue
	fi
	break
    done
    echo $pMountDir
    return 0
} # fRemoteGetAnotherMountDir

# --------------------
fRemoteGetMountDir()
{
    local pMountDir=$1
    
    local tSrc=${BASH_SOURCE##*/}
    local tDirList
    local tResponse
    export gSelectResponse

    # ----------
    if [ $gpAuto -ne 0 ]; then
        if [ -z "$pMountDir" ]; then
            fError -m "The -d option is required with -a auto option." -l $tSrc:$LINENO
        fi
        if ! fRemoteCheckDir $pMountDir; then
            fError -m "Cannot continue with -a mode." -l $tSrc:$LINENO
        fi
        if ! fRemoteCheckDirSpace "$pMountDir" "$gpRemoteMinSpace"; then
            fError -m "Cannot continue with -a mode." -l $tSrc:$LINENO
	fi
	gpMountDir="$pMountDir"
        return 0
    fi

    # ----------
    # Prompt/response
    # Offer choices, with two dir levels (remove dirs with spaces in names)
    tDirList=$(fRemoteGetDirList $pMountDir)
    tPrompt="Select by number, the location for the remote git and raw files? "
    tHelp=$(cat <<EOF
This is a list of dirs under the -d pMountDir, if you gave that option.
Other mounted directories might be listed too.
Select OTHER, to give another pMountDir.
EOF
)
    # Loop until the selection is OK, or Quit
    while true; do
        fRemoteSelect "$tPrompt" "$tDirList" "$tHelp"
	tMountDir=$gSelectResponse
	if [ "$tMountDir" = "QUIT" ]; then
	    exit 1
	fi
	if [ "$tMountDir" = "OTHER" ]; then
	    tMountDir=$(fRemoteGetAnotherMountDir $pMountDir)
    	    tDirList=$(fRemoteGetDirList $tMountDir)
	    continue
	fi
        if [ ! fRemoteCheckDir $tMountDir ]; then
	    continue
        fi
        if ! fRemoteCheckDirSpace "$tMountDir" "$gpRemoteMinSpace"; then
	    continue
	fi
    done

    gpMountDir="$tMountDir"
    return 0
} # fRemoteGetMountDir

fRemoteGetRemoteRawDir()
{
    local tSrc=${BASH_SOURCE##*/}

    gpRemoteRawDir=$gpMountDir/$gpProjName.raw
    if [ ! -d $gpRemoteRawDir ]; then
        fError -m "pMountDir/ProjName.raw already exists."
    fi

    return 0
} # fRemoteGetRawRemotePath

# --------------------
fRemoteMkRemote()
{
    local pMountDir=$1

    local tSrc=${BASH_SOURCE##*/}
    local tProjName=${PWD##*/}
    local tVerbose

    cd $pMountDir >/dev/null 2>&1
    fLog -p notice -m "git clone" -l $tSrc:$LINENO
    git clone --bare --local --no-hardlinks --progress -v $gpLocalTopDir/.git
    cd - >/dev/null 2>&1

    fLog -p info -m "Set remote origin" -l $tSrc:$LINENO
    git remote add origin $pMountDir/$tProjName.git
    git config branch.main.remote origin
    git config branch.main.merge refs/heads/main
    git config branch.develop.remote origin
    git config branch.develop.merge refs/heads/main

    mkdir -p $gpRemoteRawDir
    if [ $gpVerbose -ge 1 ]; then
        tVervose=-v
    fi
    if [ $gpVerbose -ge 2 ]; then
        tVervose=-vP
    fi
    rLog -p notice -m "'rsync'-rlptz $tVerbose $gpLocalTopDir $gpLocalTopDir/raw/* $gpRemoteRawDir/"
    'rsync'-rlptz $tVerbose $gpLocalTopDir/raw/* $gpRemoteRawDir/
    if [ $? -ne 0 ]; then
        fLog -p warning -m "Was there a problem with rsync?" -l $tSrc:$LINENO
    fi

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
If the mount path is changed, use "git proj config" to update the origin.
EOF
    return 0
} # fRemoteReport

# --------------------
fRemoteCreateRemoteGit()
{
    local pMountPath="$1"

    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-remote -d pMountPath"
    # Prompt/Response or if -a, validate the options
    # fRemoteSetGlobals() has verified we are in a git proj repo

    fRemoteVerfiyState

    # -d pMountPath (set with prompt/responnse, or required if -a)
    fRemoteGetMountDir "$gpMountDir"
    # gpMountDir has now been validated for use

    # -r pRawRemotePath
    fGetRemoteRawDir $gpRemoteRawDir

    fRemoteMkRemote $gpPath?
    fRemoteReport

    return 0
} # fRemoteCreateRemoteGit

# ========================================
# Don't forget to call fRemoteSetGlobals
# Not done here, so some functions can be tested "stand-alone"
