#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawOrigin gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fRemoteSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}

    # Assumes we in git dir, and one that is managed by gitproj.
    if ! fComGetVer; then
        fError -m "Quitting" -l $tSrc:$LINENO
    fi

    gpAuto=${gpAuto:-0}
    gpMountDir=""
    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "false")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpLocalTopDir=$($cGetTopDir 2>/dev/null)
    if [ -z "$gpLocalTopDir" ]; then
        gpLocalTopDir=$PWD
    fi

    gpProjName=$(fComGetConfig -k "gitproj.config.proj-name" -d "Error")
    if [ "$gpProjName" = "Error" ]; then
        fError -m "It looks like 'git proj init' did not finish. Config proj-name should be 'defined' " -l $tSrc:$LINENO
    fi

    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "TBD")
    if [ "$gpLocalStatus" != "defined" ]; then
        fError -m "It looks like 'git proj init' did not finish. Config local-status should be 'defined' " -l $tSrc:$LINENO
    fi

    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "TBD")
    if [ "$gpRemoteStatus" != "TBD" ]; then
        fLog -f -p warning -m "It looks like a remote has already been setup with: $gpRemoteRawOrigin. Currently gitproj can handle only one remote." -l $tSrc:$LINENO
        if [ $gpAuto -ne 0 ]; then
            fError -m "You are in 'automatic' mode, so exiting. To force an 'override' set 'remote-status = not-defined' in --local config" -l $tSrc:$LINENO
        fi
    fi

    gpRemoteRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-origin" -d "TBD")
    if [ "$gpRemoteRawOrigin" != "TBD" ]; then
        fLog -f -p warning -m "Expected: TBD, but gitproj.config.remote-raw-origin=$gpRemoteRawOrigin" -l "$tSrc:$LINENO"
    fi

    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #2g

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

