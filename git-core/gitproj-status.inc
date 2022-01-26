#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawOrigin gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fStatusGit()
{
    local pOpt="${@}"
    local tSrc=${BASH_SOURCE##*/}
    local tOrigin

    echo
    git status $pOpt

    if fComIsRemoteMounted debug; then
        tOrigin=$($cGetOrigin)
        if [ ! -d $tOrigin ]; then
            fLog -f -p notice -m "Could not find: $tOrigin" -l $tSrc:$LINENO
        fi
    fi

    return 0
} # fStatusGit

# --------------------
fStatusRaw()
{
    local pOpt="$*"
    local tSrc=${BASH_SOURCE##*/}

    if ! fComIsRemoteMounted warning; then
        fLog -p notice -m "Remote is not mounted, so cannot give status of raw/ files." -l $tSrc:$LINENO
        return 0
    fi

    cd $gpLocalTopDir >/dev/null 2>&1
    echo
    echo "REMOTE/ = $gpRemoteRawOrigin/"
    diff -qr $pOpt raw/ $gpRemoteRawOrigin/ | sed "s;$gpRemoteRawOrigin;REMOTE;"
    if [ ${PIPESTATUS[0]} -eq 0 ]; then
        echo "No differences."
    fi
    cd - >/dev/null 2>&1

    return 0
} # fStatusRaw

# --------------------
fStatusOutput()
{
    local pGitOpt="$1"
    local pRawOpt="$2"
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-status
    fStatusGit $pGitOpt
    fStatusRaw $pRawOpt

    return 0
} # fStatusOutput

# ========================================
fComGetProjGlobals
