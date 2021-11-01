# --------------------
fCheckForGit()
{
    if [ ! -d .git ]; then
        fError -m ".git not found in current directory. cd to a directory with .git in it, or use "git proj init -l" to create a new local repo." -l $LINENO
    fi
}

# --------------------
fCheckForNoGit()
{
    if git $cOrigin; then
        git branch
        fError -m ".git already exists for the directory." -l $LINENO
        exit 1
    fi
}

# ========================================
# fCreateLocalGit Section

# --------------------
fCheckPkg()
{
    local pPkg=$1

    dpkg -l $pPkg 2>/dev/null | grep -q "^ii *$pPkg"
    return $?
}

# --------------------
fValidProjPath()
{
	local tProjPath=$1

	if [ ! -d $tProjPath ]; then
	   fLog -p Err -m "Project directory not found: $tProjPath" -l $LINENO
	   return 1
	fi

	gpProjPath=$tProjPath
	gpProjName=${gpProjPath##*/}"
	echo "The project Name will be: $gpProjName"
	return 0
}

fGetProjPath()
{
	declare -l tResult

	if [ $gpAuto -ne 0 ]; then
	   if ! fValidProjPath $gpProjPath; then
	      fError -m "Invalid -l pProjPath: $gpProjPath" -l $LINENO
	   fi
	   return 0
	fi
	while true; do
      	    cat <<EOF | fmt
Define the existing project directory. The last directory will be used
for the name of the project. Default: current directory
EOF
	    read -p "Project Path (-l) [$gpProjPath/quit]? " tResult
	    if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
	      	 fLog -m "Quitting" -l $LINENO
		 exit 1
	    fi
	    if [ -z "$tResult" ]; then
	       tResult=$gpProjPath
	    fi
	    if fValidProjPath $tResult; then
	       break
	    fi
	done
} # fGetProjPath

# --------------------
fValidRawPath()
{
	local tRawPath=$1

	if [ "${tRawPath#../}" = "$tRawPath" ]then
	   fLog -p warning -m "RawPath works best if it is relative to ProjPath" -l $LINENO
	fi

	cd $gpProjPath >/dev/null 2>&1
	if [ ! -d $tRawPath ]; then
	   fLog -p err -m "Raw directory not found: $tRawPath" -l $LINENO
	   cd $cCurDir >/dev/null 2>&1
	   return 1
	fi

	cd $tRawPath >/dev/null 2>&1
	echo $PWD | grep -q "/$gpProjName/"
	if [ $? -ne 0 ]; then
	   fLog -p err -m "Raw directory cannot be in $gpProjPath" -l $LINENO
	fi

	gpRawPath=$tRawPath
	cat <<EOF | fmt
The $gpRawPath/README.txt file will point to the project dir.
EOF
	cd $cCurDir >/dev/null 2>&1
	return 0
}

fGetRawPath()
{
	local tResult

	if [ $gpAuto -ne 0 ]; then
	   if ! fValidRawPath $gpRawPath; then
	      fError -m "Invalid -l pRawPath: $gpRawPath" -l $LINENO
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
	    read -p "Raw dir (-r) [../$Name.raw | quit]? "
	    if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
	      	 fLog -m "Quitting" -l $LINENO
		 exit 1
	    fi
	    if [ -z "$tResult" ]; then
	       tResult=../$Name.raw
	    fi
	    if fValidRawPath $tResult; then
	       break
	    fi
 	done
} # fGetRawPath

# --------------------
fValidSymLink()
{
	local tSymLinkName=$1

	cd $gpProjPath >/dev/null 2>&1
	if [ ! -d $tSymLinkName ]; then
????	   fLog -p err -m "Raw directory not found: $tRawPath" -l $LINENO
	   cd $cCurDir >/dev/null 2>&1
	   return 1
	fi

	cd $tRawPath >/dev/null 2>&1
	echo $PWD | grep -q "/$gpProjName/"
	if [ $? -ne 0 ]; then
	   fLog -p err -m "Raw directory cannot be in $gpProjPath" -l $LINENO
	fi

	gpRawPath=$tRawPath
	cat <<EOF | fmt
The $gpRawPath/README.txt file will point to the project dir.
EOF
	cd $cCurDir >/dev/null 2>&1
	return 0
}

fGetSymLink()
{
	local tResult

	if [ $gpAuto -ne 0 ]; then
	   if ! fValidSymLink $gpRawPath; then
	      fError -m "Invalid -k pSymLink: $gpSymLink" -l $LINENO
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
	    read -p "Symlink (-k) [$gpSynLinkName/quit]? " tResult
	    if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
	      	 fLog -m "Quitting" -l $LINENO
		 exit 1
	    fi
	    if [ -z "$tResult" ]; then
	       tResult=$gpSynLinkName
	    fi
	    if fValidSymLink $tResult; then
	       break
	    fi
 	done
	return 0
}

# --------------------
fValidSize()
{
	local tSize=$1

	echo $tSize | grep -q '[0-9]+[bkmg]'
	if [ $? -eq 0 ]; then
	   gpMaxSize=$tSize
    	   return 0
	fi
	fLog -m "Size must be numbers followed by b, k, m, or g]" -l $LINENO
	return 1
}

fGetSize()
{
	declare -l tResult
	declare -l tSize

	if [ $gpAuto -ne 0 ]; then
	   if ! fValidSize $gpSize; then
	      fError -m "Invalid -s pSize: $gpSize" -l $LINENO
	   fi
	   return 0
	fi

	while true; do
      	    cat <<EOF | fmt
Define the "size" for large binary files. Format: Nb, Nk, Nm, or Ng
EOF
	    read -p "Size (-s) [$gpSize/quit]? " tResult
	    if [ "$tResult" = "quit" ] || [ "$tResult" = "q" ]; then
	      	 fLog -m "Quitting" -l $LINENO
		 exit 1
	    fi
	    if [ -z "$tResult" ]; then
	       tResult=$gpSize
	    fi
	    if fValidSize $tResult; then
	       break
	    fi
	done
}

# --------------------
fGetBinaryFiles()
{
    local tFile
    local tFileList=""
    declare -l tResult
    local tSize

    tSize=$(tr 'bBKmg' 'cckMG')

    for tFile in $(find $gpProjPath -type f -size +$gpSize); do
        if [ $(tr -d '[ -~]\0' <$tFile | wc -c) != 0 ]; then
	    tFileList="$tFileList, $tFile"
	fi
    done
    if [ -n "$tFileList ]; then
      	cat <<EOF | fmt
These binary files are greater than $gpSize:

    ${tFileLIst#, }

EOF
	return 1
    fi
    return 0
}

# --------------------
fGetMoveFiles()
{
	if fGetBinaryFiles; then
	   return 0
	fi
	if [ $gpAuto -ne 0 ]; then
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
	done
	return 0
}

# --------------------
fGetGetFlow()
{
	declare -l tResult
	local tGitFlowInstalled

	fCheckPkg git-flow
	tGitFlowInstalled=$?

	if [ $tGitFlowInstalled -ne 0 ]  [ $tGitFlowInstalled -ne 0 ]; then
	      fLog -p warning -m "git-flow is not installed." -l $LINENO
	      gpGitFlow=0
	fi
	if [ $gpAuto -ne 0 ]; then
	   return 0
	fi

	while true; do
	      read -p "Setup git-flow (-f) [y/n/quit]? " tResult
	      tResult=${tResult%${tResult#[a-z]}}
	      if [ $tResult = "q" ]; then
	      	 fLog -m "Quitting" -l $LINENO
		 exit 1
	      fi
	      if [ $tResult = "y" ]; then
	      	 gpGitFlow=1
		 break
	      fi
	      if [ $tResult = "n" ]; then
	      	 gpGitFlow=0
		 break
	done
	return 0
}

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
}

# --------------------
fCreateLocalGit()
{
    fCheckForNoGit

    # Prompt/Response or if -a, just validate the options
    fGetProjPath
    fGetRawPath
    fGetSymLink
    fGetSize
    fGetMoveFiles
    fGetGetFlow

    fMkGitDir
} # fCreateLocalGit

# ========================================
# fCreateRemoteGit Section

fGetMountPath()
{
#$gpMountPath
    fCheckPath $gpPath
}

fGetRawRemotePath()
{
#$gpRawRemotePath
}

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
}

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
}

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
}

# --------------------
fReport()
{
    echo 'Remote origin is now set to:'
    git $cOrigin
    cat <<EOF
Be sure the disk is mounted and $gpPath exists.
Then you can use: "git push origin develop"
If the mount path is changed, use "git proj set -e" to update the origin.
EOF
}

# --------------------
fCreateRemoteGit()
{
    export cOrigin='config --get remote.origin.url'

    fGetMountPath $gpMountPath
#    	fCheckPath $gpPath
    fGetRawRemotePath $gpRawRemotePath

    fCheckForGit
    fCheckSpace $gpPath
    fMkRemote $gpPath
    fReport
} # fCreateRemoteGit
