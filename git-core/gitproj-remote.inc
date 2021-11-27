#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

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
    gpLocalTopDir=$($cGetTopDir 2>/dev/null)
    if [ -z "$gpLocalTopDir" ]; then
        gpLocalTopDir=$PWD
    fi
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "Error")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteRawDir=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "TBD")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #2g

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
    let pMinSpace=pMinSpace/1024/1024

    if [ $tAvail -lt $tDesiredSize ] || [ $tAvail -lt $pMinSpace ]; then
        fError -n -m "There is not enough space at $pMountDir, which has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB (or ${pMinSpace}MB)" -l $tSrc:$LINENO
        return 1
    fi

    return 0
} # fRemoteCheckDirSpace

# --------------------
fRemoteGetDirList()
{
    local pMountDir="$1"

    local tSrc=${BASH_SOURCE##*/}
    local tLiist
    export gResponse=""

    if [ -n "$pMountDir" ]; then
        tList=$(find $pMountDir $pMountDir/* $pMountDir/*/* -prune -type d 2>/dev/null | grep -v ' ' | sort -uf)
	gResponse="$gResponse $tList"
    fi

    tList=$(for i in $(
	    df -h 2>/dev/null | \
	    grep -E '^/dev/|/mnt|/media' | \
	    grep -Ev '/$|/boot' | \
	    awk '{print $6}'
	); do
	find $i $i/* $i/*/* -prune -type d 2>/dev/null | grep -v ' '
    done | sort -uf
    )
    gResponse="$gResponse $tList"

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
    export gResponse

    # Loop until a valid selection, or QUIT or OTHER
    select tResponse in QUIT HELP OTHER $pDirList; do
        echo
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
    gResponse=$tResponse
    return $tStatus
} # fRemoteSelect

# --------------------
fRemoteGetAnotherMountDir()
{
    local pMountDir=$1

    local tSrc=${BASH_SOURCE##*/}
    local tReply
    local tMaxLoop=30
    gResponse=""

    # Loop until the selection is OK, or quit
    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop
        echo -ne "\nMount Dir [$pMountDir | q]? "
        read -r tReply
	if [ "$tReply" = "quit" ] || [ "$tReply" = "q" ]; then
      	    fLog -p warning -m "Quitting" -l $tSrc:$LINENO
    	    gResponse=$pMountDir
	    return 1
	fi
	if [ -z "$tReply" ]; then
	    continue
	fi
        if ! fRemoteCheckDir $tReply; then
	    continue
        fi
        if ! fRemoteCheckDirSpace "$tReply" "$gpRemoteMinSpace"; then
	    continue
	fi
	break
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    gResponse=$tReply
    return 0
} # fRemoteGetAnotherMountDir

# --------------------
fRemoteGetMountDir()
{
    local pMountDir="$1"
    
    local tSrc=${BASH_SOURCE##*/}
    export gResponse=""
    local tDirList
    local tMaxLoop=20
    local tMountDir
    local tResponse

    # ----------
    if [ ${gpAuto:0} -ne 0 ]; then
        if [ -z "$pMountDir" ]; then
            fError -m "The -d option is required with -a auto option." -l $tSrc:$LINENO
        fi
        if ! fRemoteCheckDir $pMountDir; then
            fError -m "Cannot continue with -a mode." -l $tSrc:$LINENO
        fi
        if ! fRemoteCheckDirSpace "$pMountDir" "$gpRemoteMinSpace"; then
            fError -m "Cannot continue with -a mode." -l $tSrc:$LINENO
	fi
	gResponse="$pMountDir"
        return 0
    fi

    # ----------
    # Prompt/response
    # Offer choices, with two dir levels (remove dirs with spaces in names)
    fRemoteGetDirList $pMountDir
    tDirList=$gResponse
    tPrompt="Select by number, the location for the remote git and raw files? "
    tHelp=$(cat <<EOF
This is a list of dirs under the -d pMountDir, if you gave that option.
Other mounted directories might be listed too.
Select OTHER, to give another pMountDir.
EOF
)
    # Loop until the selection is OK, or Quit
    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop
        fRemoteSelect "$tPrompt" "$tDirList" "$tHelp"
	tMountDir=$gResponse
	if [ "$tMountDir" = "QUIT" ]; then
	    exit 1
	fi
	if [ "$tMountDir" = "OTHER" ]; then
	    fRemoteGetAnotherMountDir $pMountDir
	    tMountDir=$gResponse
    	    fRemoteGetDirList $tMountDir
	    tDirList=$gResponse
	    continue
	fi
        if ! fRemoteCheckDir $tMountDir; then
	    continue
        fi
        if ! fRemoteCheckDirSpace "$tMountDir" "$gpRemoteMinSpace"; then
	    continue
	fi
	break
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    gResponse="$tMountDir"
    return 0
} # fRemoteGetMountDir

# --------------------
fRemoteGetRemoteRawDir()
{
    local tSrc=${BASH_SOURCE##*/}
    # gpMountDir has already been validated
    # gpProjName has already been validated
    # $gpMountDir/$gpProjName.raw should have be verified to not exist

    gResponse=$gpMountDir/$gpProjName.raw
    if [ -e $gResponse ]; then
        fError -i -m "pMountDir/$gpProjName.raw already exists." -l $tSrc:$LINENO
    fi

    return 0
} # fRemoteGetRemoteRawDir

# --------------------
fRemoteMkRemote()
{
    local pMountDir=$1

    local tSrc=${BASH_SOURCE##*/}
    local tProjName=${PWD##*/}
    local tVerbose

    cd $pMountDir >/dev/null 2>&1
    fLog -p notice -m "git clone to $pMountDir" -l $tSrc:$LINENO
    git clone --bare --local --no-hardlinks --progress -v $gpLocalTopDir/.git
    cd - >/dev/null 2>&1

    fLog -p info -m "Set remote origin" -l $tSrc:$LINENO
    
    git remote add origin $pMountDir/$tProjName.git
    fComSetConfig -l -k "branch.main.remote" -v "origin"
    fComSetConfig -l -k "branch.main.remote" -v "origin"
    fComSetConfig -l -k "branch.main.merge" -v "refs/heads/main"
    fComSetConfig -l -k "branch.develop.remote" -v "origin"
    fComSetConfig -l -k "branch.develop.merge" -v "refs/heads/main"

    mkdir -p $gpRemoteRawDir
    if [ $gpVerbose -ge 1 ]; then
        tVervose=-v
    fi
    if [ $gpVerbose -ge 2 ]; then
        tVervose=-vP
    fi
    fLog -p notice -m "'rsync' -rlptz $tVerbose $gpLocalTopDir/raw/* $gpLocalTopDir/raw/* $gpRemoteRawDir/"
    'rsync' -rlptz $tVerbose $gpLocalTopDir/raw/* $gpRemoteRawDir/
    if [ $? -ne 0 ]; then
        fLog -p warning -m "Was there a problem with rsync?" -l $tSrc:$LINENO
    fi

    fComSetConfig -L -k "gitproj.config.remote-raw-dir" -v "$gpRemoteRawDir"
    fComSetConfig -H -k "gitproj.config.remote-raw-dir" -v "$gpRemoteRawDir"

    return 0
} # fRemoteMkRemote

# --------------------
fRemoteReport()
{
    local tSrc=${BASH_SOURCE##*/}
    local tGitOrigin
    local tRawOrigin
    local tProjName

    # Double check the settings

    gErr=0

    tGitOrigin=$($cGetOrigin)
    if [ -z "$tGitOrigin" ]; then
        fError -i -n -m "Git origin is not set." -l $tSrc:$LINENO
    fi
    
    tProjName=${tGitOrigin##*/}
    if [ "$tProjName" != "$gpProjName" ]; then
        fError -i -n -m "Origin does not match project name." -l $tSrc:$LINENO
    fi

    tRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "TBD")
    if [ "$tRawOrigin" = "TBD" ]; then
        fError -i -n -m "gitproj.config.remote-raw-dir is not defined." -l $tSrc:$LINENO
    fi
    if [ -z "$gpRemoteRawDir" ]; then
        fError -i -n -m "gitproj.config.remote-raw-dir is not defined." -l $tSrc:$LINENO
    fi
    if [ ! -d $gpRemoteRawDir ]; then
        fError -i -n -m "$gpRemoteRawDir does not exist." -l $tSrc:$LINENO
    fi
    if [ "${gpRemoteRawDir##*/}" != "$gpProjName.raw" ]; then
        fError -i -n -m "$gpRemoteRawDir does not match $gpProjName.raw" -l $tSrc:$LINENO
    fi
    if [ "$tRawOrigin" != "$gpRemoteRawDir" ]; then
        fError -i -n -m "$tRawOrigin != $gpRemoteRawDir" -l $tSrc:$LINENO
    fi

    if [ $gErr -ne 0 ]; then
        fError -i -m "The above errors need to be fixed." -l $tSrc:$LINENO
    fi

    # All looks OK, so set remote-status to "defined"
    gpRemoteStatus="defined"
    fComSetConfig -H -k "config gitproj.config.remote-status" -v "$gpRemoteStatus"

    echo 'Remote origin is now set to:'
    $cGetOrigin
    cat <<EOF | fmt

Be sure the disk is mounted and $tGitOrigin and $gpRemoteRawDir exist.
Then you can use: "git push origin BRANCH" and "git pull origin
BRANCH" If the mount path is changed or you are on a different system,
use "git proj config" to update the git and raw origin paths.

EOF
    return 0
} # fRemoteReport

# --------------------
fRemoteCreateRemoteGit()
{
    local pMountDir="$1"

    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-remote -d pMountPath"
    # Prompt/Response or if -a, validate the options
    # fRemoteSetGlobals() has verified we are in a git proj repo

    fRemoteVerfiyState

    # -d pMountPath (set with prompt/responnse, or required if -a)
    #  gpMountDir is required with -a, othherwise it is optional
    fRemoteGetMountDir "$gpMountDir"
    gpMountDir=$gResponse

    # Define the remote raw dir name (currenly hardcoded)
    fRemoteGetRemoteRawDir
    gpRemoteRawDir=$gResponse

    fRemoteMkRemote $gpMountDir
    fRemoteReport

    return 0
} # fRemoteCreateRemoteGit

# ========================================
# Don't forget to call fRemoteSetGlobals
# Not done here, so some functions can be tested "stand-alone"
