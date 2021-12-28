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
    local tMaxLoop=30
    local tResult

    cat <<EOF

Be sure you are "cd" to the directory that the project will be cloned to.
Clone git from: $gpRemoteGitDir
Clone raw from: $gpRemoteRawDir
Project Name:   $gpProjName
Project Dir:    $PWD/$gpProjName

EOF
    # TBD: convert to using fComSelect or com yes/no. Remove dup. msgs.
    if [ "$gpYesNo" = "Yes" ]; then
        fLog -p debug-2 -m "Cloning: $gpProjName" -l $tSrc:$LINENO
        return 0
    fi
    if [ "$gpYesNo" = "No" ]; then
        fLog -p warning -m "Not continuing." -l $tSrc:$LINENO
        exit 1
    fi
    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop
        echo -n "Continue [y/n]? "
        read -r tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ "$tResult" = "y" ]; then
    	    fLog -p debug-2 -m "Cloning: $gpProjName" -l $tSrc:$LINENO
            return 0
        fi
        if [ "$tResult" = "n" ]; then
            fLog -p warning -m "Not continuing." -l $tSrc:$LINENO
            exit 1
        fi
        echo "$tResult is not valid."
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

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

    gpLocalRawDir=$gpLocalTopDir/raw

    cd $gpLocalTopDir >/dev/null 2>&1
    fLog -p info -m "mkdir $gpLocalRawDir" -l $tSrc:$LINENO
    mkdir $gpLocalRawDir

    if [ $gpVerbose -ge 1 ]; then
        tVervose=-v
    fi
    if [ $gpVerbose -ge 2 ]; then
        tVervose=-viP
    fi
    fLog -p notice -m "'rsync' $tOpt $tVerbose $gpRemoteRawDir/ $gpLocalTopDir/raw"
    'rsync' $tOpt $tVerbose $gpRemoteRawDir/ $gpLocalRawDir
    if [ $? -ne 0 ]; then
        fLog -p warning -m "Was there a problem with rsync?" -l $tSrc:$LINENO
    fi

    return 0
} # fCloneMkRawDir

