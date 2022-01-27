#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpAuto gpAutoMove gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCheckInRaw gpCheckWhitespace gpCmdName gpDebug gpDoc gpFacility \
    gpGitFlow gpHookVerbose gpLocalRawDir gpLocalStatus \
    gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled gpProjName \
    gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose

# ========================================

# --------------------
fInitGettingStarted()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=5

    cat <<EOF

Be sure you are "cd" to the "top" project directory,then use "-l
$PWD".  The directory name will be used for the git repo name. The
project directory should not already be a git repo, and it cannot
contain any git repos under it.  Also, all of the directories and
files need to be readable by your user (git proj init will verify
all of the above is true.

You only need to answer 5 questions to create the git part of this
project.

EOF
    if [ $gpAuto -ne 0 ]; then
        fLog -p debug-2 -m "fInitGettingStarted in $PWD" -l $tSrc:$LINENO
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
        fLog -p notice -m "gitproj.config.proj-name = $gpProjName" -l $tSrc:$LINENO
        return 0
    fi
    cat <<EOF | fComFmt

Define the existing project directory. The last directory will be used
for the name of the project. If you don't want this name, type "n",
then rename the directory you are in or move the files. You can start
with an empty directory.

EOF
    if ! fInitValidLocalPath $pLocalPath; then
        exit 1
    fi
    echo "Project Path: -l $pLocalPath"
    echo -n "Project Name: [${pLocalPath##*/}] Continue [y/n]? "
    read -r tResult
    echo
    if [ "$tResult" = "n" ]; then
        fLog -f -p warning -m "Quitting" -l $tSrc:$LINENO
        exit 1
    fi

    fLog -p notice -m "gitproj.config.proj-name = $gpProjName" -l $tSrc:$LINENO

    return 0
} # fInitGetLocalPath

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
    local tMaxLoop=5
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fInitValidSize $pMaxSize; then
            fError -m "Invalid -s pSize: $pMaxSize" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop

        cat <<EOF | fComFmt

Define the size for large binary files. Format: Nb, Nk, Nm, or Ng
Press enter to accept the value shown, or type a new value.
(^c to quit this program.)

EOF
        echo "Size (-s) [$pMaxSize]? "
        read -r tResult
        if [ -z "$tResult" ]; then
            tResult=$pMaxSize
        fi
        if fInitValidSize $tResult; then
            break
        fi
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    fLog -p notice -m "gitproj.hook.proj-binary-file-size = gpMaxSize" -l $tSrc:$LINENO

    return 0
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
        echo $tFileList | fComFmt
        return 0
    fi
    return 1
} # fInitGetBinaryFiles

# --------------------
fInitGetMoveFiles()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=5
    declare -l tResult

    echo
    fLog -p notice -m "Looking for binary files are greater than $gpMaxSize:" -l $tSrc:$LINENO

    if ! fInitGetBinaryFiles; then
        fLog -p notice -m 'No large binary files were found.' -l $tSrc:$LINENO
        return 0
    fi

    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop

        cat <<EOF | fComFmt

The files, shown above, can be moved to the project's raw
directory. Paths will be created in the top raw directory A symlink
will replace the moved file. The symlink will point to
[raw/path]. ^c if you want to change the MaxSize.

EOF
        # TBD convert this logic to use fComSelect()
        echo -n "Move the files (-m) [y/n]? "
        read -r tResult
        case $tResult in
            y*)
                gpAutoMove=1
                echo "Binary files will be moved."
                echo
                break
                ;;
            n*)
                gpAutoMove=0
                echo "Binary files will NOT be moved."
                echo
                break
                ;;
        esac
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    return 0
} # fInitGetMoveFiles

