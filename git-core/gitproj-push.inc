#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawOrigin gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fPushRawFiles()
{
    local pDeleteRaw=${1:-0}
    local tSrc=${BASH_SOURCE##*/}
    local tDiff
    local tOpt="-azC"
    local tDel=""
    local tVerbose

    if [ $gpVerbose -ge 3 ]; then
        tVerbose=-iP
    fi
    if [ ${pDeleteRaw:-0} -ne 0 ]; then
        tDel="--delete --delete-after"
    fi

    echo
    echo '----------------------------------------'
    echo 'raw/ push'
    tDiff=$(diff -rq $gpLocalTopDir/raw $gpRemoteRawOrigin 2>&1)
    if [ $? -eq 0 ]; then
        fLog -f -p notice -m "There are no differences found with 'raw' files." -l $tSrc:$LINENO
        return 0
    fi
    echo "diff summary:"
    echo -E "$tDiff"

    if [ $gpVerbose -ne 0 ]; then
        cat <<EOF

Dry Run:
'rsync' --dry-run $tOpt $tDel -v $tVerbose
        $gpLocalTopDir/raw/
        $gpRemoteRawOrigin
EOF
        'rsync' --dry-run $tOpt $tDel -v $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawOrigin
    fi

    echo
    if ! fComYesNo "Are the above differences OK"; then
        echo
        fLog -p warning -m "Nothing was pushed." -l $tSrc:$LINENO
        return 1
    fi
    echo

    'rsync' $tOpt $tDel -v $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawOrigin
    return $?
} # fPushRawFiles

# --------------------
fPushGit()
{
    local pBranchFlag="${1:-0}"
    local tSrc=${BASH_SOURCE##*/}
    local tBranch

    if [ $pBranchFlag -eq 0 ]; then
        return 0
    fi

    echo
    echo '----------------------------------------'
    echo 'git push'
    tBranch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)
    if [ $? -ne 0 ]; then
        fError -i -m "Problem getting current branch name." -l $tSrc:$LINENO
    fi

    fComGit push origin $tBranch
    return $?
} # fPushGit

# --------------------
fPushToOrigin()
{
    local pBranchFlag="$1"
    local pDeleteRaw="$2"
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-push

    fComGetProjGlobals
    if ! fComIsRemoteMounted warning; then
        return 1
    fi

    fPushRawFiles $pDeleteRaw
    fPushGit $pBranchFlag

    return 0
} # fPushToOrigin

# ========================================
# Don't forget to call fComGetProjGlobals
# Not done here, so some functions can be tested "stand-alone"
