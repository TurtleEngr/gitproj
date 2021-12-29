#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpAutoMove gpBin gpBranch gpCheckFileNames \
    gpCheckForBigFiles gpCmdName gpDebug gpDoc gpFacility gpGitFlow \
    gpHookSource gpLocalRawDir gpLocalStatus gpLocalTopDir gpMaxSize \
    gpPath gpPreCommitEnabled gpProjName gpProjStatus gpRemoteGitDir \
    gpRemoteRawDir gpRemoteStatus gpSysLog gpVer gpVerbose

# ========================================

# --------------------
fCloneGettingStarted()
{
    local tSrc=${BASH_SOURCE##*/}

    cat <<EOF

Be sure you are "cd" to the directory that the project will be cloned to.
Clone git from: $gpRemoteGitDir
Clone raw from: $gpRemoteRawDir
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
    local tSrc=${BASH_SOURCE##*/}
    local tAvail
    local tGitSize
    local tRawSize
    local tDesiredSize

    if ! fComMustNotBeInGit $PWD; then
        return 1
    fi

    if [ ! -w $PWD ]; then
        fError -m "Current dir is not writable" -l $tSrc:$LINENO
    fi

    if [ ! -d $gpRemoteGitDir ]; then
        fError -m "$gpRemoteGitDir does not exist." -l $tSrc:$LINENO
    fi

    if [ ! -d $gpRemoteRawDir ]; then
        fError -m "$gpRemoteRawDir does not exist." -l $tSrc:$LINENO
    fi

    if ! fComAllMustBeReadable $gpRemoteGitDir; then
        exit 1
    fi
    if ! fComAllMustBeReadable $gpRemoteRawDir; then
        exit 1
    fi

    if [ -e $gpProjName ]; then
        fError -m "A $gpProjName already exists in this dir." -l $tSrc:$LINENO
    fi

    # Is there enough room in current dir for the clone? (s/b 2x size
    # of remote git and raw)

    tAvail=$(df -BM $PWD --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm $gpRemoteGitDir | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm $gpRemoteRawDir | awk '{print $1}')
    let tRawSize*=2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tAvail -lt $tDesiredSize ]; then
        fError -m "There is not enough space in current directory. Project '$gpProjName' needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi

    return 0
} # fCloneValidRemoteDir

# --------------------
fCloneCheckLocalConfig()
{
    local tSrc=${BASH_SOURCE##*/}
    
    if [ -f $cConfigLocal ]; then
        return 0
    fi
    fLog -p warning -m "Missing file: $cConfigLocal It should have been versioned. Will try to recreate it from a host config file." -l $tSrc:$LINENO

    # Look for the newest host file (by date-stamp) (but not $cConfigLocal)
    tHost=$('ls' -1t .gitproj.config.* 2>/dev/null | grep -v local | head -n 1)
    if [ -z "$tHost" ]; then
        fError -i -m "There are not host file to copy from." -l $tSrc:$LINENO
    fi

    fLog -p debug-2 -m "cp $tHost $cConfigLocal" -l $tSrc:$LINENO
    cp $tHost $cConfigLocal

    # Adjust the host var values
    fComSetConfig -L -k "gitproj.config.remote-raw-dir" -v $gpRemoteRawDir

    git add $cConfigLocal

    return 0
} # fCloneCheckLocalConfig

# --------------------
fCloneCheckHostConfig()
{
    local tSrc=${BASH_SOURCE##*/}
    local tHost

    if [ -f $cConfigHost ]; then
        return 0
    fi
    fLog -p warning -m "Missing host file: $cConfigHost  It will be created for this new host" -l $tSrc:$LINENO
    
    # Look for the newest host file (by date-stamp) (but not $cConfigLocal)
    tHost=$('ls' -1t .gitproj.config.* 2>/dev/null | grep -v local | head -n 1)
    if [ -n "$tHost" ]; then
        fLog -p debug-2 -m "cp $tHost $cConfigHost" -l $tSrc:$LINENO
        cp $tHost $cConfigHost
    else
        fLog -p debug-2 -m "cp $cConfigLocal $cConfigHost" -l $tSrc:$LINENO
        cp $cConfigLocal $cConfigHost
    fi
    
    # Adjust the host var values
    fComSetConfig -H -k "gitproj.config.remote-raw-dir" -v $gpRemoteRawDir

    git add $cConfigHost

    return 0
} # fCloneCheckHostConfig

# --------------------
fCloneCheckProjConfig()
{
    local tSrc=${BASH_SOURCE##*/}
    declare -a tHostList
    local tC
    
    tHostList=(.gitproj.config.*)
    if [ "${tHostList[0]}" != ".gitproj.config.*" ]; then
        return 0
    fi

    fLog -p warning -m "There are no host project config files for this project. They should have been versioned! Will try to create them, but they could have bad values." -l $tSrc:$LINENO

    # Get a raw local file from $gpDoc
    fLog -p debug-2 -m "cp $gpDoc/config/${cConfigLocal#.} $cConfigLocal"
    cp $gpDoc/config/${cConfigLocal#.} $cConfigLocal
    fLog -p debug-2 -m "cp $gpDoc/config/${cConfigLocal#.} $cConfigHost"
    cp $gpDoc/config/${cConfigLocal#.} $cConfigHost
    
    # Fix up the vars, making some assumptions.
    fComSetConfig -L -k "gitproj.config.remote-raw-dir" -v $gpRemoteRawDir
    fComSetConfig -H -k "gitproj.config.remote-raw-dir" -v $gpRemoteRawDir

    for tC in L H; do
        fComSetConfig -$tC -k "gitproj.config.local-status" -v "defined"
    	fComSetConfig -$tC -k "gitproj.config.remote-status" -v "defined"
    	fComSetConfig -$tC -k "gitproj.config.proj-name" -v "$gpProjName"
    	fComSetConfig -$tC -k "gitproj.config.git-flow-pkg" -v "git-flow"
    done

    git add $cConfigLocal
    git add $cConfigHost

    return 0
} # fCloneCheckProjConfig

# --------------------
fCloneMkGitDir()
{
    local tSrc=${BASH_SOURCE##*/}
    local tStatus
    local tFound

    # Assumes you are cd'd to the dir you want to clone into.

    git clone $gpRemoteGitDir
    if [ $? -ne 0 ]; then
        gpVerbose=2
        fLog -p info -m "git clone $gpRemoteGitDir" -l $tSrc:$LINENO
        fError -i -m "The above command should have worked." -l $tSrc:$LINENO
    fi
    gpProjName=${gpRemoteGitDir##*/}
    gpProjName=${gpProjName%.git}
    gpLocalTopDir=$PWD/$gpProjName

    # Checkout some expected branches
    cd $gpLocalTopDir >/dev/null 2>&1
    for gpBranch in develop main master; do
        git checkout $gpBranch >/dev/null 2>&1
    done

    # Set to the "best" default branch
    tFound=0
    git checkout develop >/dev/null 2>&1
    for gpBranch in develop main master; do
        if git checkout $gpBranch >/dev/null 2>&1; then
	    tFound=1
	    break
	fi
    done
    
    gpBranch=$($cGetCurBranch)
    if [ $tFound -eq 0 ]; then
	fLog -p warning -m "Unexpected default branch: $gpBranch" -l $tSrc:$LINENO
    fi    

    if [ ! -f .gitignore ]; then
        fLog -p warning -m "Missing .gitignore file" -l $tSrc:$LINENO
	# TBD Get default?
    fi

    fCloneCheckProjConfig
    fCloneCheckHostConfig
    fCloneCheckLocalConfig

    fLog -p info -m "Include: ../$cConfigHost in .git/config" -l $tSrc:$LINENO
    fComSetConfig -a -l -k "include.path" -v ../$cConfigHost
    cp $gpDoc/hooks/* $gpLocalTopDir/.git/hooks

    git commit -am "Updating configs for this host."

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

    if [ $gpVerbose -ge 1 ]; then
        tVerbose=-v
    fi
    if [ $gpVerbose -ge 2 ]; then
        tVerbose=-viP
    fi
    tCmd="/usr/bin/rsync $tOpt $tVerbose $gpRemoteRawDir/ $gpLocalRawDir"
    fLog -p notice -m "Run: $tCmd"
    $tCmd
    tStatus=$?
    if [ $tStatus -ne 0 ]; then
        fLog -p warning -m "There may be a problem with this rsync:" -l $tSrc:$LINENO
        fLog -p warning -m "$tCmd" -l $tSrc:$LINENO
    fi

    return $tStatus
} # fCloneMkRawDir

# --------------------
fCloneUpdateHostConfig()
{
    local tSrc=${BASH_SOURCE##*/}

    fComSetConfig -H -k "gitproj.config.remote-raw-dir" -v $gpRemoteRawDir

    tInclude=$(fComGetConfig -l -k "include.path" -v $cConfigHost)
    if [ -n "$tInclude" ]; then
        fLog -i -p warning -m "Weird, include.path is alrady set." -l $tSrc:$LINENO
    fi

    fLog -p info -m "Include: ../$cConfigHost in .git/config" -l $tSrc:$LINENO
    fComSetConfig -a -l -k "include.path" -v ../$cConfigHost

    git add $cConfigHost

    return 0
} # fCloneUpdateHostConfig

# --------------------
fCloneSummary()
{
    local tSrc=${BASH_SOURCE##*/}
    local tOrigin

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
    gpBranch=$($cGetCurBranch)
    tOrigin=$($cGetOrigin)

    cat <<EOF

Summary
    (auto mode)  $gpYesNo
    (verbose)  $gpVerbose
    (debug)  $gpDebug
    ($(flExists $HOME/.gitconfig))  HOME/.gitconfig
    ($(flExists $HOME/$cConfigGlobal))  HOME/$cConfigGlobal
    ($(flExists $HOME/.gitignore))  HOME/.gitignore
    ($(flExists $cConfigLocal))  $gpProjName/$cConfigLocal
    ($(flExists $cConfigHost))  $gpProjName/$cConfigHost
    ($(flExists .gitignore))  $gpProjName/.gitignore
    (branch)  $gpBranch
    (remote-git-dir $(flExists $tOrigin))  $tOrigin
    (local-raw-dir $(flExists $gpLocalRawDir))  $gpLocalRawDir

Global Config ~/$cConfigGlobal
[gitproj "config"]
    proj-status = $(fComGetConfig -k "gitproj.config.proj-status")
    syslog = $(fComGetConfig -k "gitproj.config.syslog")
    facility = $(fComGetConfig -k "gitproj.config.facility")
    bin = $(fComGetConfig -k "gitproj.config.bin")
    doc = $(fComGetConfig -k "gitproj.config.doc")
    test = $(fComGetConfig -k "gitproj.config.test")

Local Config $gpProjName/$cConfigHost
[gitproj "config"]
    local-status = $(fComGetConfig -k "gitproj.config.local-status")
    remote-status = $(fComGetConfig -k "gitproj.config.remote-status")
    ver = $(fComGetConfig -k "gitproj.config.ver")
    proj-name = $(fComGetConfig -k "gitproj.config.proj-name")
    git-flow-pkg = $(fComGetConfig -k "gitproj.config.git-flow-pkg")
    remote-raw-dir = $(fComGetConfig -k "gitproj.config.remote-raw-dir")
    remote-min-size = $(fComGetConfig -k "gitproj.config.remote-min-size")
[gitproj "hook"]
    source = $(fComGetConfig -k "gitproj.hook.source")
    pre-commit-enabled = $(fComGetConfig -k "gitproj.hook.pre-commit-enabled")
    check-file-names = $(fComGetConfig -k "gitproj.hook.check-file-names")
    check-for-big-files = $(fComGetConfig -k "gitproj.hook.check-for-big-files")
    binary-file-size-limit = $(fComGetConfig -k "gitproj.hook.binary-file-size-limit")
    auto-move = $(fComGetConfig -k "gitproj.hook.auto-move")

Modified files:
$(git status)

EOF
    if ! fComYesNo "Continue (commit the changes)"; then
        fLog -p warning -m "Not continuing." -l $tSrc:$LINENO
        return 1
    fi
    fLog -p notice -m "Committing changes" -l $tSrc:$LINENO
    git commit -am "Updated $cConfigHost"
    return 0
} # fCloneSummary

# --------------------
fCloneFromRemoteDir()
{
    # Called by "git proj clone"
    local tSrc=${BASH_SOURCE##*/}

    fCloneGettingStarted
    fCloneValidRemoteDir
    fCloneMkGitDir
    fCloneMkRawDir
    fCloneUpdateHostConfig
    fCloneSummary

    cat <<EOF | fComFmt

[Reuse text at gitproj-init.inc fInitCreateLocalGit]
EOF
    return 0
} # fCloneFromRemoteDir

# ========================================
export tSrc=${BASH_SOURCE##*/}
fComPreProjSetGlobals