# --------------------
fInitGetGitFlow()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=5
    declare -l tResult

    fComCheckPkg $gpGitFlowPkg
    if [ $? -ne 0 ]; then
        echo
        fLog -p warning -m "git-flow is not installed. It can be added later." -l $tSrc:$LINENO
        echo
        gpGitFlow="false"
        return 0
    fi
    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    while [ $tMaxLoop -gt 0 ]; do
        let --tMaxLoop
        echo
        echo -n "Setup git-flow (-f) [y/n]? "
        read -r tResult
        tResult=${tResult%${tResult#[a-z]}}
        echo
        if [ "$tResult" = "y" ]; then
            gpGitFlow="true"
            fLog -p notice -m "gitproj.config.git-flow = true" -l $tSrc:$LINENO
            echo
            break
        fi
        if [ "$tResult" = "n" ]; then
            gpGitFlow="false"
            fLog -p notice -m "gitproj.config.git-flow = false" -l $tSrc:$LINENO
            break
        fi
    done
    if [ $tMaxLoop -eq 0 ]; then
        fError -i -m "Possible infinite loop." -l $tSrc:$LINENO
    fi

    return 0
} # fInitGetGitFlow

# --------------------
fInitSummary()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMaxLoop=5
    declare -l tResult

    cat <<EOF

Summary
    (auto) = $gpAuto
    (verbose) = $gpVerbose
    (debug) = $gpDebug
    (exits)   HOME/.gitconfig
    (created)   HOME/.gitconfig
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
    proj-name = $gpProjName
    git-flow-pkg = $gpGitFlowPkg
    verbose = $gpVerbose

[gitproj "hook"]
    verbose = $gpHookVerbose
    pre-commit-enabled = $gpPreCommitEnabled
    check-file-names = $gpCheckFileNames
    check-in-raw = $gpCheckInRaw
    check-whitespace = $gpCheckWhitespace
    check-for-big-files = $gpCheckForBigFiles
    binary-file-size = $gpMaxSize

The above variables will be put in these config files: ~/.gitconfig
(--global), PROJ/.git/config, (--local), PROJ/.gitproj. You can edit
these files directly or use "git config" when you are in the git-proj
directory. (PROJ/.git/config usually updated from PROJ/.git/config)

You can quit now, edit the values in ~/.gitconfig, and those values
will be used for the initial default values. For example you might
want to set "syslog = true", so most of these messages will be sent to
syslog.

EOF
    if [ $gpAuto -eq 0 ]; then
        while [ $tMaxLoop -gt 0 ]; do
            let --tMaxLoop
            echo -n "Continue with creating a local git repo [y/n]? "
            read -r tResult
            echo
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
    echo
    echo '===================================================================='
    echo "Creating git-proj repo for $gpProjName"
    echo
    return 0
} # fInitSummary

# --------------------
fInitMkRaw()
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
    echo
    fLog -p notice -m "Moving large binary files." -l $tSrc:$LINENO
    for tFile in $tFileList; do
        tDir=${tFile%/*}
        if [ "$tDir" = "$tFile" ]; then
            tDir=""
        else
            if [ ! -d $gpLocalRawDir/$tDir ]; then
                tResult=$(mkdir -v -p $gpLocalRawDir/$tDir 2>&1)
                fLog -p info -m "$tResult" -l $tSym:$LINENO
            fi
            if [ ! -d $gpLocalRawDir/$tDir ]; then
                fError -i -m "Could not create: $gpLocalRawDir/$tDir" -l $tSrc:$LINENO
            fi
            fLog -p info -m "Exists: $gpLocalRawDir/$tDir" -l $tSrc:$LINENO
        fi
        tResult=$(mv -v $tFile $gpLocalRawDir/$tDir)
        fLog -p info -m "mv: $tResult" -l $tSym:$LINENO
        if [ ! -s $gpLocalRawDir/$tFile ]; then
            fError -i -m "Could not move: $tFile" -l $tSrc:$LINENO
        fi
        if [ -f $tFile ]; then
            rm $tFile
        fi
        ln -s $gpLocalRawDir/$tFile $tFile
        if [ ! -L $tFile ]; then
            fError -i -m "Could not create symlink for: $tFile" -l $tSrc:$LINENO
        fi
        tLink=$('ls' -l $tFile | awk '{print $9,$10,$11}')
        fLog -p notice -m "Created link: $tLink" -l $tSrc:$LINENO
    done

    echo
    fLog -p notice -m "Version and use the file symlinks, with no changes in your code. Or better, delete the symlinks and access the files from the project's $gpLocalRawDir." -l $tSrc:$LINENO

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
        echo
        fLog -p warning -m "git-flow is not installed" -l $tSrc:$LINENO
        gpGitFlow=false
        fComSetConfig -g -k gitproj.config.git-flow -v false
        return 1
    fi
    echo
    fLog -p notice -m "git flow init" -l $tSrc:$LINENO
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
} # fInitMkGitFlow

# --------------------
fInitMkGitDir()
{
    local tSrc=${BASH_SOURCE##*/}
    local tStatus
    local tVerbose=""

    cd $gpLocalTopDir >/dev/null 2>&1

    if [ $gpVerbose -ge 2 ]; then
        tVerbose='-v'
    fi

    echo
    fLog -p notice -m "Create: .gitignore" -l $tSrc:$LINENO
    cp $tVerbose --backup=t $gpDoc/config/gitignore .gitignore

    echo
    fLog -p notice -m "git init" -l $tSrc:$LINENO
    git init

    echo
    fLog -p notice -m "git add FILES" -l $tSrc:$LINENO
    git add .gitignore *

    echo
    fLog -p notice -m "git commit" -l $tSrc:$LINENO
    git commit -am "Initial commit"
    tStatus=$?
    if [ $tStatus -ne 0 ]; then
        fError -n -i "Initial git commit failed." -l $tSrc:$LINENO
    fi

    echo
    fLog -p notice -m "Rename master to main" -l $tSrc:$LINENO
    git branch --move master main
    echo
    fLog -p notice -m "Create develop branch" -l $tSrc:$LINENO
    git branch develop
    git checkout develop

    fInitMkGitFlow
    echo
    git branch
    echo
    git status

    echo
    cp $tVerbose --backup=t $gpDoc/hooks/* $gpLocalTopDir/.git/hooks

    return 0
} # fInitMkGitDir

# --------------------
fInitMkLocalConfig()
{
    local tSrc=${BASH_SOURCE##*/}

    cd $gpLocalTopDir >/dev/null 2>&1

    echo
    fLog -p notice -m "Create: .gitproj" -l $tSrc:$LINENO
    touch .gitproj
    fComConfigCopy -s $HOME/.gitconfig -d .gitproj -i 'gitproj\.'

    echo
    fLog -p notice -m "Define: host values in --local" -l $tSrc:$LINENO
    fComConfigCopy -s .gitproj -d .git/config

    return 0
} # fInitMkLocalConfig

