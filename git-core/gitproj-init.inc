#!/bin/bash

# --------------------
export gpAuto gpLocalPath gpRawLocalPath gpSymLinkName gpMaxSize gpMove
export gpGitFlow gpAction gpProjName
. fSetInitGlobals

# --------------------
fSetInitGlobals()
{
    cExpectVer=$(fComGetConfig -k "gitproj.config.ver")
    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility")

    gpAuto=0
    gpLocalPath=$PWD
    gpProjName=${gpLocalPath##*/}"
    cRawDir=$(fComGetConfig -k "gitproj.config.raw-dir")
    gpRawLocalPath=$cRawDir/${gpLocalPath##*/}
    gpSymLinkName=$(fComGetConfig -k "gitproj.config.raw-symlink")
    gpMaxSize=$(fComGetConfig -k "gitproj.hook.binary-file-size-limit")
    gpMove=0
    gpGitFlow=0
    gpAction=""

} # fSetInitGlobals

# ========================================
# fCreateLocalGit Section

# --------------------
fCheckPkg()
{
    local pPkg=$1

    dpkg -l $pPkg 2>/dev/null | grep -q "^ii *$pPkg"
    return $?
} # fCheckPkg

# --------------------
fValidLocalPath()
{
	local pLocalPath=$1

	if [ ! -d $pLocalPath ]; then
	   fLog -p Err -m "Project directory not found: $pLocalPath" -l $LINENO
	   return 1
	fi

	gpLocalPath=$pLocalPath
	gpProjName=${gpLocalPath##*/}"
    echo "The project Name will be: $gpProjName"
    return 0
} # fValidLocalPath

fGetLocalPath()
{
    local pLocalPath=$1
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidLocalPath $pLocalPath; then
            fError -m "Invalid -l pLocalPath: $pLocalPath" -l $LINENO
        fi
        return 0
    fi
    while true; do
        cat <<EOF | fmt
Define the existing project directory. The last directory will be used
for the name of the project. Default: current directory
EOF
        read -p "Project Path (-l) [$pLocalPath/quit]? " tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -m "Quitting" -l $LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$pLocalPath
        fi
        if fValidLocalPath $tResult; then
            break
        fi
    done
    return 0
} # fGetLocalPath

# --------------------
fValidRawLocalPath()
{
    local pRawLocalPath=$1

    if [ "${pRawLocalPath#../}" = "$pRawLocalPath" ]; then
        fLog -p warning -m "RawLocalPath works best if it is relative to LocalPath" -l $LINENO
    fi

    cd $gpLocalPath >/dev/null 2>&1
    if [ ! -d $pRawLocalPath ]; then
        fLog -p err -m "Raw directory not found: $pRawLocalPath" -l $LINENO
        cd $cCurDir >/dev/null 2>&1
        return 1
    fi

    cd $pRawLocalPath >/dev/null 2>&1
    echo $PWD | grep -q "/$gpProjName/"
    if [ $? -ne 0 ]; then
        fLog -p err -m "Raw directory cannot be in $gpLocalPath" -l $LINENO
    fi

    gpRawLocalPath=$pRawLocalPath
    cat <<EOF | fmt
The $gpRawLocalPath/README.txt file will point to the project dir.
EOF
    cd $cCurDir >/dev/null 2>&1
    return 0
} # fValidRawLocalPath

fGetRawLocalPath()
{
    local pRawLocalPath=$1
    local tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidRawLocalPath $pRawLocalPath; then
            fError -m "Invalid -l pRawLocalPath: $pRawLocalPath" -l $LINENO
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
        read -p "Raw dir (-r) [../$gpProjName.raw | quit]? "
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -m "Quitting" -l $LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=../$gpProjName.raw
        fi
        if fValidRawLocalPath $tResult; then
            break
        fi
    done
} # fGetRawLocalPath

# --------------------
fValidSymLink()
{
    local pSymLinkName=$1

    cd $gpLocalPath >/dev/null 2>&1
    if [ ! -e $pSymLinkName ]; then
        fLog -p err -m "Symlink: $pSymLinkName already exists. Pick another name or quit and remove it." -l $LINENO
        cd $cCurDir >/dev/null 2>&1
        return 1
    fi

    gpSymLinkName=$pSymLinkName
    cd $cCurDir >/dev/null 2>&1
    return 0
} # fValidSymLink

fGetSymLink()
{
    local pSymLinkName=$1
    local tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidSymLink $pSymLinkName; then
            fError -m "Invalid -k pSymLinkName: $pSymLinkName" -l $LINENO
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
        read -p "Symlink (-k) [$pSymLinkName/quit]? " tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -m "Quitting" -l $LINENO
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
} # fGetSymLink

# --------------------
fValidSize()
{
    local pMaxSize=$1

    echo $pMaxSize | grep -q '[0-9]+[bkmg]'
    if [ $? -eq 0 ]; then
        gpMaxSize=$pMaxSize
        return 0
    fi
    fLog -p err -m "Size must be numbers followed by b, k, m, or g]" -l $LINENO
    return 1
} # fValidSize

fGetSize()
{
    local pMaxSize=$1
    declare -l tResult

    if [ $gpAuto -ne 0 ]; then
        if ! fValidSize $pMaxSize; then
            fError -m "Invalid -s pSize: $pMaxSize" -l $LINENO
        fi
        return 0
    fi

    while true; do
        cat <<EOF | fmt
Define the "size" for large binary files. Format: Nb, Nk, Nm, or Ng
EOF
        read -p "Size (-s) [$pSize/quit]? " tResult
        if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
            fLog -m "Quitting" -l $LINENO
            exit 1
        fi
        if [ -z "$tResult" ]; then
            tResult=$pSize
        fi
        if fValidSize $tResult; then
            break
        fi
    done
} # fGetSize

# --------------------
fGetBinaryFiles()
{
    local tFile
    local tFileList=""
    local tSize

    tSize=$(echo "$gpSize" | tr 'bBKmg' 'cckMG')

    for tFile in $(find $gpLocalPath -type f -size +$gpSize); do
        if [ $(tr -d '[ -~]\0' <$tFile | wc -c) != 0 ]; then
            tFileList="$tFileList, $tFile"
        fi
    done
    if [ -n "$tFileList" ]; then
        cat <<EOF | fmt
These binary files are greater than $gpSize:

    ${tFileLIst#, }

EOF
        return 0
    fi
    return 1
} # fGetBinaryFiles

# --------------------
fGetMoveFiles()
{
    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    if ! fGetBinaryFiles; then
        fLog -m 'No "large" binary files were found.' -l $LINENO
        return 0
    fi

    while true; do
        cat <<EOF | fmt
The listed files can be moved to the project's raw diirectory. Paths
will be created in the raw directory that correspond to the project's
directory. A symlink will replace the moved file. The symlink will
point to [raw]

EOF
        read -p "Move the files (-m) [y/n/quit]? " tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ $tResult = "q" ]; then
            fLog -m "Quitting" -l $LINENO
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
} # fGetMoveFiles

# --------------------
fGetGetFlow()
{
    declare -l tResult
    local tGitFlowInstalled

    fCheckPkg git-flow
    tGitFlowInstalled=$?

    if [ $tGitFlowInstalled -ne 0 ] [ $tGitFlowInstalled -ne 0 ]; then
        fLog -p warning -m "git-flow is not installed." -l $LINENO
        gpGitFlow=0
    fi
    if [ $gpAuto -ne 0 ]; then
        return 0
    fi

    while true; do
        read -p "Setup git-flow (-f) [y/n/quit]? " tResult
        tResult=${tResult%${tResult#[a-z]}}
        if [ "$tResult" = "q" ]; then
            fLog -m "Quitting" -l $LINENO
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
} # fGetGetFlow

# --------------------
fMkGitDir()
{
    declare -l tResult

    if [ $gpAuto -eq 0 ]; then
        while true; do
            read -p "Continue with creating a local git repo [y/n/quit]? " tResult
            tResult=${tResult%${tResult#[a-z]}}
            if [ $tResult = "q" ]; then
                fLog -m "Quitting" -l $LINENO
                exit 1
            fi
            if [ $tResult = "y" ]; then
                break
            fi
            if [ $tResult = "n" ]; then
                flog -m "Quitting, nothing will be done." -l $LINE
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
    git add .gitignore *
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
} # fMkGitDir

fMkRaw()
{
    echo "TBD"
    # The $gpRawLocalPath/README.txt file will point to the project dir.
} # fMkRaw

# --------------------
fCreateLocalGit()
{
    fMustBeNoGit
    fAllMustBeReadable
    
    # Prompt/Response or if -a, just validate the options
    fGetLocalPath $gpLocalPath
    fGetRawLocalPath $gpRawLocalPath
    fGetSymLink $gpSymLinkName
    fGetSize $gpMaxSize
    fGetMoveFiles $gpMove
    fGetGetFlow $gpGitFlow

    fMkGitDir
    fMkRaw
} # fCreateLocalGit

# ========================================
# fCreateRemoteGit Section

fGetMountPath()
{
    #$gpMountPath
    fCheckPath $gpPath
} # fGetMountPath

fGetRawRemotePath()
{
    echo TBD
    #$gpRawRemotePath
} # fGetRawRemotePath

# --------------------
fCheckPath()
{
    local pPath=$1

    local tName=${PWD##*/}

    if [ ! -d $pPath ]; then
        fError -m "Could not find: $pPath" -l $LINENO
    fi
    if [ ! -w $pPath ]; then
        fError -m "$pPath is not writable for you." -l $LINENO
    fi
    if [ -e $pPath/$tName.git ]; then
        fError -m "$pPath/$tName.git already exists." -l $LINENO
    fi
} # fCheckPath

fCheckSpace()
{
    local pPath=$1

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
        fError -m "There is not enough space at $pPath. $pPath has ${tAvail}MB. .git and raw needs ${tDesiredSize}MB" -l $LINENO
    fi
} # fCheckSpace

# --------------------
fMkRemote()
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
} # fMkRemote

# --------------------
fReport()
{
    echo 'Remote origin is now set to:'
    $cGetOrigin
    cat <<EOF
Be sure the disk is mounted and $gpPath exists.
Then you can use: "git push origin develop"
If the mount path is changed, use "git proj set -e" to update the origin.
EOF
} # fReport

# --------------------
fCreateRemoteGit()
{
    fGetMountPath $gpMountPath
    #    	fCheckPath $gpPath
    fGetRawRemotePath $gpRawRemotePath

    fMustBeInGitRepo
    fCheckSpace $gpPath
    fMkRemote $gpPath
    fReport
} # fCreateRemoteGit
