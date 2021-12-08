#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpAutoMove gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCmdName gpDebug gpDoc gpFacility gpGitFlow \
    gpHookSource gpLocalRawDir \
    gpLocalStatus gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled \
    gpProjName gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose

# ========================================

# --------------------
fCloneSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTrim

    fComFirstTimeSet
    
    gpVer=$cGitProjVersion

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpAuto=0
    gpLocalTopDir=$PWD
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "TBD")
    if [ "$gpProjName" = "TBD" ]; then
        gpProjName="${gpLocalTopDir##*/}"
    fi

    gpLocalRawDir=$gpLocalTopDir/raw

    gpMaxSize=$(fComGetConfig -k "gitproj.hook.binary-file-size-limit" -d "10k")
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #20g

    gpAutoMove="true"
    gpGitFlow="false"
    gpGitFlowPkg=$(fComGetConfig -k "gitproj.config.git-flow-pkg" -d "git-flow")

    gpProjStatus=$(fComGetConfig -k "gitproj.config.proj-status" -d "installed")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")

    gpCheckFileNames=true
    gpCheckForBigFiles=true
    gpPreCommitEnabled=true
    gpHookSource=hooks/pre-commit
    gpMountDir=TBD
    gpRemoteRawDir=TBD

    return 0
} # fCloneSetGlobals

# --------------------
fCloneGettingStarted()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=30

    cat <<EOF

Be sure you are "cd" to the "top" project directory. Or use the "-l
pLocalPath" option. The directory name will be used for the git repo
name. The project directory should not already be a git repo, and it
cannot contain any git repos under it.  Also, all of the directories
and files need to be readable by your user.

EOF
    if [ $gpAuto -ne 0 ]; then
        fLog -p debug-2 -m "fCloneGettingStarted in $PWD" -l $tSrc:$LINENO
        return 0
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
    local pLocalPath=$1
    local tSrc=${BASH_SOURCE##*/}

    if ! fComMustNotBeInGit $pLocalPath; then
        return 1
    fi
    if ! fComAllMustBeReadable $pLocalPath; then
        return 1
    fi

    gpLocalTopDir=$pLocalPath
    gpProjName="${gpLocalTopDir##*/}"
    echo "The project Name will be: $gpProjName"
    return 0
} # fCloneValidRemoteDir

# --------------------
fCloneGetSize()
{
# TBD is there enough room in current dir for the clone? (s/b 2x size
# of remote)

    local pMaxSize=$1
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=30
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fCloneValidSize $pMaxSize; then
            fError -m "Invalid -s pSize: $pMaxSize" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop

        cat <<EOF | fComFmt
Define the size for large binary files. Format: Nb, Nk, Nm, or Ng
EOF
        echo "Size (-s) [$pMaxSize/quit]? "
        read -r tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$pMaxSize
        fi
        if fCloneValidSize $tResult; then
            break
        fi
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    return 0
} # fCloneGetSize

# --------------------
fCloneGetGitFlow()
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
fCloneSummary()
{
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
fCloneMkRaw()
{
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
} # fCloneMkRaw

# --------------------
fCloneMkGitFlow()
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

# --------------------
fCloneMkGitDir()
{
    local tSrc=${BASH_SOURCE##*/}
    local tStatus

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
fCloneMkLocalConfig()
{
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
} # fCloneMkLocalConfig

# --------------------
fCloneSaveVarsToConfigs()
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
} # fCloneSaveVarsToConfigs

# --------------------
fCloneFromRemoteDir()
{
    # Called by "git proj clone"
    local tSrc=${BASH_SOURCE##*/}

    fCloneGettingStarted

    # Prompt/Response or if -a, validate the options

# should NOT be in a git dir
fCheckForGit

# Does the remove dir exist and is there a ProjName.git/ and ProjName.raw/?
fCheckPath $gpRemoteDir

# Clone git, then clone raw, create .gitproj.config.local ...
fGetLocal $gpPath

    cat <<EOF | fComFmt

[Reuse text at gitproj-init.inc fInitCreateLocalGit]
EOF
    return 0
} # fCloneFromRemoteDir

# ========================================
export tSrc=${BASH_SOURCE##*/}
fCloneSetGlobals
