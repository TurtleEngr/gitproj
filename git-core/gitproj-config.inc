#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCheckInRaw gpCheckWhitespace gpCmdName gpDebug gpDoc gpFacility \
    gpGitFlow gpHookVerbose gpLocalRawDir gpLocalStatus \
    gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled gpProjName \
    gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose

fConfigMenu()
{
    local tPrompt
    local tHelp
    local tMenu

    tPrompt="What would you like to do? "
    tHelp=$(cat <<EOF

The "health checks" will look for errors, differences, or just give
congiuration status report. If there are validation errors, you will
be prompted with repair options.

The "actions" are most useful for updating an old project to use newer
configuration settings that you have defined at the global level.

EOF
    )
    tList=""
} # fConfigMenu
