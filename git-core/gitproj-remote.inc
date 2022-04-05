#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cRemoteProjFile cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMaxLoop \
    gpPath gpProjName gpProjStatus gpRemoteRawOrigin gpRemoteStatus \
    gpSysLog gpVer gpVerbose gpYesNo gResponse

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

    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #2g

    return 0
} # fRemoteSetGlobals

# --------------------
fRemoteCheckDir()
{
    local pDir=$1
    local tSrc=${BASH_SOURCE##*/}
    local tRemoteGitProj

    if [ ! -d $pDir ]; then
        fError -n -m "Could not find: $pDir" -l $tSrc:$LINENO
        return 1
    fi

    if [ ! -w $pDir ]; then
        fError -n -m "$pDir is not writable for you." -l $tSrc:$LINENO
        return 1
    fi

    if ! fComMustNotBeInGit $pDir >/dev/null 2>&1; then
        fError -n -m "$pDir cannot be in another git dir." -l $tSrc:$LINENO
        return 1
    fi

    if [ -e $pDir/$gpProjName.git ]; then
        fError -n -m "$pDir/$gpProjName.git already exists." -l $tSrc:$LINENO
        return 1
    fi

    if [ -e $pDir/$gpProjName.raw ]; then
        tRemoteGitProj=$(cat $pDir/$gpProjName.raw/$cRemoteProjFile)
        fError -n -m "$pDir/$gpProjName.raw already exists, and belongs to remote: $tRemoteGitProj" -l $tSrc:$LINENO
        return 1
    fi

    if [[ "$pDir" =~ (.*\.git$|.*\.git/) ]]; then
        fError -n -m "$pDir is in another remote git dir." -l $tSrc:$LINENO
        return 1
    fi

    if [[ "$pDir" =~ (.*\.raw$|.*\.raw/) ]]; then
        fError -n -m "$pDir is in another remote raw dir." -l $tSrc:$LINENO
        return 1
    fi

    return 0
} # fRemoteCheckDir

# --------------------
fRemoteGetDirList()
{
    local pMountDir="$1"
    local tSrc=${BASH_SOURCE##*/}
    local tList
    export gResponse=""

    if [ -n "$pMountDir" ]; then
        tList=$(find $pMountDir $pMountDir/* -prune -type d 2>/dev/null | grep -v ' ' | sort -u)
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
        done | sort -u
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
    local tMaxLoop=$gpMaxLoop
    gResponse=""

    # Loop until the selection is OK, or quit
    tMaxLoop=$gpMaxLoop
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
        if ! fComCheckDirSpace "$tReply" "$gpRemoteMinSpace"; then
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
    local tMaxLoop=$gpMaxLoop
    local tMountDir
    local tStatus

    # ----------
    if [ ${gpAuto:0} -ne 0 ]; then
        if [ -z "$pMountDir" ]; then
            fError -m "The -d option is required with -a auto option." -l $tSrc:$LINENO
        fi
        if ! fRemoteCheckDir $pMountDir; then
            fError -m "Cannot continue with -a mode." -l $tSrc:$LINENO
        fi
        if ! fComCheckDirSpace "$pMountDir" "$gpRemoteMinSpace"; then
            fError -m "Cannot continue with -a mode." -l $tSrc:$LINENO
        fi
	cd $pMountDir >/dev/null 2>&1
        gResponse="$PWD"
	cd - >/dev/null 2>&1
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
    tMaxLoop=$gpMaxLoop
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
        if ! fComCheckDirSpace "$tMountDir" "$gpRemoteMinSpace"; then
            continue
        fi
        if ! fComYesNo "Use: $tMountDir"; then
            continue
        fi
        break
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    cd $pMountDir >/dev/null 2>&1
    gResponse="$PWD"
    cd - >/dev/null 2>&1
    return 0
} # fRemoteGetMountDir

# --------------------
fRemoteGetRemoteRawDir()
{
    local pMountDir="$1"
    local tSrc=${BASH_SOURCE##*/}

    # pMountDir has already been validated
    # pProjName has already been validated
    # $pMountDir/$gpProjName.raw should have been verified to not exist

    gResponse=$pMountDir/$gpProjName.raw
    if [ -e $gResponse ]; then
        fError -i -m "$pMountDir/$gpProjName.raw already exists." -l $tSrc:$LINENO
    fi

    return 0
} # fRemoteGetRemoteRawDir

# --------------------
fRemoteMkRemote()
{
    local pMountDir=$1
    local tSrc=${BASH_SOURCE##*/}
    local tProjName=${PWD##*/}
    local tVerbose=""
    local tOpt="-az"

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
        tVerbose=-v
    fi
    if [ $gpVerbose -ge 2 ]; then
        tVerbose=-viP
    fi
    fLog -p notice -m "'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawOrigin"
    'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawOrigin
    if [ $? -ne 0 ]; then
        fLog -p warning -m "Was there a problem with rsync?" -l $tSrc:$LINENO
    fi

    # TODO test this
    # Special file used to verify the remote raw/ belongs with this git
    echo "$($cGetOrigin)" >$gpLocalTopDir/raw/$cRemoteProjFile
    cp $gpLocalTopDir/raw/$cRemoteProjFile $gpRemoteRawOrigin/$cRemoteProjFile

    fComSetConfig -L -k "gitproj.config.remote-raw-origin" -v "TBD"
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
    local tRemoteGitProj
    local tGitOrigin=$($cGetOrigin)
    local tErr=0

    # Double check the settings

    cd $gpLocalTopDir >/dev/null 2>&1
    tGitOrigin=$($cGetOrigin)
    if [ -z "$tGitOrigin" ]; then
	tErr=1
        fError -i -n -m "Git origin is not set." -l $tSrc:$LINENO
    fi

    tProjName=${tGitOrigin##*/}
    tProjName=${tProjName%.git}
    if [ "$tProjName" != "$gpProjName" ]; then
	tErr=1
        fError -i -n -m "Origin name ($tProjName) does not match project name ($gpProjName)." -l $tSrc:$LINENO
    fi

    tRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-origin" -d "TBD")
    if [ "$tRawOrigin" = "TBD" ]; then
	tErr=1
        fError -i -n -m "gitproj.config.remote-raw-origin is not defined." -l $tSrc:$LINENO
    fi
    if [ -z "$gpRemoteRawOrigin" ]; then
	tErr=1
        fError -i -n -m "gitproj.config.remote-raw-origin is not defined." -l $tSrc:$LINENO
    fi
    if [ ! -d $gpRemoteRawOrigin ]; then
	tErr=1
        fError -i -n -m "$gpRemoteRawOrigin does not exist." -l $tSrc:$LINENO
    fi
    if [ "${gpRemoteRawOrigin##*/}" != "$gpProjName.raw" ]; then
	tErr=1
        fError -i -n -m "$gpRemoteRawOrigin does not match $gpProjName.raw" -l $tSrc:$LINENO
    fi
    if [ "$tRawOrigin" != "$gpRemoteRawOrigin" ]; then
	tErr=1
        fError -i -n -m "$tRawOrigin != $gpRemoteRawOrigin" -l $tSrc:$LINENO
    fi

    if [ ! -f $gpRemoteRawOrigin/$cRemoteProjFile ]; then
	tErr=1
        fError -i -n -m "Missing: $gpRemoteRawOrigin/$cRemoteProjFile" -l $tSrc:$LINENO
    else
        tRemoteGitProj=$(cat $gpRemoteRawOrigin/$cRemoteProjFile)
    	if [ "$tRemoteGitProj" != "$tGitOrigin" ]; then
	    tErr=1
            fError -i -n -m "$tRemoteGitProj != $tGitOrigin" -l $tSrc:$LINENO
    	fi
    fi

    if [ $tErr -ne 0 ]; then
        fError -i -m "The above errors need to be fixed." -l $tSrc:$LINENO
    fi

    # All looks OK, so set remote-status to "defined"
    gpRemoteStatus="defined"
    fComSetConfig -l -k "gitproj.config.remote-status" -v "$gpRemoteStatus"
    fComSetConfig -L -k "gitproj.config.remote-status" -v "TBD"

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
fRemoteStatus()
{
    local tSrc=${BASH_SOURCE##*/}
    local tRemoteStatus
    local tRawOrigin
    local tGitOrigin

    tRemoteStatus=$(fComGetConfig -k gitproj.config.remote-status -d TBD)
    tRawOrigin=$(fComGetConfig -k gitproj.config.remote-raw-origin -d TBD)
    tGitOrigin=$(fComGetConfig -k remote.origin.url -d TBD)

    if [[ "$tRemoteStatus" != TBD || "$tRawOrigin" != TBD || "$tGitOrigin" != TBD ]]; then
        cat <<EOF

It looks like a remote origin already esists:
    gitprog.config.remote-status = $tRemoteStatus
    gitproj.config.remote-raw-origin = $tRawOrigin
    remote.origin.url = $tGitOrigin

EOF
        if [ $gpAuto -ne 0 ]; then
            if [ $gpYesNo = "Yes" ]; then
                return 0
            fi
            return 1
        fi

        # Prompt
        if ! fComYesNo "Replace this with another remote origin"; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
    fi

    return 0
} # fRemoteStatus

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

    fRemoteStatus

    # -d pMountPath (set with prompt/response, or required if -a)
    #  pMountDir is required with -a, otherwise it is optional
    fRemoteGetMountDir "$pMountDir"
    pMountDir=$gResponse

    # Define the remote raw dir name (currently hardcoded)
    fRemoteGetRemoteRawDir "$pMountDir"
    gpRemoteRawOrigin=$gResponse

    echo
    echo "================================================================="
    echo
    fRemoteMkRemote $pMountDir
    echo
    fRemoteReport
    echo
    fRemoteCommit

    echo
    echo "================================================================="
    echo
    echo -n 'git remote origin is now: '
    $cGetOrigin
    echo "raw remote origin is now: $gpRemoteRawOrigin"

    cat <<EOF | fComFmt

Be sure the disk is mounted and that $tGitOrigin and
$gpRemoteRawOrigin exists.  Then you can use: "git push origin BRANCH"
and "git pull origin BRANCH"

If the mount path has changed, or you are on a different system, you
can re-run "git proj remote".

EOF
    return 0
} # fRemoteCreateRemoteGit

# ========================================
# Don't forget to call fRemoteSetGlobals
# Not done here, so some functions can be tested "stand-alone"
