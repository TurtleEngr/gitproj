#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCheckInRaw gpCheckWhitespace gpCmdName gpDebug gpDoc gpFacility \
    gpGitFlow gpHookVerbose gpLocalRawDir gpLocalStatus \
    gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled gpProjName \
    gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose


# --------------------
fCheckForErrors()
{
    local tSrc=${BASH_SOURCE##*/}
    echo "TBD fCheckForErrors()"
} # fCheckForErrors()

fCheckForWarnings()
{
    local tSrc=${BASH_SOURCE##*/}
    echo "TBD fCheckForWarnings()"
} # fCheckForWarnings

fCheckStatus()
{
    local tSrc=${BASH_SOURCE##*/}
    echo "TBD fCheckStatus()"
} # fCheckStatus

# --------------------
fConfigHealth()
{
    local pStatus=$1
    local tSrc=${BASH_SOURCE##*/}

    case $pStatus in
        error)
	    fCheckForErrors
	    ;;
	warning)
	    fCheckForErrors
	    fCheckForWarnings
	    ;;
	info)
	    fCheckForErrors
	    fCheckForWarnings
	    fCheckStatus
	    ;;
	*)
	    fError -i -n -m "Invalid option: $pStatus" -l $tSrc$LINENO
	    ;;
    esac

    return 0
} # fConfigHealth

fConfigMergeIgnore()
{
    local pFrom=$1
    local pTo=$2
    local tSrc=${BASH_SOURCE##*/}

    if [ ! -f $pFrom ]; then
        fError -i -n -m "" -l $tSrc:$LINENO
    fi
    $gpDoc/config/gitignore ~/.gitignore
    if [ ! -f $tTo ]; then
        cp -v $pFrom $tTo
	return 0
    fi

    cp -v --backup=t  $tTo $tTo.bak
    cat $pFrom $pTo | grep -v '^#' | sort -u >t.tmp
    mv t.tmp $pTo

    return 0
} # fConfigMergeIgnore

fConfigProduct2User()
{
    local pStatus=$1
    local tSrc=${BASH_SOURCE##*/}

    case $pStatus in
        missing)
	    echo TBD fConfigProduct2User missing
	    ;;
        missing-hook)
	    echo TBD fConfigProduct2User missing-hook
	    ;;
 	force)
	    echo TBD fConfigProduct2User force
	    ;;
 	force-hook)
	    echo TBD fConfigProduct2User force-hook
	    ;;
 	ignore)
	    fConfigMergeIgnore $gpDoc/config/gitignore ~/.gitignore
	    ;;
 	pre-commit)
	    cp -v --backup=t $gpDoc/hooks/pre-commit ~/.pre-commit
	    ;;
	*)
	    fError -i -n -m "Invalid option: $pStatus" -l $tSrc$LINENO
	    ;;
    esac

    return 0
} # fConfigProduct2User

fConfigUser2Project()
{
    local pStatus="$1"
    local pDir="$2"
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir="$($cGetTopDir 2>&1)"

    case "${pDir:-error}" in
        up)   : ;;
	down) : ;;
	*)
	    fError -i -n -m "Invalid option: pDir=$pDir" -l $tSrc$LINENO
	    ;;
    esac

    case $pStatus in
        missing)
	    echo TBD fConfigUser2Project missing $pDir
	    ;;
        missing-hook)
	    echo TBD fConfigUser2Project missing-hook $pDir
	    ;;
 	force)
	    echo TBD fConfigUser2Project force $pDir
	    ;;
 	force-hook)
	    echo TBD fConfigUser2Project force-hook $pDir
	    ;;
 	ignore)
	    if [ "$pDir" = "down" ]; then
	        fConfigMergeIgnore ~/.gitignore $tTopDir/.gitignore
		return 0
	    fi
	    fConfigMergeIgnore $tTopDir/.gitignore ~/.gitignore
	    ;;
 	pre-commit)
	    if [ "$pDir" = "down" ]; then
	        cp -v --backup=t ~/.pre-commit $tTopDir/.pre-commit
		return 0
	    fi
	    cp -v --backup=t $tTopDir/.pre-commit ~/.pre-commit
	    ;;
	*)
	    fError -i -n -m "Invalid option: pStatus=$pStatus" -l $tSrc$LINENO
	    ;;
    esac

    return 0
} # fConfigUser2Project

