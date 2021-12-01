#!/bin/bash

# --------------------
export cCurDir cGetOrigin cGetTopDir cTop

export gpAuto gpBin gpCmdName gpDebug gpDoc gpFacility gpLocalRawDir \
    gpLocalTopDir gpLocalStatus gpRemoteMinSpace gpMountDir gpPath \
    gpProjName gpProjStatus gpRemoteRawDir gpRemoteStatus gpSysLog \
    gpVer gpVerbose gResponse

# ========================================

# --------------------
fPushSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}

    # Assumes we in git dir, and one that is managed by gitproj.
    if ! fComMustBeInProjRepo $PWD; then
        fUsage usage
        exit 1
    fi

    gpAuto=${gpAuto:-0}
    gpMountDir=""
    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)
    gpLocalTopDir=$($cGetTopDir 2>/dev/null)
    if [ -z "$gpLocalTopDir" ]; then
        gpLocalTopDir=$PWD
    fi
    gpProjName=$(fComGetConfig -k "gitproj.config.proj-name" -d "Error")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteRawDir=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "TBD")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #2g

    return 0
} # fPushSetGlobals

# --------------------
fPushVerifyState()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir

    fComGetVer

    if [ "$gpLocalStatus" != "defined" ]; then
        fError -m "It looks like 'git proj init' did not finish. Config local-status should be 'defined' " -l $tSrc:$LINENO
    fi

    tTopDir=$($cGetTopDir)
    if [ "$gpLocalTopDir" != "$tTopDir" ]; then
        fError -m "This repo appears to be configured for $gpLocalTopDir, not $tTopDir. Run 'git proj config' to reconfigure for a different host." -l $tSrc:$LINENO
    fi

    if [ "$gpRemoteStatus" = "defined" ]; then
        fLog -p warning -m "It looks like a remote has already been setup with: $gpRemoteRawDir. Currently gitproj can handle only one remote." -l $tSrc:$LINENO
        if [ $gpAuto -ne 0 ]; then
            fError -m "You are in 'automatic' mode, so exiting. To force an 'override' set 'remote-status = not-defined' in $cConfigHost" -l $tSrc:$LINENO
        fi
    fi

    if [ "$gpRemoteStatus" = "not-defined" ] &&
        [ "$gpRemoteRawDir" != "TBD" ] &&
        [ -n "$gpRemoteRawDir" ]; then
        fError -i -m "Config problem remote-status=not-defined, but remote-raw-dir is set to $gpRemoteRawDir. It should be undefined or set to TBD" -l $tSrc:$LINENO
    fi

    return 0
} # fPushVerifyState

# --------------------
fPushRawFiles()
{
    echo TBD
    return 0
} # fPushRawFiles()

# --------------------
fPushGit()
{
    echo TBD
    return 0
} # fPushGit()

# --------------------
fPushToOrigin()
{
    local pPushBranch="$1"

    local tSrc=${BASH_SOURCE##*/}

    # Called by: "git-proj-push [-b]

    fPushVerifyState

    fPushRawFiles

    if [ $pPushBranch -ne 0 ]; then
        fPushGit
    fi
    return 0
} # fPushToOrigin

# ========================================
# Don't forget to call fPushSetGlobals
# Not done here, so some functions can be tested "stand-alone"