# --------------------
fInitSaveVarsToConfigs()
{
    local tSrc=${BASH_SOURCE##*/}
    local tFile
    local tS

    gErr=0

    tFile=$HOME/.gitconfig
    tS=gitproj.config
    fComSaveVar2Config $tFile $tS.proj-status gpProjStatus
    fComSaveVar2Config $tFile $tS.bin gpBin
    fComSaveVar2Config $tFile $tS.doc gpDoc
    fComSaveVar2Config $tFile $tS.facility gpFacility
    fComSaveVar2Config $tFile $tS.syslog gpSysLog

    for tFile in \
        $gpLocalTopDir/.gitproj \
        $gpLocalTopDir/.git/config; do
        tS=gitproj.config
        fComSaveVar2Config $tFile $tS.ver gpVer
        fComSaveVar2Config $tFile $tS.local-status gpLocalStatus
        fComSaveVar2Config $tFile $tS.remote-status gpRemoteStatus
        fComSaveVar2Config $tFile $tS.proj-name gpProjName
    done

    for tFile in $HOME/.gitconfig \
        $gpLocalTopDir/.gitproj \
        $gpLocalTopDir/.git/config; do
        tS=gitproj.config
        fComSaveVar2Config $tFile $tS.git-flow-pkg gpGitFlow
        tS=gitproj.hook
        fComSaveVar2Config $tFile $tS.verbose gpHookVerbose
        fComSaveVar2Config $tFile $tS.pre-commit-enabled gpPreCommitEnabled
        fComSaveVar2Config $tFile $tS.check-file-names gpCheckFileNames
        fComSaveVar2Config $tFile $tS.check-in-raw gpCheckInRaw
        fComSaveVar2Config $tFile $tS.check-whitespace gpCheckWhitespace
        fComSaveVar2Config $tFile $tS.check-for-big-files gpCheckForBigFiles
        fComSaveVar2Config $tFile $tS.binary-file-size gpMaxSize
    done

    if [ $gErr -eq 0 ]; then
        fLog -p info -m "Save local configs" -l $tSrc:$LINENO
        git add .gitproj
        git commit -am "Save .gitproj"
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
    fComIntroText

    cat <<EOF | fComFmt

Now would be a good time to setup an external drive for archiving your
project: "git proj remote -d pMountPath"

[$tSrc:$LINENO]
EOF
    return 0
} # fInitCreateLocalGit

# ========================================
if [ ${gpAuto:-0} -eq 0 ]; then
    gpVerbose=2
fi
fComPreProjSetGlobals
