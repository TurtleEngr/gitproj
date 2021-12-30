#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fPushRawFiles()
{
    local tSrc=${BASH_SOURCE##*/}
    local tDiff
    local tHelp
    local tOpt="-azC --delete-after"
    local tStatus
    local tVerbose
    declare -l tReply
    
    if [ $gpVerbose -ge 2 ]; then
        tVervose=-viP
    fi

    tDiff=$(diff -rq $gpLocalTopDir/raw $gpRemoteRawDir 2>&1)
    if [ $? -eq 0 ]; then
        tVerbose=$gpVerbose
        fLog -p notice -m "There are no differences found with 'raw' files." -l $tSrc:$LINENO
        gpVerbose=$tVerbose
        return 0
    fi

    tHelp=$(
        cat <<EOF

diff summary:
$tDiff

Select 'Yes' if the above differences look OK."
EOF
)
    echo "$tHelp"

    if [ $gpVerbose -ne 0 ]; then
        cat <<EOF

Dry Run:
'rsync' $tOpt $tVerbose --dry-run
        $gpLocalTopDir/raw/
        $gpRemoteRawDir"
EOF
        'rsync' $tOpt $tVerbose --dry-run -v $gpLocalTopDir/raw/ $gpRemoteRawDir
    fi

    echo
    gResponse=$gpYesNo
    if [ -z "$gpYesNo" ]; then
        fComSelect "Continue? " "Yes-push-these-files No-skip-pushing" "$tHelp"
    fi
    case $gResponse in
        QUIT)
            exit 1
            ;;
        No*)
            fLog -p warning -m "Nothing was pushed." -l $tSrc:$LINENO
            return 1
            ;;
        Yes*)
            :
            ;;
        *)
            fError -i -m "Expected: Yes or No" -l $tSrc:$LINENO
            ;;
    esac

    'rsync' $tOpt $tVerbose $gpLocalTopDir/raw/ $gpRemoteRawDir
    tStatus=$?

    return $tStatus
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

    tBranch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)
    if [ $? -ne 0 ]; then
        fError -i -m "Problem getting current branch name." -l $tSrc:$LINENO
    fi

    fComGit push origin $tBranch
    tStatus=$?

    return 0
} # fPushGit

# --------------------
fPushToOrigin()
{
    local pBranchFlag="${1:-0}"
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-push [-b]

    fComGetProjGlobals
    fComIsRemoteMounted
    fPushRawFiles
    fPushGit $pBranchFlag

    return 0
} # fPushToOrigin

# ========================================
# Don't forget to call fComGetProjGlobals
# Not done here, so some functions can be tested "stand-alone"
