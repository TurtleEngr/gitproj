#!/bin/bash

. ../git-core/gitproj-com.inc

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

    gMainActn[0]="The 'health checks' will look for errors,
differences, or just give congiuration status report. If there are
validation errors, you will be prompted with repair options.\n\n The
'actions' are most useful for updating an old project to use newer
configuration settings that you have defined at the global level."

    gMainMenu[1]="Quit"
    gMainActn[1]="func exit"
    gMainMenu[2]="Help"
    gMainActn[2]="help"

    gMainMenu[3]="Run all health checks, only report problems." #  [error]
    gMainActn[3]="func echo fComStub arg1"

    gMainMenu[4]="Run all health checks, only report differences" # [diff, warnings]
    gMainActn[4]="func echo fComStub arg1 arg2"

    gMainMenu[5]="Run all health checks, report status" # [info]
    gMainActn[5]="func echo fComStub"

    gMainMenu[6]="Select Global Actions"
    gMainActn[6]="menu gGlobalMenu gGlobalActn"

    tNext=7
#    if fComMustBeInProjRepo $PWD >/dev/null 2>&1; then
    if true; then
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
    gGlobalActn[4]="func echo fComStub update missing"

    gGlobalMenu[5]="$gpDoc/config/gitconfig -> ~/.gitconfig Force update of all gitproj vars"
    gGlobalActn[5]="func echo fComStub update ALL"

    gGlobalMenu[6]="$gpDoc/hooks/pre-commit -> ~/.pre-commit"
    gGlobalActn[6]="func echo fComStub replace pre-commig"

    gGlobalMenu[7]="$gpDoc/config/gitignore -> ~/.gitignore (only add missing)"
    gGlobalActn[7]="func echo fComStub update ignore"
    # Note this will result in a sorted list, with duplicates removed.
    # cat $gpDoc/config/gitignore ~/.gitignore grep -v '#' | sort -fu

    gGlobalMenu[8]="Install/update git-flow and corresponding configs"
    gGlobalActn[8]="func echo fComStub git-flow"

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
    gLocalActn[5]="func echo fComStub arg1"

    gLocalMenu[6]="~/gitconfig -> $gpProjName/.gitproj and --local (Update missing vars in gitproj.config sections only)"
    gLocalActn[6]="func echo fComStub arg1"

    gLocalMenu[7]="~/.gitconfig ->  $gpProjName/.gitproj and --local (Force update of all gitproj.config section, vars in-common) (remotes are not changed)"
    gLocalActn[7]="func echo fComStub arg1"

    gLocalMenu[8]="~/.gitignore -> $gpProjName/.gitignore (only adds missing items)"
    # Note this will result in a sorted list, with duplicates removed)
    gLocalActn[8]="func echo fComStub arg1"

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
    gHookActn[4]="func echo fComStub arg1"

    gHookMenu[5]="~/.pre-commit -> $gpProjName/.pre-commit and $gpProjName/.git/hooks/pre-commit"
    gHookActn[5]="func echo fComStub arg1"

    gHookMenu[6]="--local -> $gpProjName/.gitproj (Force update of 'gitproj.hooks' section)"
    gHookActn[6]="func echo fComStub arg1"

    gHookMenu[7]="$gpProjName/.git/hooks/pre-commit -> $gpProjName/.pre-commit"
    gHookActn[7]="func echo fComStub arg1"

    gHookMenu[8]="--local -> ~/.gitconfig (Force update of 'gitproj.hooks' section) "
    gHookActn[8]="func echo fComStub arg1"

    gHookMenu[9]="$gpProjName/.git/hooks/pre-commit -> ~/.pre-commit from "
    gHookActn[9]="func echo fComStub arg1"

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
    gOtherActn[4]="func echo fComStub arg1"
    # (1) set manually, 2) set from ~/.gitconfig, 3) default)"

    gOtherMenu[5]="Set the max size for commits of binary files."
    # (1) set manually, 2) set from ~/.gitconfig, 3) default)"
    gOtherActn[5]="func echo fComStub arg1"

    # ----------
    fComMenu "Main" gMainMenu gMainActn

    return 0
} # fConfigMenu

fConfigExecAction()
{
    echo "In fConfigExecAction: $*"

#  Example 1:
#    Update configs or files;
#    Copy all config:
#    core, alias, git-flow, gitproj.config;
#    Product -> User;
#    Only copy missing

#  Example 2:
#    Update configs or files;
#    Copy gitignore;
#    User -> Project

    exit 0
} # fConfigExecAction

