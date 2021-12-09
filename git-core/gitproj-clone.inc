#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpAutoMove gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCmdName gpDebug gpDoc gpFacility gpGitFlow \
    gpHookSource gpLocalRawDir \
    gpLocalStatus gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled \
    gpProjName gpProjStatus gpRemoteGitDir gpRemoteRawDir \
    gpRemoteStatus gpSysLog gpVer gpVerbose gpYesNo

# ========================================

# --------------------
fCloneGettingStarted()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=30
    local tResult

    cat <<EOF

Be sure you are "cd" to the directory that the project will be cloned to.

EOF
    if [ "$gpYesNo" = "Yes" ]; then
        fLog -p debug-2 -m "fCloneGettingStarted in $PWD" -l $tSrc:$LINENO
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
            return 0
        fi
        if [ "$tResult" = "n" ]; then
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

    if [ -w $PWD ]; then
        fError -m "Current dir is not writable" -l $tSrc:$LINENO
    fi
    
    if [ -d $gpRemoteGitDir ]; then
        fError -m "$gpRemoteGitDir does not exist." -l $tSrc:$LINENO
    fi

    if [ -d $gpRemoteRawDir ]; then
        fError -m "$gpRemoteRawDir does not exist." -l $tSrc:$LINENO
    fi

    if ! fComAllMustBeReadable $gpRemoteGitDir; then
        exit 1
    fi
    if ! fComAllMustBeReadable $gpRemoteRawDir; then
        exit 1
    fi

    if [ ! -e $gpProjName ]; then
        fError -m "A $gpProjName already exists in this dir." -l $tSrc:$LINENO
    fi

# TBD is there enough room in current dir for the clone? (s/b 2x size
# of remote)

    tAvail=$(df -BM $PWD --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm $gpRemoteGitDir | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm $gpRemoteRawDir | awk '{print $1}')
    let tRawSize*=2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tAvail -lt $tDesiredSize ]; then
        fError -m "There is not enough space in current directory. $gpProjName needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi

    return 0
} # fCloneValidRemoteDir

# --------------------
fCloneMkGitDir()
{
#TBD
    local tSrc=${BASH_SOURCE##*/}
    local tStatus

    gpLocalTopDir=$PWD/$gpProjName

    cd $gpLocalTopDir >/dev/null 2>&1

    fLog -p info -m "Create: .gitignore" -l $tSrc:$LINENO
    cp $gpDoc/config/gitignore.default .gitignore

    fLog -p info -m "git init" -l $tSrc:$LINENO
    git init

    fLog -p info -m "git add FILES" -l $tSrc:$LINENO
    git add .gitignore *

    fLog -p info -m "git commit" -l $tSrc:$LINENO
    git commit -am "Initial commit"
    tStatus=$?
    if [ $tStatus -ne 0 ]; then
        fError -i "Initial git commit failed." -l $tSrc:$LINENO
    fi

    fLog -p info -m "Rename master to main" -l $tSrc:$LINENO
    git branch --move master main
    fLog -p info -m "Create develop branch" -l $tSrc:$LINENO
    git branch develop
    git checkout develop

    fCloneMkGitFlow
    git branch
    git status

    cp $gpDoc/hooks/* $gpLocalTopDir/.git/hooks

    return 0
} # fCloneMkGitDir

# --------------------
fCloneMkRawDir()
{
#TBD
    local tSrc=${BASH_SOURCE##*/}

    gpLocalRawDir=$gpLocalTopDir/raw

    cd $gpLocalTopDir >/dev/null 2>&1
    fLog -p info -m "mkdir $gpLocalRawDir" -l $tSym:$LINENO
    mkdir $gpLocalRawDir

    fLog -p info -m "Create: raw/README.txt" -l $tSym:$LINENO
    cat <<EOF | fmt >raw/README.txt
    
Do NOT remove these files or this directory.

These files are managed by the "git proj" subcommand. It's purpose is
to keep large binary files out of git repositories. Large files are
put in this directory and symlinks may be put in the git managed
directories to point to the files.

The initial working directory was created at $HOSTNAME:$gpLocalTopDir

For help with the "git proj" subcommand, just type "git proj -h".
EOF
    cd - >/dev/null 2>&1
    return 0
} # fCloneMkRawDir

# --------------------
fCloneMkHostConfig()
{
#TBD
    local tSrc=${BASH_SOURCE##*/}

    cd $gpLocalTopDir >/dev/null 2>&1
    fLog -p info -m "Create: $cConfigLocal" -l $tSrc:$LINENO
    cp $gpDoc/config/gitproj.config.local $cConfigLocal
    fLog -p info -m "Create: $cConfigHost" -l $tSrc:$LINENO
    cp $gpDoc/config/gitproj.config.HOSTNAME $cConfigHost

    tInclude=$(fComGetConfig -l -k "include.path" -v $cConfigHost)
    if [ -n "$tInclude" ]; then
        fLog -i -p warning -m "Weird, $cConfigHost is already included" -l $tSrc:$LINENO
        return 0
    fi

    fLog -p info -m "Include: ../$cConfigHost in .git/config" -l $tSrc:$LINENO
    fComSetConfig -a -l -k "include.path" -v ../$cConfigHost

    return 0
} # fCloneMkHostConfig

# --------------------
fCloneSaveVars()
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
fCloneSummary()
{
# TBD
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=30
    declare -l tResult

    cat <<EOF

Summary
           (auto) = $gpAuto
        (verbose) = $gpVerbose
          (debug) = $gpDebug
          (exits)   HOME/.gitconfig
        (created)   HOME/$cConfigGlobal
  (local-raw-dir) = $gpLocalRawDir
  (git-flow init) = $gpGitFlow

[gitproj "config"]
      proj-status = $gpProjStatus
     local-status = $gpLocalStatus
              ver = $gpVer
           syslog = $gpSysLog
         facility = $gpFacility
              bin = $gpBin
              doc = $gpDoc
             test = $gpTest
        proj-name = $gpProjName
     git-flow-pkg = $gpGitFlowPkg

[gitproj "hook"]
                source = $gpHookSource
    pre-commit-enabled = $gpPreCommitEnabled=true
      check-file-names = $gpCheckFileNames=true
   check-for-big-files = $gpCheckForBigFiles=true
binary-file-size-limit = $gpMaxSize
             auto-move = $gpAutoMove

EOF
    if [ $gpAuto -eq 0 ]; then
        while [ $tMaxLoop -gt 0 ]; do
            let --tMaxLoop

            echo -n "Continue with creating a local git repo [y/n]? "
            read -r tResult
            tResult=${tResult%${tResult#[a-z]}}
            if [ "$tResult" = "y" ]; then
                break
            fi
            if [ "$tResult" = "n" ]; then
                fLog -p warning -m "Quitting, nothing will be done." -l $tSrc:$LINENO
                exit 1
            fi
            fLog -p warning -m "Invalid answer: $tResult" -l $tSrc:$LINENO
        done
        if [ $tMaxLoop -eq 0 ]; then
            fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
        fi
    fi
    return 0
} # fCloneSummary

# --------------------
fCloneFromRemoteDir()
{
    # Called by "git proj clone"
    local tSrc=${BASH_SOURCE##*/}

    fCloneValidRemoteDir
    fCloneGettingStarted
    fCloneMkGitDir
    fCloneMkRaw
    fCloneMkHostConfig
    fCloneSaveVars
    fCloneSummary

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
