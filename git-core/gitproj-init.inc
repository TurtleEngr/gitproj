#!/bin/bash

# --------------------
export cExpectVer cRawDir gpAction gpAuto gpFacility gpGitFlow
export gpLocalPath gpMaxSize gpMove gpProjName gpRawLocalPath gpSymLinkName
export gpSysLog

# --------------------
fInitFirstTimeSet()
{
    local tInclude

    if [ ! -f $HOME/.gitproj.config.global ]; then
        cp $cDoc/config/gitproj.config.global $HOME/.gitproj.config.global
    fi
    tInclude=$(fComGetConfig -g -k "include.path" -v .gitproj.config.global)
    if [ -z "$tInclude" ]; then
    	fComSetConfig -a -g -k "include.path" -v .gitproj.config.global
    fi
} # fInitFirstTimeSet

# --------------------
fInitSetGlobals()
{
    fInitFirstTimeSet
    
    # TBD set the default to the installed version
    cExpectVer=$(fComGetConfig -k "gitproj.config.ver" -d "1.1")

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d 1)
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpAuto=0

    # Absolute or relative to top-dir
    cRawDir=$(fComGetConfig -k "gitproj.config.local-raw-dir" -d "..")

    gpSymLinkName=$(fComGetConfig -k "gitproj.config.local-raw-symlink" -d raw)

    gpLocalPath=$(fComGetConfig -k "gitproj.config.top-dir" -d "$PWD")
    if [ "$gpLocalPath" = "TBD" ]; then
        gpLocalPath=$PWD
    fi
    if [ ! -d $gpLocalPath ]; then
        gpLocalPath="TBD"
    fi
    
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "TBD")
    if [ "$gpProjName" = "TBD" ]; then
        gpProjName="${gpLocalPath##*/}"
    fi

    # TBD - path is relative to top-dir? Make it absolute?
    gpRawLocalPath=${cRawDir}/${gpProjName}.raw

    gpMaxSize=$(fComGetConfig -k "gitproj.hook.binary-file-size-limit" -d "1k")
    gpMove=0
    gpGitFlow=0
    gpAction=""

    return 0
} # fInitSetGlobals

# ========================================
# fInitCreateLocalGit Section