fConfigMenu2()
{
    declare -ag gTypeMenu
    declare -ag gTypeActn
    declare -ag gConfMenu
    declare -ag gConfActn
    declare -ag gIgnoreMenu
    declare -ag gIgnoreActn
    declare -ag gPreCommitMenu
    declare -ag gPreCommitActn
    declare -ag gForceMenu
    declare -ag gForceActn
    declare -ag gYesNoMenu
    declare -ag gYesNoActn
    declare -g gMenuTitle="Update configs or files"

    gTypeMenu[0]="Select the configs or file to be moved:"
    gTypeActn[0]="Help for type..."
    gTypeMenu[1]="Quit"
    gTypeActn[1]="func exit 1"
    gTypeMenu[2]="Help"
    gTypeActn[2]="help"
    gTypeMenu[3]="Copy all config: core, alias, git-flow, gitproj.config"
    gTypeActn[3]="menu gConfMenu gConfActn"
    gTypeMenu[4]="Copy some config: git-flow, gitproj.config"
    gTypeActn[4]="menu gConfMenu gConfActn"
    gTypeMenu[5]="Only copy: gitproj.config"
    gTypeActn[5]="menu gConfMenu gConfActn"
    gTypeMenu[6]="Only copy: gitproj.hooks"
    gTypeActn[6]="menu gConfMenu gConfActn"
    gTypeMenu[7]="Copy gitignore"
    gTypeActn[7]="menu gIgnoreMenu gIgnoreActn"
    gTypeMenu[8]="Copy pre-commit"
    gTypeActn[8]="menu gPreCommitMenu gPreCommitActn"

    # 3, 4, 5, 6
    gConfMenu[0]="Select the from/to:"
    gConfActn[0]="Help for config..."
    gConfMenu[1]="Back"
    gConfActn[1]="func return"
    gConfMenu[2]="Help"
    gConfActn[2]="help"
    gConfMenu[3]="Quit"
    gConfActn[3]="func exit 1"
    gConfMenu[4]="Product -> User"
    gConfActn[4]="menu gForceMenu gForceActn"
    gConfMenu[5]="User -> Project"
    gConfActn[5]="menu gForceMenu gForceActn"
    gConfMenu[6]="Project -> Local"
    gConfActn[6]="menu gForceMenu gForceActn"
    gConfMenu[7]="Local -> Project"
    gConfActn[7]="menu gForceMenu gForceActn"
    gConfMenu[8]="Project -> User"
    gConfActn[8]="menu gForceMenu gForceActn"

    # 7
    gIgnoreMenu[0]="Select the from -> to direction:"
    gIgnoreActn[0]="Help for ignore direction..."
    gIgnoreMenu[1]="Back"
    gIgnoreActn[1]="func return"
    gIgnoreMenu[2]="Help"
    gIgnoreActn[2]="help"
    gIgnoreMenu[3]="Quit"
    gIgnoreActn[3]="func exit 1"
    gIgnoreMenu[4]="Product -> User"
    gIgnoreActn[4]="menu gYesNoMenu gYesNoActn"
    gIgnoreMenu[5]="User -> Project"
    gIgnoreActn[5]="menu gYesNoMenu gYesNoActn"
    gIgnoreMenu[6]="Project -> User"
    gIgnoreActn[6]="menu gYesNoMenu gYesNoActn"

    # 8
    gPreCommitMenu[0]="Select the from -> to direction:"
    gPreCommitActn[0]="Help for ignore direction..."
    gPreCommitMenu[1]="Back"
    gPreCommitActn[1]="func return"
    gPreCommitMenu[2]="Help"
    gPreCommitActn[2]="help"
    gPreCommitMenu[3]="Quit"
    gPreCommitActn[3]="func exit 1"
    gPreCommitMenu[4]="Product -> User"
    gPreCommitActn[4]="menu gYesNoMenu gYesNoActn"
    gPreCommitMenu[5]="User -> Project"
    gPreCommitActn[5]="menu gYesNoMenu gYesNoActn"
    gPreCommitMenu[6]="Project -> Local"
    gPreCommitActn[6]="menu gYesNoMenu gYesNoActn"
    gPreCommitMenu[7]="Local -> Project"
    gPreCommitActn[7]="menu gYesNoMenu gYesNoActn"
    gPreCommitMenu[8]="Project -> User"
    gPreCommitActn[8]="menu gYesNoMenu gYesNoActn"

    # 3, 4, 5, 6
    gForceMenu[0]="Select the "force" option:"
    gForceActn[0]="Help for force..."
    gForceMenu[1]="Back"
    gForceActn[1]="func return"
    gForceMenu[2]="Help"
    gForceActn[2]="help"
    gForceMenu[3]="Quit"
    gForceActn[3]="func exit 1"
    gForceMenu[4]="Only copy missing"
    gForceActn[4]="menu gYesNoMenu gYesNoActn"
    gForceMenu[5]="Force copy"
    gForceActn[5]="menu gYesNoMenu gYesNoActn"

    gYesNoMenu[0]="Is the above action correct?"
    gYesNoActn[0]="Help for yes/no..."
    gYesNoMenu[1]="Back if no"
    gYesNoActn[1]="func return"
    gYesNoMenu[2]="Help"
    gYesNoActn[2]="help"
    gYesNoMenu[3]="Quit"
    gYesNoActn[3]="func exit 1"
    gYesNoMenu[4]="Yes, continue"
    gYesNoActn[4]="func fConfigExecAction \$gMenuTitle"

    (fComMenu "$gMenuTitle" gTypeMenu gTypeActn)
    return $?
} # fConfigMenu2

export gpProjName=george
export gpDoc=/usr/share/doc/git-proj/
# fConfigMenu
fConfigMenu2
echo $?