#TBD, verify the dir is not in a git workspace!

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
    local tList
    export gResponse=""

    if [ -n "$pMountDir" ]; then
        tList=$(find $pMountDir $pMountDir/* -prune -type d 2>/dev/null | grep -v ' ' | sort -uf)
        gResponse="$gResponse $tList"
    fi

    tList=$(
        for i in $(
            df -h 2>/dev/null |
                grep -E '^/dev/|/mnt|/media' |
                grep -Ev '/$|/boot|/home' |
                awk '{print $6}'
        ); do
            find $i $i/* $i/*/* -prune -type d 2>/dev/null | grep -v ' '
        done | sort -uf
    )
    gResponse="$gResponse $tList"

    return 0
} # fRemoteGetDirList

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
    local tStatus

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
    tHelp=$(
        cat <<EOF

This is a list of dirs under the -d pMountDir, if you gave that
option.  Other mounted directories might be listed too. If you don't
see any directories listed, or one want, select OTHER, to type in the
name of top MountDir.

EOF
    )
    # Loop until the selection is OK, or Quit
    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop
        fComSelect "$tPrompt" "OTHER $tDirList" "$tHelp"
        tStatus=$?
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
        if [ $tStatus -ne 0 ]; then
            continue
        fi
        if ! fRemoteCheckDir $tMountDir; then
            continue
        fi
        if ! fRemoteCheckDirSpace "$tMountDir" "$gpRemoteMinSpace"; then
            continue
        fi
        fComYesNo "Use: $tMountDir"
        if [ $? -ne 0 ] then
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
    local tOpt="-azC"

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
    fComSetConfig -l -k "branch.develop.merge" -v "refs/heads/develop"

    mkdir -p $gpRemoteRawOrigin
    if [ $gpVerbose -ge 1 ]; then
        tVervose=-v
    fi
    if [ $gpVerbose -ge 2 ]; then
        tVervose=-viP
    fi
    fLog -p notice -m "'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawOrigin"
    'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawOrigin
    if [ $? -ne 0 ]; then
        fLog -p warning -m "Was there a problem with rsync?" -l $tSrc:$LINENO
    fi

    fComSetConfig -L -k "gitproj.config.remote-raw-origin" -v "$gpRemoteRawOrigin"
    fComSetConfig -l -k "gitproj.config.remote-raw-origin" -v "$gpRemoteRawOrigin"
    fComSetConfig -l -k "gitproj.config.local-host" -v "$HOSTNAME"

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

    cd $gpLocalTopDir >/dev/null 2>&1
    tGitOrigin=$($cGetOrigin)
    if [ -z "$tGitOrigin" ]; then
        fError -i -n -m "Git origin is not set." -l $tSrc:$LINENO
    fi

    tProjName=${tGitOrigin##*/}
    tProjName=${tProjName%.git}
    if [ "$tProjName" != "$gpProjName" ]; then
        fError -i -n -m "Origin name ($tProjName) does not match project name ($gpProjName)." -l $tSrc:$LINENO
    fi

    tRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-origin" -d "TBD")
    if [ "$tRawOrigin" = "TBD" ]; then
        fError -i -n -m "gitproj.config.remote-raw-origin is not defined." -l $tSrc:$LINENO
    fi
    if [ -z "$gpRemoteRawOrigin" ]; then
        fError -i -n -m "gitproj.config.remote-raw-origin is not defined." -l $tSrc:$LINENO
    fi
    if [ ! -d $gpRemoteRawOrigin ]; then
        fError -i -n -m "$gpRemoteRawOrigin does not exist." -l $tSrc:$LINENO
    fi
    if [ "${gpRemoteRawOrigin##*/}" != "$gpProjName.raw" ]; then
        fError -i -n -m "$gpRemoteRawOrigin does not match $gpProjName.raw" -l $tSrc:$LINENO
    fi
    if [ "$tRawOrigin" != "$gpRemoteRawOrigin" ]; then
        fError -i -n -m "$tRawOrigin != $gpRemoteRawOrigin" -l $tSrc:$LINENO
    fi

    if [ $gErr -ne 0 ]; then
        fError -i -m "The above errors need to be fixed." -l $tSrc:$LINENO
    fi

    # All looks OK, so set remote-status to "defined"
    gpRemoteStatus="defined"
    fComSetConfig -l -k "gitproj.config.remote-status" -v "$gpRemoteStatus"
    fComSetConfig -L -k "gitproj.config.remote-status" -v "$gpRemoteStatus"

    echo 'Remote origin is now set to:'
    $cGetOrigin
    cat <<EOF | fComFmt

Be sure the disk is mounted and that $tGitOrigin and $gpRemoteRawOrigin
exists.  Then you can use: "git push origin BRANCH" and "git pull
origin BRANCH" If the mount path is changed or you are on a different
system, use "git proj config" to update the git and raw origin paths.

EOF
    return 0
} # fRemoteReport

# --------------------
fRemoteCommit()
{
    cd $gpLocalTopDir >/dev/null 2>&1
    fComGit add .gitproj
    fComGit commit -am "git proj remote has been setup"
    fComGit push origin develop
    fComGit checkout main
    fComGit merge develop
    fComGit push origin main
    fComGit checkout develop
    return 0
} # fRemoteCommit

# --------------------
fRemoteCreateRemoteGit()
{
    local pMountDir="$1"
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-remote -d pMountPath"
    # Prompt/Response or if -a, validate the options
    # fRemoteSetGlobals() has verified we are in a git proj repo

    if ! fComGetVer; then
        fError -m "Quitting" -l $tSrc:$LINENO
    fi

#TBD - check to see if a remove-raw-origin or git origin have already
# been setup. If no, continue. If yes, ask for override.

    # -d pMountPath (set with prompt/responnse, or required if -a)
    #  gpMountDir is required with -a, othherwise it is optional
    fRemoteGetMountDir "$gpMountDir"
    gpMountDir=$gResponse

    # Define the remote raw dir name (currenly hardcoded)
    fRemoteGetRemoteRawDir
    gpRemoteRawOrigin=$gResponse

    fRemoteMkRemote $gpMountDir
    fRemoteReport
    fRemoteCommit

    return 0
} # fRemoteCreateRemoteGit

# ========================================
# Don't forget to call fRemoteSetGlobals
# Not done here, so some functions can be tested "stand-alone"