# --------------------
fInitGettingStarted() {
    cat <<EOF

Be sure you are "cd" to the "top" project directory. Or use the "-l
pLocalPath" option. The directory name will be used for the git repo
name. The project directory should not already be a git repo, and it
cannot contain any git repos under it.  Also, all of the directories
and files need to be readable by your user.

EOF
    if [ $gpAuto -ne 0 ]; then
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

    gpLocalPath=$pLocalPath
    gpProjName="${gpLocalPath##*/}"
    echo "The project Name will be: $gpProjName"
    return 0
} # fInitValidLocalPath

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
fInitValidRawLocalPath()
{
    local pRawLocalPath=$1

    local tSrc=${BASH_SOURCE##*/}

    if [ "${pRawLocalPath#../}" = "$pRawLocalPath" ]; then
        fLog -p warning -m "RawLocalPath works best if it is relative to LocalPath" -l $tSrc:$LINENO
    fi

    cd $gpLocalPath >/dev/null 2>&1
    if [ ! -d $pRawLocalPath ]; then
        fLog -p err -m "Raw directory not found: $pRawLocalPath" -l $tSrc:$LINENO
        cd $cCurDir >/dev/null 2>&1
        return 1
    fi

    cd $pRawLocalPath >/dev/null 2>&1
    echo $PWD | grep -q "/$gpProjName/"
    # shellcheck disable=SC2181
    if [ $? -ne 0 ]; then
        fLog -p err -m "Raw directory cannot be in $gpLocalPath" -l $tSrc:$LINENO
    fi

    gpRawLocalPath=$pRawLocalPath
    cat <<EOF | fmt
The $gpRawLocalPath/README.txt file will point to the project dir.
EOF
    cd $cCurDir >/dev/null 2>&1
    return 0
} # fInitValidRawLocalPath

fInitGetRawLocalPath()
{
    local pRawLocalPath=$1
    
    local tSrc=${BASH_SOURCE##*/}
    local tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidRawLocalPath $pRawLocalPath; then
            fError -m "Invalid -l pRawLocalPath: $pRawLocalPath" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while true; do
        cat <<EOF | fmt
Set the location for large binary files. It cannot be *in* any of the
project directories. It will be created, if it does not existi. The
path must be relative to the project dir (and not too far away or
cloning the project may not work). The files in the raw directory will
NOT be versioned--only the latest copy will be saved. Default: ../Name.raw
EOF
        echo -n "Raw dir (-r) [../$gpProjName.raw | quit]? "
        read -r tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=../$gpProjName.raw
        fi
        if fValidRawLocalPath $tResult; then
            break
        fi
    done
} # fInitGetRawLocalPath

# --------------------
fInitValidSymLink()
{
    local pSymLinkName=$1

    local tSrc=${BASH_SOURCE##*/}

    cd $gpLocalPath >/dev/null 2>&1
    if [ ! -e $pSymLinkName ]; then
        fLog -p err -m "Symlink: $pSymLinkName already exists. Pick another name or quit and remove it." -l $tSrc:$LINENO
        cd $cCurDir >/dev/null 2>&1
        return 1
    fi

    gpSymLinkName=$pSymLinkName
    cd $cCurDir >/dev/null 2>&1
    return 0
} # fInitValidSymLink

fInitGetSymLink()
{
    local pSymLinkName=$1
    
    local tSrc=${BASH_SOURCE##*/}
    local tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidSymLink $pSymLinkName; then
            fError -m "Invalid -k pSymLinkName: $pSymLinkName" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while true; do
        cat <<EOF | fmt
Define the symlink name that will point to the Raw dir. It will be
created in the project's top dir. If the Raw dir is moved, this link
will need to be updated. Symlink in the git directories will point to
this dir and the symlinks will be versioned.

EOF
        echo -n "Symlink (-k) [$pSymLinkName/quit]? "
        read -r tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -m warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$pSymLinkName
        fi
        if fValidSymLink $tResult; then
            break
        fi
    done
    return 0
} # fInitGetSymLink

# --------------------
fInitValidSize()
{
    local pMaxSize=$1

    local tSrc=${BASH_SOURCE##*/}

    echo $pMaxSize | grep -q '[0-9]+[bkmg]'
    # shellcheck disable=SC2181
    if [ $? -eq 0 ]; then
        gpMaxSize=$pMaxSize
        return 0
    fi
    fLog -p err -m "Size must be numbers followed by b, k, m, or g" -l $tSrc:$LINENO
    return 1
} # fInitValidSize

fInitGetSize()
{
    local pMaxSize=$1

    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidSize $pMaxSize; then
            fError -m "Invalid -s pSize: $pMaxSize" -l $tSrc:$LINENO
        fi
        return 0
    fi

    while true; do
        cat <<EOF | fmt
Define the "size" for large binary files. Format: Nb, Nk, Nm, or Ng
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
        if fValidSize $tResult; then
            break
        fi
    done
} # fInitGetSize

# --------------------
fInitGetBinaryFiles()
{
    local tFile
    local tFileList=""
    local tSize

    tSize=$(echo "$gpMaxSize" | tr 'bBKmg' 'cckMG')

    for tFile in $(find $gpLocalPath -type f -size +$tSize); do
        if [ $(tr -d '[ -~]\0' <$tFile | wc -c) != 0 ]; then
            tFileList="$tFileList, $tFile"
        fi
    done
    if [ -n "$tFileList" ]; then
        cat <<EOF | fmt
These binary files are greater than $gpMaxSize:

    ${tFileList#, }

EOF
        return 0
    fi
    return 1
} # fInitGetBinaryFiles

# --------------------
fInitGetMoveFiles()
{
    local tSrc=${BASH_SOURCE##*/}

    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    if ! fGetBinaryFiles; then
        fLog -p warning -m 'No "large" binary files were found.' -l $tSrc:$LINENO
        return 0
    fi

    while true; do
        cat <<EOF | fmt
The listed files can be moved to the project's raw diirectory. Paths
will be created in the raw directory that correspond to the project's
directory. A symlink will replace the moved file. The symlink will
point to [raw]

EOF
        echo -n "Move the files (-m) [y/n/quit]? "
        read -r tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ $tResult = "q" ]; then
            fLog -p warning -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ $tResult = "y" ]; then
            gpMove=1
            break
        fi
        if [ $tResult = "n" ]; then
            gpMove=0
            break
        fi
    done
    return 0
} # fInitGetMoveFiles

# --------------------
fInitGetGetFlow()
{
    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult
    local tGitFlowInstalled

    fCheckPkg git-flow
    tGitFlowInstalled=$?

    if [ $tGitFlowInstalled -ne 0 ] [ $tGitFlowInstalled -ne 0 ]; then
        fLog -p warning -m "git-flow is not installed." -l $tSrc:$LINENO
        gpGitFlow=0
    fi
    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    while true; do
        echo -n "Setup git-flow (-f) [y/n/quit]? "
        read -r tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ "$tResult" = "q" ]; then
            fLog -p warnint -m "Quitting" -l $tSrc:$LINENO
            exit 1
        fi
        if [ "$tResult" = "y" ]; then
            gpGitFlow=1
            break
        fi
        if [ "$tResult" = "n" ]; then
            gpGitFlow=0
            break
        fi
    done
    return 0
} # fInitGetGetFlow

# --------------------
fInitMkGitDir()
{
    declare -l tResult

    if [ $gpAuto -eq 0 ]; then
        while true; do
            echo -n "Continue with creating a local git repo [y/n/quit]? "
            read -r tResult
            tResult=${tResult%${tResult#[a-z]}}
            if [ $tResult = "q" ]; then
                fLog -p warning -m "Quitting" -l $tSrc:$LINENO
                exit 1
            fi
            if [ $tResult = "y" ]; then
                break
            fi
            if [ $tResult = "n" ]; then
                flog -p warning -m "Quitting, nothing will be done." -l $LINE
                exit 1
            fi
        done
    fi

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
    set -x >/dev/null 2>&1
    git init
    git add .gitignore ./*
    git commit -am "Initial commit"
    git branch --move master main
    git branch develop
    git checkout develop
    set +x >/dev/null 2>&1
    if fCheckPkg git-flow; then
        set -x >/dev/null 2>&1
        git flow init -d
        set +x >/dev/null 2>&1
    fi
    set -x >/dev/null 2>&1
    git branch
    git status
    set +x >/dev/null 2>&1
} # fInitMkGitDir

fInitMkRaw()
{
    echo "TBD"
    # The $gpRawLocalPath/README.txt file will point to the project dir.
} # fMkRaw

# --------------------
fInitCreateLocalGit()
{
    fInitGettingStarted

    # Prompt/Response or if -a, just validate the options
    fInitGetLocalPath $gpLocalPath
    fInitGetRawLocalPath $gpRawLocalPath
    fInitGetSymLink $gpSymLinkName
    fInitGetSize $gpMaxSize
    fInitGetMoveFiles $gpMove
    fInitGetGetFlow $gpGitFlow

    fInitMkGitDir
    fInitMkRaw
} # fInitCreateLocalGit

# ========================================
# fInitCreateRemoteGit Section

fInitGetMountPath()
{
    #$gpMountPath
    fCheckPath $gpPath	# gpPath??? TBD
} # fInitGetMountPath

fInitGetRawRemotePath()
{
    echo TBD
    #$gpRawRemotePath
} # fInitGetRawRemotePath

# --------------------
fInitCheckPath()
{
    local pPath=$1

    local tSrc=${BASH_SOURCE##*/}
    local tName=${PWD##*/}

    if [ ! -d $pPath ]; then
        fError -m "Could not find: $pPath" -l $tSrc:$LINENO
    fi
    if [ ! -w $pPath ]; then
        fError -m "$pPath is not writable for you." -l $tSrc:$LINENO
    fi
    if [ -e $pPath/$tName.git ]; then
        fError -m "$pPath/$tName.git already exists." -l $tSrc:$LINENO
    fi
} # fInitCheckPath

fInitCheckSpace()
{
    local pPath=$1

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
    let tRawSize*+2

    let tDesiredSize=tGitSize+tRawSize

    if [ $tDesiredSize -gt $tAvail ]; then
        fError -m "There is not enough space at $pPath. $pPath has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB" -l $tSrc:$LINENO
    fi
} # fInitCheckSpace

# --------------------
fInitMkRemote()
{
    local pPath=$1

    local tSrcPath=$PWD
    local tProjName=${PWD##*/}

    set -x >/dev/null 2>&1
    cd $pPath >/dev/null 2>&1
    git clone --bare --local --no-hardlinks --progress -v $tSrcPath/.git
    cd - >/dev/null 2>&1
    git remote add origin $pPath/$tProjName.git
    set +x >/dev/null 2>&1
} # fInitMkRemote

# --------------------
fInitReport()
{
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
    fGetMountPath $gpMountPath		# gpMountPath??? TBD
    #    	fCheckPath $gpPath
    fGetRawRemotePath $gpRawRemotePath	# gpRawRemotePath??? TBD

    fMustBeInGitRepo
    fCheckSpace $gpPath
    fMkRemote $gpPath
    fReport
} # fCreateRemoteGit

# ========================================
export tSrc=${BASH_SOURCE##*/}
fInitSetGlobals
