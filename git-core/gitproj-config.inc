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
    local tNext
    declare -ag gMainMenu
    declare -ag gMainActn
    declare -ag gGlobalMenu
    declare -ag gGlobalActn
    declare -ag gLocalMenu
    declare -ag gLocalActn
    declare -ag gHookMenu
    declare -ag gHookActn

    # --------------------
    # Setup Menu Structure

    # ----------
    gMainMenu[0]="Select by number: "

    gMainActn[0]="The "health checks" will look for errors,
differences, or just give congiuration status report. If there are
validation errors, you will be prompted with repair options.\n\n The
"actions" are most useful for updating an old project to use newer
configuration settings that you have defined at the global level."

    gMainMenu[1]="Quit"
    gMainActn[1]="func exit"
    gMainMenu[2]="Help"
    gMainActn[2]="help"

    gMainMenu[3]="Run all health checks, only report problems." #  [error]
    gMainActn[3]="func fComStub arg1"

    gMainMenu[4]="Run all health checks, only report differences" # [diff, warnings]
    gMainActn[4]="func fComStub arg1 arg2"

    gMainMenu[5]="Run all health checks, report status" # [info]
    gMainActn[5]="func fComStub"

    gMainMenu[6]="Select Global Actions"
    gMainActn[6]="menu gGlobalMenu gGlobalActn"

    tNext=7
    if fComMustBeInProjRepo $PWD >/dev/null 2>&1; then
        # Offered only if in a PROJ
        gMainMenu[7]="Select Local Actions"
        gMainActn[7]="menu gLocalMenu gLocalActn"
        tNext=8
    fi

    gMainMenu[$tNext]="Select Other Actions"
    gMainActn[$tNext]="menu gOtherMenu gOtherActn"

    # ----------
    gGlobalMenu[0]="Select an action by number: "
    gGlobalActn[0]="Global help, TBD"
    gGlobalMenu[1]="Back"
    gGlobalActn[1]="back"
    gGlobalMenu[2]="Help"
    gGlobalActn[2]="help"
    gGlobalMenu[3]="Quit"
    gGlobalActn[3]="func exit"

    gGlobalMenu[4]="$gpDoc/config/gitconfig -> ~/.gitconfig Only update missing vars"
    gGlobalActn[4]="func fComStub update missing"

    gGlobalMenu[5]="$gpDoc/config/gitconfig -> ~/.gitconfig Force update of all gitproj vars"
    gGlobalActn[5]="func fComStub update ALL"

    gGlobalMenu[6]="$gpDoc/hooks/pre-commit -> ~/.pre-commit"
    gGlobalActn[6]="func fComStub replace pre-commig"

    gGlobalMenu[7]="$gpDoc/config/gitignore -> ~/.gitignore (only add missing)"
    gGlobalActn[7]="func fComStub update ignore"
    # Note this will result in a sorted list, with duplicates removed.
    # cat $gpDoc/config/gitignore ~/.gitignore grep -v '#' | sort -fu

    gGlobalMenu[8]="Install/update git-flow and corresponding configs"
    gGlobalActn[8]="func fComStub git-flow"

    # ----------
    gLocalMenu[0]="Select an action by number: "
    gLocalActn[0]="Local help, TBD"
    gLocalMenu[1]="Back"
    gLocalActn[1]="back"
    gLocalMenu[2]="Help"
    gLocalActn[2]="help"
    gLocalMenu[3]="Quit"
    gLocalActn[3]="func exit"

    gLocalMenu[4]="Select pre-commiit Hook Actions"
    gLocalActn[4]="menu gHookMenu gHookActn"

    gLocalMenu[5]="--local -> $gpProjName/.gitproj (Force core, git-flow, and gitproj.config sections only)"
    gLocalActn[5]="func fComStub arg1"

    gLocalMenu[6]="~/gitconfig -> $gpProjName/.gitproj and --local (Update missing vars in gitproj.config sections only)"
    gLocalActn[6]="func fComStub arg1"

    gLocalMenu[7]="~/.gitconfig ->  $gpProjName/.gitproj and --local (Force update of all gitproj.config section, vars in-common) (remotes are not changed)"
    gLocalActn[7]="func fComStub arg1"

    gLocalMenu[8]="~/.gitignore -> $gpProjName/.gitignore (only adds missing items)"
    # Note this will result in a sorted list, with duplicates removed)
    gLocalActn[8]="func fComStub arg1"

    # ----------
    gHookMenu[0]="Select an action by number: "
    gHookActn[0]="pre-commit hook help, TBD"
    gHookMenu[1]="Back"
    gHookActn[1]="back"
    gHookMenu[2]="Help"
    gHookActn[2]="help"
    gHookMenu[3]="Quit"
    gHookActn[3]="func exit"

    gHookMenu[4]="~/.gitconfig -> $gpProjName/.gitproj and --local (Force update of 'gitproj hooks' section)"
    gHookActn[4]="func fComStub arg1"

    gHookMenu[5]="~/.pre-commit -> $gpProjName/.pre-commit and $gpProjName/.git/hooks/pre-commit"
    gHookActn[5]="func fComStub arg1"

    gHookMenu[6]="--local -> $gpProjName/.gitproj (Force update of 'gitproj.hooks' section)"
    gHookActn[6]="func fComStub arg1"

    gHookMenu[7]="$gpProjName/.git/hooks/pre-commit -> $gpProjName/.pre-commit"
    gHookActn[7]="func fComStub arg1"

    gHookMenu[8]="--local -> ~/.gitconfig (Force update of 'gitproj.hooks' section) "
    gHookActn[8]="func fComStub arg1"

    gHookMenu[9]="$gpProjName/.git/hooks/pre-commit -> ~/.pre-commit from "
    gHookActn[9]="func fComStub arg1"

    # ----------
    gOtherMenu[0]="Select an action by number: "
    gOtherActn[0]="Local help, TBD"
    gOtherMenu[1]="Back"
    gOtherActn[1]="back"
    gOtherMenu[2]="Help"
    gOtherActn[2]="help"
    gOtherMenu[3]="Quit"
    gOtherActn[3]="func exit"

    gOtherMenu[4]="Set remote-min-space"
    gOtherActn[4]="func fComStub arg1"
    # (1) set manually, 2) set from ~/.gitconfig, 3) default)"

    gOtherMenu[5]="Set the max size for commits of binary files."
    # (1) set manually, 2) set from ~/.gitconfig, 3) default)"
    gOtherActn[5]="func fComStub arg1"

    # ----------
    fComMenu "Main" gMainMenu gMainActn

    return 0
} # fConfigMenu
