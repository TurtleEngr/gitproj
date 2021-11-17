#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpAutoMove gpBin gpCheckFileNames gpCheckForBigFiles \
       gpCmdName gpDebug gpDoc gpFacility gpGitFlow gpHardLink \
       gpHookSource gpLocalRawDir gpLocalRawDirPat gpLocalRawSymLink \
       gpLocalStatus gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled \
       gpProjName gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose

# ========================================

# --------------------
fInitFirstTimeSet()
{
    local tSrc=${BASH_SOURCE##*/}
    local tInclude
    local tUserName
    local tUserEmail

    if [ ! -f $HOME/.gitconfig ]; then
	fLog -p info -m "Create: $HOME/.gitconig" -l $tSrc:$LINENO
        cp $gpDoc/config/gitconfig.default $HOME/.gitconfig
    fi

    tUserName=$(fComGetConfig -g -k "user.name")
    tUserEmail=$(fComGetConfig -g -k "user.email")
    if [ -z "$tUserName" ] || [ -z "$tUserEmail" ]; then
        fError -m "user.name and user.email must be in your ~/.gitconfig file. See $cgDoc/config/gitconfig.default for some other useful settings." -l $tSrc:$LINENO
    fi

    if [ ! -f $cConfigGlobal ]; then
	fLog -p info -m "Create: $cConfigGlobal" -l $tSrc:$LINENO
        cp $gpDoc/config/gitproj.config.global $cConfigGlobal
    fi
    tInclude=$(fComGetConfig -g -k "include.path" -v .gitproj.config.global)
    if [ -z "$tInclude" ]; then
	fLog -p info -m "include.path=.gitproj.config.global" -l $tSrc:$LINENO
        fComSetConfig -a -g -k "include.path" -v .gitproj.config.global
    fi
} # fInitFirstTimeSet

# --------------------
fInitSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTrim

    fInitFirstTimeSet

    gpVer=$(fComGetConfig -k "gitproj.config.ver")
    # Only the first two # are important
    tTrim=${gpVer#*.*.}
    gpVer=${gpVer%.$tTrim}

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpAuto=0

    # Relative to top-dir
    gpLocalRawDirPat=$(fComGetConfig -k "gitproj.config.local-raw-dir-pat" -d "..")

    gpLocalRawSymLink=$(fComGetConfig -k "gitproj.config.local-raw-symlink" -d raw)

    gpLocalTopDir=$(fComGetConfig -k "gitproj.config.top-dir" -d "$PWD")
    if [ "$gpLocalTopDir" = "TBD" ]; then
        gpLocalTopDir=$PWD
    fi
    if [ ! -d $gpLocalTopDir ]; then
        gpLocalTopDir="TBD"
    fi
    
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "TBD")
    if [ "$gpProjName" = "TBD" ]; then
        gpProjName="${gpLocalTopDir##*/}"
    fi

    # Path is relative to top-dir
    gpLocalRawDir=${gpLocalRawDirPat}/${gpProjName}.raw

    gpMaxSize=$(fComGetConfig -k "gitproj.hook.binary-file-size-limit" -d "10k")
    gpRemoteMinSpace=$(fComGetConfig -k "gitproj.config.remote-min-space" -d "20g")
    gpAutoMove="true"
    gpGitFlow="false"
    gpGitFlowPkg=$(fComGetConfig -k "gitproj.config.git-flow-pkg" -d "git-flow")

    gpProjStatus=$(fComGetConfig -k "gitproj.config.proj-status" -d "installed")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")

    # Uknown at this point, copy files is the safest option (i.e. false)
    gpHardLink=$(fComGetConfig -k "gitproj.config.hardlink" -d "false")

    gpCheckFileNames=true
    gpCheckForBigFiles=true
    gpPreCommitEnabled=true
    gpHookSource=hooks/pre-commit
    gpMountDir=TBD
    gpRemoteRawDir=TBD

    return 0
} # fInitSetGlobals

