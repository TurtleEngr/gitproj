#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fStatusGit()
{
    local pShort="$1"
    local tSrc=${BASH_SOURCE##*/}
    local tOrigin

    git status $pShort

    if fStatusIsRemoteMounted warn; then
        tOrigin=$($cGetOrigin)
        if [ -d $tOrigin ]; then
            fLog -p warning -m "Could not find: $tOrigin" -l $tSrc:$LINENO
        fi
    fi

    return 0
} # fStatusGit

# --------------------
fStatusRaw()
{
    local tSrc=${BASH_SOURCE##*/}

    if ! fComIsRemoteMounted warn; then
        fLog -p warning -m "Remote: $gpRemoteRawDir is not mounted, so cannot give status of raw/ files." -l $tSrc:$LINENO
        return 0
    fi

    cd $gpLocalTopDir >/dev/null 2>&1
    echo
    echo "REMOTE/ = $gpRemoteRawDir/"
    diff -qr raw/ $gpRemoteRawDir/ | sed "s;$gpRemoteRawDir;REMOTE;"
    cd - >/dev/null 2>&1

    return 0
} # fStatusRaw

# --------------------
fStatusOutput()
{
    local pShort=$1
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-status
    fStatusGit $pShort
    fStatusRaw

    return 0
} # fStatusOutput

# ========================================
export tSrc=${BASH_SOURCE##*/}
fComPreProjSetGlobals
