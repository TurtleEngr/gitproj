#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fPullRawFiles()
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

    tDiff=$(diff -rq $gpRemoteRawDir $gpLocalTopDir/raw 2>&1)
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
        $gpRemoteRawDir/"
        $gpLocalTopDir/raw
EOF
        'rsync' $tOpt $tVerbose --dry-run -v $gpRemoteRawDir/ $gpLocalTopDir/raw
    fi

    echo
    gResponse=$gpYesNo
    if [ -z "$gpYesNo" ]; then
        fComSelect "Continue? " "Yes-pull-these-files No-skip-pulling" "$tHelp"
    fi
    case $gResponse in
        QUIT)
            exit 1
            ;;
        No*)
            fLog -p warning -m "Nothing was pulled." -l $tSrc:$LINENO
            return 1
            ;;
        Yes*)
            :
            ;;
        *)
            fError -i -m "Expected: Yes or No" -l $tSrc:$LINENO
            ;;
    esac

    'rsync' $tOpt $tVerbose $gpRemoteRawDir/ $gpLocalTopDir/raw
    tStatus=$?

    return $tStatus
} # fPullRawFiles

# --------------------
fPullGit()
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

    fComGit pull origin $tBranch
    tStatus=$?
    
    return $tStatus
} # fPullGit

# --------------------
fPullFromOrigin()
{
    local pBranchFlag="${1:-0}"
    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-pull [-b]

    fComGetProjGlobals
    fComIsRemoteMounted
    fPullRawFiles
    fPullGit $pBranchFlag

    return 0
} # fPullFromOrigin

# ========================================
# Don't forget to call fComGetProjGlobals
# Not done here, so some functions can be tested "stand-alone"