# --------------------
fInitGettingStarted() {
    local tSrc=${BASH_SOURCE##*/}

    cat <<EOF

Be sure you are "cd" to the "top" project directory. Or use the "-l
pLocalPath" option. The directory name will be used for the git repo
name. The project directory should not already be a git repo, and it
cannot contain any git repos under it.  Also, all of the directories
and files need to be readable by your user.

EOF
    if [ $gpAuto -ne 0 ]; then
        fLog -p debug-2 -m "fInitGettingStarted in $PWD" -l $tSrc:$LINENO
        return 0
    fi
    while true; do
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
    return 0
} #fInitGettingStarted

# --------------------
fInitValidLocalPath()
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
} # fInitValidLocalPath

# --------------------
fInitGetLocalPath()
{
    local pLocalPath=$1

    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fInitValidLocalPath $pLocalPath; then
            fError -m "Invalid -l pLocalPath: $pLocalPath" -l $tSrc:$LINENO
        fi
        return 0
    fi
    while true; do
        cat <<EOF | fmt
Define the existing project directory. The last directory will be used
for the name of the project. Default: current directory
EOF
        echo -n "Project Path (-l) [$pLocalPath/quit]? "
        read -r tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$pLocalPath
        fi
        if fInitValidLocalPath $tResult; then
            break
        fi
    done
    return 0
} # fInitGetLocalPath

