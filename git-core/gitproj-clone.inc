#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cRemoteProjFile cTop

export gpAuto gpAutoMove gpBin gpCheckFileNames \
    gpCheckForBigFiles gpCmdName gpDebug gpDoc gpFacility gpGitFlow \
    gpLocalRawDir gpLocalStatus gpLocalTopDir gpMaxSize \
    gpPath gpPreCommitEnabled gpProjName gpProjStatus gpRemoteGitDir \
    gpRemoteRawOrigin gpRemoteStatus gpSysLog gpVer gpVerbose

# ========================================

# --------------------
fCloneGettingStarted()
{
    local tSrc=${BASH_SOURCE##*/}

    cat <<EOF

Be sure you are "cd" to the directory that the project will be cloned to.
Clone git from: $gpRemoteGitDir
Clone raw from: $gpRemoteRawOrigin
Project Name:   $gpProjName
Project Dir:    $PWD/$gpProjName

EOF
    if ! fComYesNo "Continue"; then
        fLog -p warning -m "Not continuing." -l $tSrc:$LINENO
        exit 1
    fi
    fLog -p debug-2 -m "Cloning: $gpProjName" -l $tSrc:$LINENO
    return 0
} #fCloneGettingStarted

# --------------------
fCloneValidRemoteDir()
{
    # Input: possible gpRemoteGitDir
    # If all valid, sets: gpRemoteGitDir, gpRemoteRawOrigin, gpProjName
    local pRemoteGitDir="$1"
    local tSrc=${BASH_SOURCE##*/}
    local tAvail
    local tGitSize
    local tRawSize
    local tDesiredSize
    local tSize
    local tRemoteRawOrigin
    local tProjName

    if ! fComMustNotBeInGit $PWD; then
        return 1
    fi

    if [ ! -w $PWD ]; then
        fError -m "Current dir is not writable" -l $tSrc:$LINENO
    fi

    if [ ! -d $pRemoteGitDir ]; then
        fError -m "$pRemoteGitDir does not exist." -l $tSrc:$LINENO
    fi

    if [ ! -d $pRemoteGitDir/objects ]; then
        fError -m "$pRemoteGitDir is not a git repo." -l $tSrc:$LINENO
    fi

    if [ "${pRemoteGitDir%.git}" = "$pRemoteGitDir" ]; then
        fError -m "$pRemoteGitDir does not have '.git' at end." -l $tSrc:$LINENO
    fi

    if ! fComAllMustBeReadable $pRemoteGitDir; then
        exit 1
    fi
    # Probably valid
    gpRemoteGitDir=$pRemoteGitDir

    # Make it an absolute path, relative paths are messy
    cd $gpRemoteGitDir >/dev/null 2>&2
    gpRemoteGitDir=$PWD
    cd - >/dev/null 2>&2

    tRemoteRawOrigin=${pRemoteGitDir%.git}.raw
    if [ ! -d $tRemoteRawOrigin ]; then
        fError -m "$tRemoteRawOrigin does not exist." -l $tSrc:$LINENO
    fi

    # TODO test this
    # Verify the remote raw belongs to the remote git
    if [ ! -r $tRemoteRawOrigin/$cRemoteProjFile ]; then
        fError -m "Missing $tRemoteRawOrigin/$cRemoteProjFile" -l $tSrc:$LINENO
    fi
    tRemoteGitProj=$(cat $tRemoteRawOrigin/$cRemoteProjFile)
    tRemoteGitProj=${tRemoteGitProj##*/}
    tLocalGitProj=${gpRemoteGitDir##*/}
    if [ "$tRemoteGitProj" != "$tLocalGitProj" ]; then
        fError -m "Mismatch: $tRemoteRawOrigin belongs to $tRemoteGitProj not $tLocalGitProj See file: $cRemoteProjFile" -l $tSrc:$LINENO
    fi

    if ! fComAllMustBeReadable $tRemoteRawOrigin; then
        exit 1
    fi

    # Probably valid
    cd $tRemoteRawOrigin >/dev/null 2>&1
    gpRemoteRawOrigin=$PWD
    cd - >/dev/null 2>&1

    tProjName=${gpRemoteGitDir##*/}
    tProjName=${tProjName%.git}
    if [ -e $tProjName ]; then
        fError -m "A $tProjName already exists in this dir." -l $tSrc:$LINENO
    fi

    # TODO test this
    if [ "${tLocalGitProj%.git}" != "$tProjName" ]; then
        fError -m "$tProjName does not match expected ${tLocalGitProj%.git}" -l $tSrc:$LINENO
    fi

    # Probably valid
    gpProjName=$tProjName

    # Is there enough room in current dir for the clone?
    # s/b 2x size of remote git and raw, or remote-min-size, which
    #  ever is larger.

    tAvail=$(df -BM $PWD --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm $gpRemoteGitDir | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm $gpRemoteRawOrigin | awk '{print $1}')
    let tRawSize*=2

    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") # 2g
    let tSize=gpRemoteMinSpace/1024/1024
    let tDesiredSize=tGitSize+tRawSize
    if [ $tSize -gt $tDesiredSize ]; then
        if [ $tAvail -lt $tSize ]; then
	   # TODO test this
           fLog -f -p warning -m "The current directory should have ${tSize}MB available. Found: ${tAvail}MB" -l $tSrc:$LINENO
	fi
    fi

    if [ $tAvail -lt $tDesiredSize ]; then
        fError -m "${tAvail}MB is not enough space in current directory. Project '$gpProjName' needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi

    return 0
} # fCloneValidRemoteDir

# --------------------
fCloneCheckLocalConfig()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ -f .gitproj ]; then
        return 0
    fi
    fLog -p warning -m "Missing file: .gitproj It should have been versioned! Will try to recreate it from ~/.gitconfig." -l $tSrc:$LINENO

    touch .gitproj
    fComConfigCopy -f -s $HOME/.gitconfig -d .gitproj -i 'gitproj\.'

    fComSetConfig -L -k gitproj.config.proj-name -v $gpProjName
    fComSetConfig -L -k gitproj.config.ver -v "$(cat $gpDoc/VERSION)"

    git add .gitproj

    return 0
} # fCloneCheckLocalConfig

# --------------------
fCloneSetupGitProjConfig()
{
    local tSrc=${BASH_SOURCE##*/}
    local tVerbose=""

    fCloneCheckLocalConfig

    if [ $gpVerbose -ge 2 ]; then
        tVerbose='-v'
    fi

    fComMkPreCommit

    if [ ! -f .gitignore ]; then
        fLog -f -p warning -m "Missing .gitignore file. Adding it now." -l $tSrc:$LINENO
        cp $tVerbose --backup=t $gpDoc/config/gitignore .gitignore
        git add .gitignore
    fi

    fComConfigCopy -s .gitproj -d .git/config

    fComSetConfig -l -k "gitproj.config.local-host" -v $HOSTNAME
    fComSetConfig -l -k "gitproj.config.local-status" -v "defined"
    fComSetConfig -l -k "gitproj.config.proj-name" -v "$gpProjName"

    fComConfigUpdateLocal

    return 0
} # fCloneSetupGitProjConfig

# --------------------
fCloneMkGitDir()
{
    local tSrc=${BASH_SOURCE##*/}
    local tFound
    local tBranch

    # Assumes you are cd'd to the dir you want to clone into.
    # And it has been verified to not be a git workspace.

    git clone $gpRemoteGitDir
    if [ $? -ne 0 ]; then
        fLog -f -p info -m "git clone $gpRemoteGitDir" -l $tSrc:$LINENO
        fError -i -m "The above command should have worked." -l $tSrc:$LINENO
    fi
    gpProjName=${gpRemoteGitDir##*/}
    gpProjName=${gpProjName%.git}
    gpLocalTopDir=$PWD/$gpProjName
    cd $gpLocalTopDir >/dev/null 2>&1

    # Checkout some expected branches
    for tBranch in develop main master; do
        git checkout $tBranch >/dev/null 2>&1
    done

    # Set to the "best" default branch
    tFound=0
    git checkout develop >/dev/null 2>&1
    for tBranch in develop main master; do
        if git checkout $tBranch >/dev/null 2>&1; then
            tFound=1
            break
        fi
    done

    tBranch=$($cGetCurBranch)
    if [ $tFound -eq 0 ]; then
        fLog -f -p warning -m "Unexpected default branch: $tBranch" -l $tSrc:$LINENO
    fi

    fCloneSetupGitProjConfig

    return 0
} # fCloneMkGitDir

# --------------------
fCloneMkRawDir()
{
    local tSrc=${BASH_SOURCE##*/}
    local tOpt="-azC"
    local tVerbose=""
    local tStatus
    local tCmd

    gpLocalRawDir=$gpLocalTopDir/raw

    cd $gpLocalTopDir >/dev/null 2>&1
    if [ -d $gpLocalRawDir ]; then
        fLog -i -p warning -m "$gpLocalRawDir already exists." -l $tSrc:$LINENO
    else
        fLog -p info -m "mkdir $gpLocalRawDir" -l $tSrc:$LINENO
        mkdir $gpLocalRawDir
    fi

    if [ $gpVerbose -ge 2 ]; then
        tVerbose=-v
    fi
    if [ $gpVerbose -ge 3 ]; then
        tVerbose=-viP
    fi
    cd $gpLocalTopDir/.. >/dev/null 2>&1
    tCmd="/usr/bin/rsync $tOpt $tVerbose $gpRemoteRawOrigin/ $gpLocalRawDir"
    fLog -p notice -m "Run: $tCmd"
    $tCmd
    tStatus=$?
    if [ $tStatus -ne 0 ]; then
        fLog -f -p warning -m "There may be a problem with this rsync:" -l $tSrc:$LINENO
        fLog -f -p warning -m "$tCmd" -l $tSrc:$LINENO
    fi

    cd $gpLocalTopDir >/dev/null 2>&1
    fComSetConfig -l -k "gitproj.config.remote-raw-origin" -v "$gpRemoteRawOrigin"
    fComSetConfig -l -k "gitproj.config.remote-status" -v "defined"

    return $tStatus
} # fCloneMkRawDir

# --------------------
fCloneSummary()
{
    local tSrc=${BASH_SOURCE##*/}
    local tOrigin
    local tBranch

    function flExists()
    {
        local tFile=$1
        if [ -e $tFile ]; then
            echo "found"
            return 0
        fi
        echo "not-found"
        return 1
    } # flExists

    cd $gpLocalTopDir >/dev/null 2>&1
    tBranch=$($cGetCurBranch)
    tOrigin=$($cGetOrigin)

    cat <<EOF

Summary
    (auto mode)  $gpYesNo
    (verbose)  $gpVerbose
    (debug)  $gpDebug
    ($(flExists $HOME/.gitconfig))  HOME/.gitconfig
    ($(flExists $HOME/.gitignore))  HOME/.gitignore
    ($(flExists .gitproj))  $gpProjName/.gitproj
    ($(flExists .gitignore))  $gpProjName/.gitignore
    (branch)  $tBranch
    (remote-git-dir $(flExists $tOrigin))  $tOrigin
    (local-raw-dir $(flExists $gpLocalRawDir))  $gpLocalRawDir

Global Config ~/.gitconfig
[gitproj "config"]
    proj-status = $(fComGetConfig -k "gitproj.config.proj-status")
    syslog = $(fComGetConfig -k "gitproj.config.syslog")
    facility = $(fComGetConfig -k "gitproj.config.facility")

Local Proj Config $gpProjName/.gitproj
[gitproj "config"]
$(git config -f .gitproj --list | grep gitproj.config | sed 's/gitproj\.config\.//')

[gitproj "hook"]
$(git config -f .gitproj --list | grep gitproj.hook | sed 's/gitproj\.hook\.//')

Other vars in $gpProjName/.gitproj
$(git config -f .gitproj --list | grep -v gitproj)

Modified files:
$(git status --long)

EOF
    if ! fComYesNo "Continue (commit the changes)"; then
        fLog -f -p warning -m "Not continuing." -l $tSrc:$LINENO
        return 1
    fi

    fLog -p notice -m "Committing changes" -l $tSrc:$LINENO
    fComConfigUpdateLocal
    git commit -am "Commit all changes"

    return 0
} # fCloneSummary

# --------------------
fCloneFromRemoteDir()
{
    # Called by "git-proj-clone"
    # Minimum settings:
    # All the common vars set by gitproj-com.inc fComSetGlobals()
    # Required: $1 <- gpRemoteGitDir
    # gpYesNo, if gpAuto=1
    local pRemoteGitDir="$1"
    local tSrc=${BASH_SOURCE##*/}

    fCloneValidRemoteDir "$pRemoteGitDir"
    fCloneGettingStarted

    echo
    echo '================================================================='
    echo
    fCloneMkGitDir
    fCloneMkRawDir

    echo
    echo '================================================================='
    fCloneSummary
    fComIntroText

    return 0
} # fCloneFromRemoteDir

# ========================================
fComPreProjSetGlobals
