#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCheckInRaw gpCheckWhitespace gpCmdName gpDebug gpDoc gpFacility \
    gpGitFlow gpHookVerbose gpLocalRawDir gpLocalStatus \
    gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled gpProjName \
    gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose

# --------------------
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

    gMainActn[0]="\n  The 'health checks' will look for errors, differences, or just give a configuration status report. If there are validation errors, you will be prompted with repair options.\n\n  The 'actions' are most useful for updating an old project to use newer configuration settings that you have defined at the global level.\n"
    gMainMenu[1]="Quit"
    gMainActn[1]="func exit 0"
    gMainMenu[2]="Help"
    gMainActn[2]="help"

    gMainMenu[3]="Run all health checks, only report problems." #  [error]
    gMainActn[3]="func echo fConfigHealth error"

    gMainMenu[4]="Run all health checks, only report differences" # [diff, warnings]
    gMainActn[4]="func echo fConfigHealth warning"

    gMainMenu[5]="Run all health checks, report status" # [info]
    gMainActn[5]="func echo fConfigHealth info"

    gMainMenu[6]="Select Global User Level Actions"
    gMainActn[6]="menu gGlobalMenu gGlobalActn"

    tNext=7
#    if fComMustBeInProjRepo $PWD >/dev/null 2>&1; then
    if true; then
        gpProjName=${gpProjName:-ProjName}
        # Offered only if in a PROJ
        tNext=9

        gMainMenu[7]="Select Local Project Level Actions"
        gMainActn[7]="menu gLocalMenu gLocalActn"

        gMainMenu[8]="Select pre-commit Hook Actions"
        gMainActn[8]="menu gHookMenu gHookActn"
    fi

    gMainMenu[$tNext]="Select Other Actions"
    gMainActn[$tNext]="menu gOtherMenu gOtherActn"

    # ----------
    gGlobalMenu[0]="Select an action by number: "
    gGlobalActn[0]="\n These options help with adjustments between Product and User configs.\n\n
  o ProductConfig is short for $gpDoc/config/gitconfig\n\n
  o ProductIgnore is short for $gpDoc/config/gitignore\n\n
  o --global is short for ~/.gitconfig\n\n
  o UserIgnore is short for ~/.gitignore\n\n
"
    gGlobalMenu[1]="Back"
    gGlobalActn[1]="back"
    gGlobalMenu[2]="Help"
    gGlobalActn[2]="help"
    gGlobalMenu[3]="Quit"
    gGlobalActn[3]="func exit 0"

    gGlobalMenu[4]="ProductConfig -> --global Only update missing vars"
    gGlobalActn[4]="func echo fProd2User missing"

    gGlobalMenu[5]="ProductConfig -> --global Force update of all gitproj vars"
    gGlobalActn[5]="func echo fProd2User all"

    gGlobalMenu[6]="ProductIgnore -> UserIgnore (only add missing)"
    gGlobalActn[6]="func echo fProd2User ignore"
    # Note this will result in a sorted list, with duplicates removed.
    # cat $gpDoc/config/gitignore ~/.gitignore grep -v '#' | sort -fu

    gGlobalMenu[7]="Install/update git-flow and corresponding configs"
    gGlobalActn[7]="func echo git-flow"

    # ----------
    #

    gLocalMenu[0]="Select an action by number: "
    gLocalActn[0]="\n These options help with adjustments between User and Project configs.\n\n
  o --global is short for ~/.gitconfig\n\n
  o UserIgnore is short for ~/.gitignore\n\n
  o --local is short for $gpProjName/.git/config\n\n
  o ProjectConfig is short for $gpProjName/.gitproj\n\n
  o ProjectIgnore is short for $gpProjName/.gitignore\n\n
"
    gLocalMenu[1]="Back"
    gLocalActn[1]="back"
    gLocalMenu[2]="Help"
    gLocalActn[2]="help"
    gLocalMenu[3]="Quit"
    gLocalActn[3]="func exit 0"

    gLocalMenu[4]="--local -> ProjectConfig (Force core, git-flow, and gitproj.config sections only)"
    gLocalActn[4]="func echo fLocal2Proj"

    gLocalMenu[5]="--global -> ProjectConfig and --local (Update missing vars in gitproj.config sections only)"
    gLocalActn[5]="func echo fGlobal2Proj missing"

    gLocalMenu[6]="--global -> ProjectConfig and --local (Force update of all gitproj.config section, vars in-common) (remotes are not changed)"
    gLocalActn[6]="func echo fGlobal2Proj force all"

    gLocalMenu[7]="UserIgnore -> ProjectIgnore (only adds missing items)"
    # Note this will result in a sorted list, with duplicates removed)
    gLocalActn[7]="func echo fUser2ProjIgnore"

    # ----------
    gHookMenu[0]="Select an action by number: "
    gHookActn[0]="\n These options help with Hook adjustments between Product, User, and Project.\n\n
  o ProductConfig is short for $gpDoc/config/gitconfig\n\n
  o ProductPreCommit is short for $gpDoc/hooks/pre-commit\n\n
  o --global is short for ~/.gitconfig\n\n
  o UserPreCommit is short for ~/.git/hooks/pre-commit\n\n
  o --local is short for $gpProjName/.git/config\n\n
  o ProjectConfig is short for $gpProjName/.gitproj\n\n
  o ProjectPreCommit is short for $gpProjName/.pre-commit\n\n
  o GitPreCommit is short for $gpProjName/.git/hooks/pre-commit\n\n
"
    gHookMenu[1]="Back"
    gHookActn[1]="back"
    gHookMenu[2]="Help"
    gHookActn[2]="help"
    gHookMenu[3]="Quit"
    gHookActn[3]="func exit 0"

    gHookMenu[4]="ProductPreCommit -> UserPreCommit"
    gHookActn[4]="func echo fProd2User replace"

    gHookMenu[5]="--global -> ProjectConfig and --local (Force update of 'gitproj hooks' section)"
    gHookActn[5]="func echo fGlobal2Proj hooks"

    gHookMenu[6]="UserPreCommit -> ProjectPreCommit and GitPreCommit"
    gHookActn[6]="func echo fUser2Proj pre-commit"

    gHookMenu[7]="--local -> ProjectConfig (Force update of 'gitproj.hooks' section)"
    gHookActn[7]="func echo fLocal2Proj hooks force"

    gHookMenu[8]="GitPreCommit -> ProjectPreCommit"
    gHookActn[8]="func echo fGit2Project pre-commit"

    gHookMenu[9]="--local -> --global (Force update of 'gitproj.hooks' section) "
    gHookActn[9]="func echo fLocal2Global force"

    gHookMenu[10]="GitPreCommit -> UserPreCommit"
    gHookActn[10]="func echo fGit2UserHook"

    # ----------
    gOtherMenu[0]="Select an action by number: "
    gOtherActn[0]="\n Other help, TBD\n"
    gOtherMenu[1]="Back"
    gOtherActn[1]="back"
    gOtherMenu[2]="Help"
    gOtherActn[2]="help"
    gOtherMenu[3]="Quit"
    gOtherActn[3]="func exit 0"

    gOtherMenu[4]="Set remote-min-space"
    gOtherActn[4]="func echo fSetMinRemoteSpace"
    # (1) set manually, 2) set from --global, 3) default)"

    gOtherMenu[5]="Set the max size for commits of binary files."
    # (1) set manually, 2) set from --global, 3) default)"
    gOtherActn[5]="func echo fSetMaxSize"

    # ----------
    fComMenu "Main" gMainMenu gMainActn

    return 0
} # fConfigMenu