# --------------------
NAfCloneSaveVars()
{
    local tSrc=${BASH_SOURCE##*/}
    local tFile
    local tS

    gErr=0

    tFile=$HOME/$cConfigGlobal
    tS=gitproj.config
    fComSaveVar2Config $tFile $tS.proj-status gpProjStatus
    fComSaveVar2Config $tFile $tS.bin gpBin
    fComSaveVar2Config $tFile $tS.doc gpDoc
    fComSaveVar2Config $tFile $tS.test gpTest
    fComSaveVar2Config $tFile $tS.facility gpFacility
    fComSaveVar2Config $tFile $tS.syslog gpSysLog

    for tFile in \
        $gpLocalTopDir/$cConfigLocal \
        $gpLocalTopDir/$cConfigHost; do
        tS=gitproj.config
        fComSaveVar2Config $tFile $tS.ver gpVer
        fComSaveVar2Config $tFile $tS.local-status gpLocalStatus
        fComSaveVar2Config $tFile $tS.remote-status gpRemoteStatus
        fComSaveVar2Config $tFile $tS.proj-name gpProjName
    done

    for tFile in $HOME/$cConfigGlobal \
        $gpLocalTopDir/$cConfigLocal \
        $gpLocalTopDir/$cConfigHost; do
        tS=gitproj.config
        fComSaveVar2Config $tFile $tS.git-flow-pkg gpGitFlow
        tS=gitproj.hook
        fComSaveVar2Config $tFile $tS.auto-move gpAutoMove
        fComSaveVar2Config $tFile $tS.binary-file-size-limit gpMaxSize
        fComSaveVar2Config $tFile $tS.check-file-names gpCheckFileNames
        fComSaveVar2Config $tFile $tS.check-for-big-files gpCheckForBigFiles
        fComSaveVar2Config $tFile $tS.pre-commit-enabled gpPreCommitEnabled
        fComSaveVar2Config $tFile $tS.source gpHookSource
    done

    if [ $gErr -eq 0 ]; then
        fLog -p info -m "Save local configs" -l $tSrc:$LINENO
        git add .gitproj.config.*
        git commit -am "Save gitproj.config"
    fi

    return $gErr
} # fCloneSaveVars

# --------------------
fCloneUpdateHostConfig()
{
    local tSrc=${BASH_SOURCE##*/}

    fComSetConfig -H -k "gitproj.config.remote-raw-dir" -v $gpRemoteGitDir

    tInclude=$(fComGetConfig -l -k "include.path" -v $cConfigHost)
    if [ -n "$tInclude" ]; then
        fLog -i -p warning -m "Weird, $cConfigHost is already included" -l $tSrc:$LINENO
        return 0
    fi

    fLog -p info -m "Include: ../$cConfigHost in .git/config" -l $tSrc:$LINENO
    fComSetConfig -a -l -k "include.path" -v ../$cConfigHost

#? fCloneSaveVars

    return 0
} # fCloneUpdateHostConfig

# --------------------
fCloneSummary()
{
# TBD
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=30
    declare -l tResult

    cat <<EOF

Summary
      (auto mode)  $gpYesNo
        (verbose)  $gpVerbose
          (debug)  $gpDebug
         (exists)  HOME/.gitconfig
         (exists)  HOME/$cConfigGlobal
         (exists)  HOME/.gitignore
         (exists)  $gpProjName/$cConfigLocal
         (exists)  $gpProjName/$cConfigHost
         (exists)  $gpProjName/.gitignore	#?
         (branch)  $gpBranch
 (remote-git-dir)  $gpRemoteGitDir
  (local-raw-dir)  $gpLocalRawDir

[gitproj "config"]
     proj-status = $gpProjStatus
    local-status = $gpLocalStatus
   remote-status = $gpRemoteStatus
             ver = $gpVer
          syslog = $gpSysLog
        facility = $gpFacility
             bin = $gpBin
             doc = $gpDoc
       proj-name = $gpProjName
    git-flow-pkg = $gpGitFlowPkg
(remoge-raw-dir) = $gpRemoteRawDir

[gitproj "hook"]
                source = $gpHookSource
    pre-commit-enabled = $gpPreCommitEnabled=true
      check-file-names = $gpCheckFileNames=true
   check-for-big-files = $gpCheckForBigFiles=true
binary-file-size-limit = $gpMaxSize
             auto-move = $gpAutoMove

Modified files:
$(git status)

EOF
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
    fCloneMkRaw
    fCloneUpdateHostConfig
 ##fCloneSaveVars
    fCloneSummary
    git commit -am "Added $cConfigHost"

    cat <<EOF | fComFmt

[Reuse text at gitproj-init.inc fInitCreateLocalGit]
EOF
    return 0
} # fCloneFromRemoteDir

# --------------------
NAfCloneGetGitFlow()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=30
    declare -l tResult

    fComCheckPkg $gpGitFlowPkg
    if [ $? -ne 0 ]; then
        fLog -p warning -m "git-flow is not installed." -l $tSrc:$LINENO
        gpGitFlow="false"
        return 0
    fi
    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop

        echo -n "Setup git-flow (-f) [y/n/quit]? "
        read -r tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ "$tResult" = "y" ]; then
            gpGitFlow="true"
            break
        fi
        if [ "$tResult" = "n" ]; then
            gpGitFlow="false"
            break
        fi
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    return 0
} # fCloneGetGitFlow

# --------------------
NAfCloneMkGitFlow()
{
# TBD: is this needed?
    local tSrc=${BASH_SOURCE##*/}

    if [ "$gpGitFlow" = "false" ]; then
        return 0
    fi
    if ! fComCheckPkg git-flow; then
        fError -n -m "git-flow is not installed" -l $tSrc:$LINENO
        return 1
    fi
    fLog -p info -m "git flow init" -l $tSrc:$LINENO
    git flow init -d

    # Define the default prefixes

    fComSetConfig -g -k gitflow.branch.main -v main
    fComSetConfig -g -k gitflow.branch.master -v main
    fComSetConfig -g -k gitflow.branch.develop -v develop

    fComSetConfig -g -k gitflow.prefix.feature -v feature/
    fComSetConfig -g -k gitflow.prefix.bugfix -v bug/
    fComSetConfig -g -k gitflow.prefix.release -v release/
    fComSetConfig -g -k gitflow.prefix.hotfix -v hotfix/
    fComSetConfig -g -k gitflow.prefix.support -v support/

    return 0
} # fCloneMkGitFlow

# ========================================
export tSrc=${BASH_SOURCE##*/}
fComPreProjSetGlobals