# --------------------
fInitValidLocalRawDirPat()
{
    local pLocalRawDirPat=$1

    local tSrc=${BASH_SOURCE##*/}

    if [ "${pLocalRawDirPat#..}" = "$pLocalRawDirPat" ]; then
        fLog -p warning -m "RawLocalPathPat works best if it is relative to top-dir: $gpLocalTopDir" -l $tSrc:$LINENO
    fi

    cd $gpLocalTopDir >/dev/null 2>&1
    if [ ! -d $pLocalRawDirPat ]; then
        fLog -p err -m "Raw directory $pLocalRawDirPat not found relative to $gpLocalTopDir" -l $tSrc:$LINENO
        cd - >/dev/null 2>&1
        return 1
    fi
    cd - >/dev/null 2>&1

    cd $gpLocalTopDir/$pLocalRawDirPat >/dev/null 2>&1
    echo $PWD | grep -q "/$gpProjName/"
    # shellcheck disable=SC2181
    if [ $? -eq 0 ]; then
        fLog -p err -m "Raw directory cannot be in $gpLocalTopDir" -l $tSrc:$LINENO
        cd - >/dev/null 2>&1
        return 1
    fi
    cd - >/dev/null 2>&1

    # TBD: error for gpLocalRawDirPat to be "in" a raw dir.

    cd $gpLocalTopDir >/dev/null 2>&1
    gpLocalRawDir=${gpLocalRawDirPat}/${gpProjName}.raw
    if [ -d $gpLocalRawDir ]; then
        fLog -p warn -m "Raw directory already exists: $gpLocalRawDir" -l $tSrc:$LINENO
    fi
    cd - >/dev/null 2>&1
    return 0
} # fInitValidLocalRawDirPat

# --------------------
fInitSetHardLink()
{
    local tSrc=${BASH_SOURCE##*/}
    local tDevTopDir
    local tDevRawDir

    # If on same filesystem, hardlinks can be used to move files quickly
    cd $gpLocalTopDir >/dev/null 2>&1
    tDevTopDir=$(df -BM $gpLocalTopDir --output=source | tail -n 1)
    tDevRawDir=$(df -BM $gpLocalRawDirPat --output=source | tail -n 1)
    gpHardLink=false
    if [ "$tDevTopDir" = "$tDevRawDir" ]; then
        gpHardLink=true
    fi
    return 0
} # fInitSetHardLink

# --------------------
fInitGetLocalRawDirPat()
{
    local pLocalRawDirPat=$1
    
    local tSrc=${BASH_SOURCE##*/}
    local tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fInitValidLocalRawDirPat $pLocalRawDirPat; then
            fError -m "Invalid -l pLocalRawDirPat: $pLocalRawDirPat" -l $tSrc:$LINENO
        fi
        fInitSetHardLink
        return 0
    fi

    while true; do
        cat <<EOF | fmt
        
Set the location for large binary files. It cannot be *in* any of the
project directories. It will be created, if it does not exist. The
path should be relative to the project dir. The files in the raw
directory will NOT be versioned--only the latest copy will be
saved. Default: ..

EOF
        echo -n "Raw dir (-r) [$gpLocalRawDirPat/quit]? "
        read -r tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$gpLocalRawDirPat
        fi
        if fInitValidLocalRawDirPat $tResult; then
            break
        fi
    done

    fInitSetHardLink
    return 0
} # fInitGetLocalRawDirPat

# --------------------
fInitValidSymLink()
{
    export pSymLinkName=$1

    local tSrc=${BASH_SOURCE##*/}

    cd $gpLocalTopDir >/dev/null 2>&1
    if [ -e $pSymLinkName ]; then
        fLog -p err -m "Symlink or file: $pSymLinkName, already exists. Pick another name or quit and remove it." -l $tSrc:$LINENO
        cd - >/dev/null 2>&1
        return 1
    fi
    cd - >/dev/null 2>&1

    gpLocalRawSymLink=$pSymLinkName
    return 0
} # fInitValidSymLink

# --------------------
fInitGetSymLink()
{
    local pSymLinkName=$1
    
    local tSrc=${BASH_SOURCE##*/}
    local tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fInitValidSymLink $pSymLinkName; then
            fError -m "Invalid -k pSymLinkName: $pSymLinkName" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while true; do
        cat <<EOF | fmt
        
Define the symlink name that will point to the Raw dir. It will be
created in the project's top dir. If the Raw dir is moved, this link
will need to be updated. Symlink in the git directories will point to
this dir and the symlinks will be versioned. Default: raw

EOF
        echo -n "Symlink (-k) [$pSymLinkName/quit]? "
        read -r tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$pSymLinkName
        fi
        if fInitValidSymLink $tResult; then
            break
        fi
    done
    return 0
} # fInitGetSymLink

# --------------------
fInitValidSize()
{
    declare -l pMaxSize=$1

    local tSrc=${BASH_SOURCE##*/}

    echo $pMaxSize | grep -Eq '[0-9]+[bkmg]'
    # shellcheck disable=SC2181
    if [ $? -eq 0 ]; then
        gpMaxSize=$pMaxSize
        return 0
    fi
    fLog -p err -m "Size must be numbers followed by b, k, m, or g" -l $tSrc:$LINENO
    return 1
} # fInitValidSize

# --------------------
fInitGetSize()
{
    local pMaxSize=$1

    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fInitValidSize $pMaxSize; then
            fError -m "Invalid -s pSize: $pMaxSize" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while true; do
        cat <<EOF | fmt
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
        if fInitValidSize $tResult; then
            break
        fi
    done
} # fInitGetSize

# --------------------
fInitGetBinaryFiles()
{
    local tSrc=${BASH_SOURCE##*/}
    local tFile
    local tFileList=""
    local tSize

    # shellcheck disable=SC2020
    tSize=$(echo "$gpMaxSize" | tr 'bBKmg' 'cckMG')

    cd $gpLocalTopDir >/dev/null 2>&1
    # shellcheck disable=SC2044
    for tFile in $(find * -type f -size +$tSize); do
        # If char are found outside of the space to ~ range, it is
        # probably a binary file.
        if [ "$(tr -d '[ -~\n\r\b\f\t\v]' <$tFile | wc -c)" != "0" ]; then
            tFileList="$tFileList $tFile"
        fi
    done
    if [ -n "$tFileList" ]; then
        echo $tFileList | fmt
        return 0
    fi
    return 1
} # fInitGetBinaryFiles

# --------------------
fInitGetMoveFiles()
{
    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult

    fLog -p info -m "These binary files are greater than $gpMaxSize:" -l $tSrc:$LINENO

    if ! fInitGetBinaryFiles; then
        fLog -p warning -m 'No large binary files were found.' -l $tSrc:$LINENO
        return 0
    fi

    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    while true; do
        cat <<EOF | fmt

The listed files can be moved to the project's raw directory. Paths
will be created in the raw directory that correspond to the project's
directory. A symlink will replace the moved file. The symlink will
point to [raw]. Quit if you want to change the MaxSize.

EOF
        echo -n "Move the files (-m) [y/n/quit]? "
        read -r tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ "$tResult" = "y" ]; then
            gpAutoMove=1
            break
        fi
        if [ "$tResult" = "n" ]; then
            gpAutoMove=0
            break
        fi
    done
    return 0
} # fInitGetMoveFiles

# --------------------
fInitGetGitFlow()
{
    local tSrc=${BASH_SOURCE##*/}
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

    while true; do
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
    return 0
} # fInitGetGitFlow

# --------------------
fInitSummary()
{
    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult

    cat <<EOF

Summary
           (auto) = $gpAuto
        (verbose) = $gpVerbose
          (debug) = $gpDebug
          (exits)   HOME/.gitconfig
        (created)   HOME/.gitproj.config.global
 (local-raw-dir)  = $gpLocalRawDirPat/$gpProjName.raw
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
    local-top-dir = $gpLocalTopDir
        proj-name = $gpProjName
local-raw-dir-pat = $gpLocalRawDirPat
local-raw-symlink = $gpLocalRawSymLink
         hardlink = $gpHardLink
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
        while true; do
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
    fi
    return 0
} # fInitSummary

# --------------------
fInitMkRaw()
{
    local tSrc=${BASH_SOURCE##*/}

    cd $gpLocalTopDir >/dev/null 2>&1
    fLog -p info -m "mkdir $gpLocalRawDir" -l $tSym:$LINENO
    mkdir $gpLocalRawDir

    fLog -p notice -m "Create: TOP_DIR/$gpLocalRawSymLink -> $gpLocalRawDir" -l $tSrc:$LINENO
    ln -s $gpLocalRawDir $gpLocalRawSymLink

    cd $gpLocalTopDir/$gpLocalRawSymLink >/dev/null 2>&1
    fLog -p info -m "Create: raw/README.txt" -l $tSym:$LINENO
    cat <<EOF | fmt >README.txt
    
Do NOT remove these files or this directory.

These files are managed by the "git proj" subcommand. It's purpose is
to keep large binary files out of git repositories. Large files are
put in this directory and symlinks in the git managed area will point
to the files.

The initial working directory was created at $cHostName:$gpLocalTopDir

For help with the "git proj" subcommand, just cd into the working directory
and type "git proj".
EOF
    fLog -p notice -m "Use $gpLocalRawSymLink to access the files in $gpLocalRawDir  That way, if $gpLocalRawDir moves, only one place needs to be fixed up." -l $tSrc:$LINENO
    return 0
} # fInitMkRaw

# --------------------
fInitMoveBinaryFiles()
{
    local tSrc=${BASH_SOURCE##*/}
    local tFileList
    local tFile
    local tDir
    local tResult

    cd $gpLocalTopDir >/dev/null 2>&1
    tFileList=$(fInitGetBinaryFiles)
    if [ $? -ne 0 ]; then
        return 0
    fi
    if [ "$gpAutoMove" = "false" ]; then
        fLog -p warning -m "Binary files were found, but will not be moved." -l $tSrc:$LINENO
        return 0
    fi
    fLog -p notice -m "Moving large binary files." -l $tSrc:$LINENO
    for tFile in $tFileList; do
        tDir=${tFile%/*}
        if [ "$tDir" = "$tFile" ]; then
            tDir=""
        else
            if [ ! -d $gpLocalRawSymLink/$tDir ]; then
                tResult=$(mkdir -v -p $gpLocalRawSymLink/$tDir 2>&1)
		fLog -p info -m "$tResult" -l $tSym:$LINENO
            fi
            if [ ! -d $gpLocalRawSymLink/$tDir ]; then
                fError -i -m "Could not create: $gpLocalRawSymLink/$tDir" -l $tSrc:$LINENO
            fi
            fLog -p info -m "Exists: $gpLocalRawSymLink/$tDir" -l $tSrc:$LINENO
        fi
        if [ "$gpHardLink" = "true" ]; then
            tResult=$(mv -v $tFile $gpLocalRawSymLink/$tDir)
	    fLog -p info -m "mv: $tResult" -l $tSym:$LINENO
        else
            tResult=$('rsync' -av --info=stats0,name2,flist0 $tFile $gpLocalRawSymLink/$tDir)
	    fLog -p info -m "rsync: $tResult" -l $tSym:$LINENO
        fi
        if [ ! -s $gpLocalRawSymLink/$tFile ]; then
            fError -i -m "Could not move: $tFile" -l $tSrc:$LINENO
        fi
        if [ -f $tFile ]; then
            rm $tFile
        fi
        ln -s $gpLocalRawSymLink/$tFile $tFile
        if [ ! -L $tFile ]; then
            fError -i -m "Could not create symlink for: $tFile" -l $tSrc:$LINENO
        fi
        tLink=$('ls' -l $tFile | awk '{print $9,$10,$11}')
        fLog -p notice -m "Created link: $tLink" -l $tSrc:$LINENO
    done

    fLog -p notice -m "Version and use the file symlinks, with no changes in your code. Or better, delete the symlinks and access the files through the project's $gpLocalRawSymLink symlink." -l $tSrc:$LINENO

    return 0
} # fInitMoveBinaryFiles

# --------------------
fInitMkGitFlow()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ "$gpGitFlow" = "false" ]; then
        return 0
    fi
    if ! fComCheckPkg git-flow; then
        fLog -p err -m "git-flow is not installed" -l $tSrc:$LINENO
        return 1
    fi
    fLog -p info -m "git flow init" -l $tSrc:$LINENO
    git flow init -d

    # Define the default prefixes

    fComSetConfig -g -k gitflow.branch.main -v main
    fComSetConfig -g -k gitflow.branch.develop -v develop

    fComSetConfig -g -k gitflow.prefix.feature -v feature/
    fComSetConfig -g -k gitflow.prefix.bugfix -v bug/
    fComSetConfig -g -k gitflow.prefix.release -v release/
    fComSetConfig -g -k gitflow.prefix.hotfix -v hotfix/
    fComSetConfig -g -k gitflow.prefix.support -v support/

    return 0
} # fInitMkGitFlow

# --------------------
fInitMkGitDir()
{
    local tSrc=${BASH_SOURCE##*/}
    local tStatus

    cd $gpLocalTopDir >/dev/null 2>&1
    fLog -p info -m "Create: .gitignore" -l $tSrc:$LINENO
    cat <<EOF >.gitignore
*.tmp
*~
.cache/
.cvsignore
CVS/
core
nohup.out
proxy/
thumbs/
tmp/
EOF
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
    fInitMkGitFlow
    git branch
    git status

    return 0
} # fInitMkGitDir

# --------------------
fInitMkLocalConfig()
{
    local tSrc=${BASH_SOURCE##*/}

    cd $gpLocalTopDir >/dev/null 2>&1
    fLog -p info -m "Create: .gitproj.config.local" -l $tSrc:$LINENO
    cp $gpDoc/config/gitproj.config.local .gitproj.config.local
    fLog -p info -m "Create: .gitproj.config.$cHostName" -l $tSrc:$LINENO
    cp $gpDoc/config/gitproj.config.HOSTNAME .gitproj.config.$cHostName

    tInclude=$(fComGetConfig -l -k "include.path" -v .gitproj.config.$cHostName)
    if [ -n "$tInclude" ]; then
        fLog -i -p warning -m "Weird, .gitproj.config.$cHostName is already included" -l $tSrc:$LINENO
        return 0
    fi

    fLog -p info -m "Include: ../.gitproj.config.$cHostName in .git/config" -l $tSrc:$LINENO
    fComSetConfig -a -l -k "include.path" -v ../.gitproj.config.$cHostName

    return 0
} # fInitMkLocalConfig

# --------------------
fInitSaveSingleConfig()
{
    local pFile="$1"
    local pKey="$2"
    local pVarName="$3"
    
    local tSrc=${BASH_SOURCE##*/}
    local tValue

    tValue=$(eval echo \$$pVarName)
    if [ -z "$tValue" ]; then
        tValue="Undefined"
    fi
    git config --file $pFile $pKey "$tValue"
    if [ $? -ne 0 ]; then
        fLog -i -p warning -m "There was problem saving $pVarName value to $pKey to file $pFile" -l $tSrc:$LINENO
	gErr=1
    fi
} # fInitSaveSingleConfig

# --------------------
fInitSaveVarsToConfigs()
{
    local tSrc=${BASH_SOURCE##*/}
    local tFile
    local tS

    gErr=0

    tFile=~/.gitproj.config.global
    tS=gitproj.config
    fInitSaveSingleConfig $tFile $tS.proj-status gpProjStatus
    fInitSaveSingleConfig $tFile $tS.bin gpBin
    fInitSaveSingleConfig $tFile $tS.doc gpDoc
    fInitSaveSingleConfig $tFile $tS.test gpTest
    fInitSaveSingleConfig $tFile $tS.facility gpFacility
    fInitSaveSingleConfig $tFile $tS.syslog gpSysLog

    for tFile in $gpLocalTopDir/.gitproj.config.local \
    	    $gpLocalTopDir/.gitproj.config.$cHostName; do
    	tS=gitproj.config
    	fInitSaveSingleConfig $tFile $tS.local-status   gpLocalStatus
    	fInitSaveSingleConfig $tFile $tS.remote-status   gpRemoteStatus
    	fInitSaveSingleConfig $tFile $tS.proj-name     gpProjName
    	fInitSaveSingleConfig $tFile $tS.local-top-dir gpLocalTopDir
    done
    
    for tFile in ~/.gitproj.config.global \
    	    $gpLocalTopDir/.gitproj.config.local \
    	    $gpLocalTopDir/.gitproj.config.$cHostName; do
        tS=gitproj.config
        fInitSaveSingleConfig $tFile $tS.git-flow-pkg      gpGitFlow
        fInitSaveSingleConfig $tFile $tS.hardlink          gpHardLink
        fInitSaveSingleConfig $tFile $tS.local-raw-dir-pat gpLocalRawDirPat
        fInitSaveSingleConfig $tFile $tS.local-raw-symlink gpLocalRawSymLink
        tS=gitproj.hook
        fInitSaveSingleConfig $tFile $tS.auto-move              gpAutoMove
        fInitSaveSingleConfig $tFile $tS.binary-file-size-limit gpMaxSize
        fInitSaveSingleConfig $tFile $tS.check-file-names       gpCheckFileNames
        fInitSaveSingleConfig $tFile $tS.check-for-big-files    gpCheckForBigFiles
       fInitSaveSingleConfig $tFile $tS.pre-commit-enabled     gpPreCommitEnabled
        fInitSaveSingleConfig $tFile $tS.source                 gpHookSource
    done

    if [ $gErr -eq 0 ]; then
	fLog -p info -m "Save local configs" -l $tSrc:$LINENO
        git add .gitproj.config.*
    	git commit -am "Save gitproj.config"
    fi

    return $gErr
} # fInitSaveVarsToConfigs

# --------------------
fInitCreateLocalGit()
{
    # Called by "git proj init"
    local tSrc=${BASH_SOURCE##*/}

    fInitGettingStarted

    # Prompt/Response or if -a, validate the options
    
    # -l pLocalDir
    fInitGetLocalPath $gpLocalTopDir
    
    # -k pSymLinkPat
    fInitGetLocalRawDirPat $gpLocalRawDirPat
    
    # combine gpLocalTopDir,  pSymLinkPat, and gpProjName
    fInitGetSymLink $gpLocalRawSymLink

    # -s pSize
    fInitGetSize $gpMaxSize

    # -m (if -a default is true)
    fInitGetMoveFiles $gpAutoMove

    # -f
    fInitGetGitFlow $gpGitFlow

    fInitSummary
    
    fInitMkRaw
    fInitMoveBinaryFiles
    
    fInitMkGitDir
    fInitMkLocalConfig

    gpLocalStatus=defined
    fInitSaveVarsToConfigs

    cat <<EOF | fmt

You now have a local git repository project (${gpProjName}) setup at
${gpLocalTopDir} with support for managing large binary files (larger
than $gpMaxSize).

~/gitconfig will include the ~/gitproj.config.global file, which will
be used for defaults when you create other "git proj" projects on your
system.  The variables will also be used, if are not overridden by the
project's config definitions found at: .gitproj.config.local and
.gitproj.config.\$HOSTNAME You should version these files with your
project.

When you "clone" your git project to another system, a new
.gitproj.config.\$HOSTNAME will be created, so that differences
between systems are saved. For example, mount points and filesystem
differences.

Now would be a good time to setup an external drive for archiving your
project: "git proj init remote -d pMountPath"

[$tSrc:$LINENO]
EOF
    return 0
} # fInitCreateLocalGit

# ========================================
# fInitCreateRemoteGit Section

# --------------------
fInitCheckDir()
{
    local pPath=$1

    local tSrc=${BASH_SOURCE##*/}

    if [ ! -d $pPath ]; then
        fError -n -m "Could not find: $pPath" -l $tSrc:$LINENO
        return 1
    fi
    if [ ! -w $pPath ]; then
        fError -n -m "$pPath is not writable for you." -l $tSrc:$LINENO
        return 1
    fi
    if [ -e $pPath/$gpProjName.git ]; then
        fError -n -m "$pPath/$gpProjName.git already exists." -l $tSrc:$LINENO
        return 1
    fi
    return 0
} # fInitCheckDir

# --------------------
fInitGetMountDir()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ $gpAuto -ne 0 ]; then
        if [ -z "$gpMountDir" ]; then
            fError -m "The -d option is required." -l $tSrc:$LINENO
	fi
    	if ! fInitCheckDir $gpMountDir; then
	    return 1
	fi
	return 0
    fi

} # fInitGetMountDir

# --------------------
fInitCheckDirSpace()
{
    local pPath=$1
    local pMinSpace=$2

    local tSrc=${BASH_SOURCE##*/}
    local tAvail
    local tGitSize
    local tRawSize
    local tDesiredSize

    tAvail=$(df -BM $pPath --output=avail | tail -n1)
    tAvail=${tAvail%M}

    tGitSize=$(du -sm .git | awk '{print $1}')
    let tGitSize*=2

    tRawSize=$(du -sm raw | awk '{print $1}')
    let tRawSize*=2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tDesiredSize -gt $tAvail ]; then
        fError -m "There is not enough space at $pPath. $pPath has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi
} # fInitCheckDirSpace

# --------------------
fInitGetMinSpace()
{
    local tSrc=${BASH_SOURCE##*/}
    
    if [ $gpAuto -ne 0 ]; then
        if ! fInitCheckDirSpace $gpMountDir $gpRemoteMinSpace; then
	   echo TBD
	   return 1
        fi
	return 0
    fi
    return 0
} # fInitGetMinSpace

# --------------------
fInitGetRemoteRawDir()
{
    local tSrc=${BASH_SOURCE##*/}

    echo TBD
    #$gpRemoteRawDir
} # fInitGetRawRemotePath

# --------------------
fInitMkRemote()
{
    local pPath=$1

    local tSrc=${BASH_SOURCE##*/}
    local tSrcPath=$PWD
    local tProjName=${PWD##*/}

    set -x >/dev/null 2>&1
    cd $pPath >/dev/null 2>&1
    fLog -p info -m "git clone" -l $tSrc:$LINENO
    git clone --bare --local --no-hardlinks --progress -v $tSrcPath/.git
    cd - >/dev/null 2>&1
    git remote add origin $pPath/$tProjName.git
    set +x >/dev/null 2>&1
} # fInitMkRemote

# --------------------
fInitReport()
{
    local tSrc=${BASH_SOURCE##*/}

    echo 'Remote origin is now set to:'
    $cGetOrigin
    cat <<EOF
Be sure the disk is mounted and $gpPath exists.
Then you can use: "git push origin develop"
If the mount path is changed, use "git proj set -e" to update the origin.
EOF
} # fInitReport

# --------------------
fInitCreateRemoteGit()
{
    # Called by: "git proj remote -d pMountPath"
    local tSrc=${BASH_SOURCE##*/}

    # Prompt/Response or if -a, validate the options

    if ! fComMustBeInGitRepo $PWD; then
       fUsage usage
    fi

    # TBD: check for already defined    

    # -d pMountPath
    fInitGetMountDir $gpMountDir

    # -s pMinSpace
    fInitGetMinSpace $gpRemoteMinSpace
    
    # -r pRawRemotePath
    fGetRemoteRawDir $gpRemoteRawDir

    fInitMkRemote $gpPath
    fInitReport
} # fCreateRemoteGit

# ========================================
export tSrc=${BASH_SOURCE##*/}
fInitSetGlobals