fConfigProject2Local()
{
    local pStatus="$1"
    local pDir="$2"
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir="$($cGetTopDir 2>&1)"

    case "${pDir:-error}" in
        up)   : ;;
	down) : ;;
	*)
	    fError -i -n -m "Invalid option: pDir=$pDir" -l $tSrc$LINENO
	    ;;
    esac

    case $pStatus in
        missing)
	    echo TBD fConfigProject2Local missing $pDir
	    ;;
        missing-hook)
	    echo TBD fConfigProject2Local missing-hook $pDir
	    ;;
 	force)
	    echo TBD fConfigProject2Local force $pDir
	    ;;
 	force-hook)
	    echo TBD fConfigProject2Local force-hook $pDir
	    ;;
 	pre-commit)
	    if [ "$pDir" = "down" ]; then
	        cp -v --backup=t $tTopDir/.pre-commit $tTopDir/.git/hooks/pre-commit
	        return 0
	    fi
	    cp -v --backup=t $tTopDir/.git/hooks/pre-commit $tTopDir/.pre-commit
	    ;;
	*)
	    fError -i -n -m "Invalid option: $pStatus" -l $tSrc$LINENO
	    ;;
    esac

    return 0
} # fConfigProject2Local

# --------------------
fConfigMenu()
{
    local tNext
    local tSrc=${BASH_SOURCE##*/}
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

    gMainMenu[3]="Run health checks that look for problems." #  [error]
    gMainActn[3]="func fConfigHealth error"

    gMainMenu[4]="Run health checks that look for differences." # [diff, warnings]
    gMainActn[4]="func fConfigHealth warning"

    gMainMenu[5]="Run all health checks and report their status" # [info]
    gMainActn[5]="func fConfigHealth info"

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
    gGlobalActn[4]="func fConfigProduct2User missing"

    gGlobalMenu[5]="ProductConfig -> --global Force update of all gitproj vars"
    gGlobalActn[5]="func fConfigProduct2User force"

    gGlobalMenu[6]="ProductIgnore -> UserIgnore (only add missing)"
    gGlobalActn[6]="func fConfigProduct2User ignore"
    # Note this will result in a sorted list, with duplicates removed.
    # cat $gpDoc/config/gitignore ~/.gitignore grep -v '#' | sort -fu

    gGlobalMenu[7]="Install/update git-flow and corresponding configs"
    gGlobalActn[7]="func fComMkGitFlow"

    # ----------
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
  o UserPreCommit is short for ~/.pre-commit\n\n
  o --local is short for $gpProjName/.git/config\n\n
  o ProjectConfig is short for $gpProjName/.gitproj\n\n
  o ProjectPreCommit is short for $gpProjName/.pre-commit\n\n
  o LocalPreCommit is short for $gpProjName/.git/hooks/pre-commit\n\n
"
    gHookMenu[1]="Back"
    gHookActn[1]="back"
    gHookMenu[2]="Help"
    gHookActn[2]="help"
    gHookMenu[3]="Quit"
    gHookActn[3]="func exit 0"

    gHookMenu[4]="ProductPreCommit -> UserPreCommit"
    gHookActn[4]="func fConfigProduct2User pre-commit"

    gHookMenu[5]="--global -> ProjectConfig and --local (Force update of 'gitproj hooks' section)"
    gHookActn[5]="func echo fGlobal2Proj hooks"

    gHookMenu[6]="UserPreCommit -> ProjectPreCommit and LocalPreCommit"
    gHookActn[6]="func echo fUser2Proj pre-commit"

    gHookMenu[7]="--local -> ProjectConfig (Force update of 'gitproj.hooks' section)"
    gHookActn[7]="func echo fLocal2Proj hooks force"

    gHookMenu[8]="LocalPreCommit -> ProjectPreCommit"
    gHookActn[8]="func echo fGit2Project pre-commit"

    gHookMenu[9]="--local -> --global (Force update of 'gitproj.hooks' section) "
    gHookActn[9]="func echo fLocal2Global force"

    gHookMenu[10]="LocalPreCommit -> UserPreCommit"
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

# ========================================
fComPreProjSetGlobals
