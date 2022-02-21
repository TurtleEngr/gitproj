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

    # Product

    # User

    # Project

    # Local

    return 0
} # fCheckForErrors()

fCheckForWarnings()
{
    local tSrc=${BASH_SOURCE##*/}
    echo "TBD fCheckForWarnings()"

    # Product

    # User

    # Project

    # Local

    return 0
} # fCheckForWarnings

fCheckStatus()
{
    local tSrc=${BASH_SOURCE##*/}
    echo "TBD fCheckStatus()"

    # Product

    # User

    # Project

    # Local

    return 0
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
            fError -i -m "Invalid option: $pStatus" -l $tSrc$LINENO -e 20
            ;;
    esac

    return 0
} # fConfigHealth

fConfigUpdate()
{
    local pInc
    local pFiles
    local pForce
    local tSrc=${BASH_SOURCE##*/}
    local tFrom
    local tTo
    local tForce
    local tTopDir=$($cGetTopDir)

    if [ "$pForce" = "force" ]; then
        tForce='-f'
    fi

    case $pFiles in
        ProdUser)
	    tFrom=/usr/share/doc/git-proj/config/gitconfig
	    tTo=~/.gitconfig
	    ;;
        UserProj)
	    tFrom=~/.gitconfig
	    tTo=$tTopDir/.gitproj
	    ;;
    	ProjLocal)
	    tFrom=$tTopDir/.gitproj
	    tTo=$tTopDir/.git/config
	    ;;
    	LocalProj)
	    tFrom=$tTopDir/.git/config
	    tTo=$tTopDir/.gitproj
	    ;;
    	ProjUser)
	    tFrom=$tTopDir/.gitproj
	    tTo=~/.gitconfig
	    ;;
	*)
	    fError -i -m "Unknown option: $pFiles" -l $tSrc:$LINENO -e 20
	    ;;
    esac

    fComConfigCopy $tForce -s $tFrom -d $tTo -i "$pInc"
    return $?
} # fConfigUpdate

fConfigMergeIgnore()
{
    local pFrom=$1
    local pTo=$2
    local tSrc=${BASH_SOURCE##*/}

    if [ ! -f $pFrom ]; then
        fError -i -m "Missing: $pFrom" -l $tSrc:$LINENO -e 20
    fi
    if [ ! -f $tTo ]; then
        cp -v $pFrom $tTo
        return $?
    fi

    cp -v --backup=t  $tTo $tTo.bak
    cat $pFrom $pTo | grep -v '^#' | sort -u >t.tmp
    mv -f t.tmp $pTo

    return 0
} # fConfigMergeIgnore

fConfigIgnore()
{
    local pFiles=$1
    local tSrc=${BASH_SOURCE##*/}
    local tFrom
    local tTo
    local tTopDir=$($cGetTopDir)
    
    case $pFiles in
        ProdUser)
	    tFrom=/usr/share/doc/git-proj/config/gitignore
	    tTo=~/.gitignore
	    ;;
        UserProj)
	    tFrom=~/.gitignore
	    tTo=$tTopDir/.gitignore
	    ;;
    	ProjUser)
	    tFrom=$tTopDir/.gitignore
	    tTo=~/.gitignore
	    ;;
	*)
	    fError -i -m "Unknown option: $pFiles" -l $tSrc:$LINENO -e 20
	    ;;
    esac

    fConfigMergeIgnore $tFrom $tTo
    return $?
} # fConfigIgnore

fConfigCopyPreCommit()
{
    local pFrom
    local pTo
    local tSrc=${BASH_SOURCE##*/}

    cp --backup=t $pFrom $pTo
    return $?
} # fConfigCopyPreCommit

fConfigPreCommit()
{
    local pFiles=$1
    local tSrc=${BASH_SOURCE##*/}
    local tFrom
    local tTo
    local tTopDir=$($cGetTopDir)
    
    case $pFiles in
        ProdUser)
	    tFrom=/usr/share/doc/git-proj/hooks/pre-commit
	    tTo=~/.pre-commit
	    ;;
        UserProj)
	    tFrom=~/.pre-commit
	    tTo=$tTopDir/.pre-commit
	    ;;
    	ProjLocal)
	    tFrom=$tTopDir/.pre-commit
	    tTo=$tTopDir/.git/hooks/pre-commit
	    ;;
    	LocalProj)
	    tFrom=$tTopDir/.git/hooks/pre-commit
	    tTo=$tTopDir/.git/hooks/pre-commit
	    ;;
    	ProjUser)
	    tFrom=$tTopDir/.git/hooks/pre-commit
	    tTo=~/.pre-commit
	    ;;
	*)
	    fError -i -m "Unknown option: $pFiles" -l $tSrc:$LINENO -e 20
	    ;;
    esac

    fConfigCopyPreCommit $tFrom $tTo
    return $?
} # fConfigPreCommit

fConfigExecAction()
{
    local pArgStr="$*"
    local tSrc=${BASH_SOURCE##*/}
    local tCode
    declare -a tArgList
    local tArg
    local i

    # Extract the codes from pArgStr and assign them to tArgList
    tCode=$(echo "} $pArgStr {" | sed 's/}[^{]*{/ /g')
    if [ $gpDebug -ne 0 ]; then
        echo "In fConfigExecAction: $pArgStr"
        echo "tCode=$tCode"
    fi

    i=0
    for tArg in $tCode; do
        let ++i
        tArgList[$i]=$tArg
    done

    # Select on the type
    case ${tArgList[1]} in
        copy-all)
	    fConfigUpdate 'core\.|alias\.|git-flow\.|gitproj\.config\.' ${tArgList[2]} ${tArgList[3]}
	    ;;
	copy-some)
	    fConfigUpdate 'git-flow\.|gitproj\.config\.' ${tArgList[2]} ${tArgList[3]}
	    ;;
	copy-gitproj)
	    fConfigUpdate 'gitproj\.config\.' ${tArgList[2]} ${tArgList[3]}
	    ;;
	copy-hooks)
	    fConfigUpdate 'gitproj\.hooks\.' ${tArgList[2]} ${tArgList[3]}
	    ;;
	gitignore)
	    fConfigIgnore ${tArgList[2]}
	    ;;
	pre-commit)
	    fConfigPreCommit ${tArgList[2]}
	    ;;
	*)
	    fError -i -m "${tArgList[1]} is unknown type" -l $tSrc:$LINENO -e 20
	    ;;
    esac

    # s/b in a subshell, so this will return to main main
    exit 0
} # fConfigExecAction

# --------------------
fConfigMenu()
{
    local tSrc=${BASH_SOURCE##*/}
    local tInProj
    declare -ag gMainMenu
    declare -ag gMainActn
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
    declare -g gMenuTitle='Main'
    declare -g gExitMenu=0

    tInProj=false
    if fComMustBeInProjRepo $PWD >/dev/null 2>&1; then
        tInProj=true
    fi

    # ------------------------------
    # Setup Menu Structure

    gMainMenu[0]="Select by number: "

    gMainActn[0]="\n  The 'health checks' will look for errors, differences, or just give a configuration status report. If there are validation errors, you will be prompted with repair options.\n\n  The 'actions' are most useful for updating an old project to use newer configuration settings that you have defined at the global level.\n"
    gMainMenu[1]="Quit, exit git-proj-config"
    gMainActn[1]="func exit 222"
    gMainMenu[2]="Help"
    gMainActn[2]="help"

    gMainMenu[3]="Run health checks that look for problems." #  [error]
    gMainActn[3]="func fConfigHealth error"

    gMainMenu[4]="Run health checks that look for differences." # [diff, warnings]
    gMainActn[4]="func fConfigHealth warning"

    gMainMenu[5]="Run all health checks and report their status" # [info]
    gMainActn[5]="func fConfigHealth info"

    gMainMenu[6]="Select actions to update configs or files"
    gMainActn[6]="menu gTypeMenu gTypeActn"

    # ----------
    gTypeMenu[0]="Select the configs or file to be moved:"
    gTypeActn[0]="\n Define the config sections or files that you want to update or copy."
    gTypeMenu[1]="Back"
    gTypeActn[1]="back"
    gTypeMenu[2]="Help"
    gTypeActn[2]="help"
    gTypeMenu[3]="Quit, back to main"
    gTypeActn[3]="func exit 2"
    gTypeMenu[4]="{copy-all} Copy all config section: core, alias, git-flow, gitproj.config"
    gTypeActn[4]="menu gConfMenu gConfActn"
    gTypeMenu[5]="{copy-some} Copy some config section: git-flow, gitproj.config"
    gTypeActn[5]="menu gConfMenu gConfActn"
    gTypeMenu[6]="{copy-gitproj} Only copy gitproj.config section"
    gTypeActn[6]="menu gConfMenu gConfActn"
    gTypeMenu[7]="{copy-hooks} Only copy gitproj.hooks section"
    gTypeActn[7]="menu gConfMenu gConfActn"
    gTypeMenu[8]="{gitignore} Copy gitignore file"
    gTypeActn[8]="menu gIgnoreMenu gIgnoreActn"
    gTypeMenu[9]="{pre-commit} Copy pre-commit file"
    gTypeActn[9]="menu gPreCommitMenu gPreCommitActn"

    # 4, 5, 6, 7
    gConfMenu[0]="Select the from/to:"
    
    gConfActn[0]="\n Define what level to copy from and what level to
copy to.\n\n For example, if you select 'Product -> User', then that
means sections from /usr/share/doc/git-proj/config/gitignore will be
copied to ~/.gitconfig\n\n The config 'levels' and files are more
completely described in the user-doc 'gitproj Configuration
Documentation' (config.md)"

    gConfMenu[1]="Back"
    gConfActn[1]="back"
    gConfMenu[2]="Help"
    gConfActn[2]="help"
    gConfMenu[3]="Quit, back to main"
    gConfActn[3]="func exit 2"
    gConfMenu[4]="{ProdUser} Product -> User"
    gConfActn[4]="menu gForceMenu gForceActn"
    if $tInProj; then
        gConfMenu[5]="{UserProj} User -> Project"
    	gConfActn[5]="menu gForceMenu gForceActn"
    	gConfMenu[6]="{ProjLocal} Project -> Local"
    	gConfActn[6]="menu gForceMenu gForceActn"
    	gConfMenu[7]="{LocalProj} Local -> Project"
	gConfActn[7]="menu gForceMenu gForceActn"
    	gConfMenu[8]="{ProjUser} Project -> User"
    	gConfActn[8]="menu gForceMenu gForceActn"
    fi
    
    # 7
    gIgnoreMenu[0]="Select the from -> to direction:"
    
    gIgnoreActn[0]="\n Define what level to copy from and what level
to copy to.\n\n For example, if you select 'Product -> User', then
that means /usr/share/doc/git-proj/config/gitignore will be merged to
~/.gitignore\n\n An existing ~/.gitignore will be copied to
~/.gitignore.bak\n\n The updated ~/.gitignore will be sorted with
duplicates and comments removed.\n\n The config 'levels' and files are
more completely described in the user-doc 'gitproj Configuration
Documentation' (config.md)"

    gIgnoreMenu[1]="Back"
    gIgnoreActn[1]="back"
    gIgnoreMenu[2]="Help"
    gIgnoreActn[2]="help"
    gIgnoreMenu[3]="Quit, back to main"
    gIgnoreActn[3]="func exit 3"
    gIgnoreMenu[4]="{ProdUser} Product -> User"
    gIgnoreActn[4]="menu gYesNoMenu gYesNoActn"
    if $tInProj; then
        gIgnoreMenu[5]="{UserProj} User -> Project"
     	gIgnoreActn[5]="menu gYesNoMenu gYesNoActn"
    	gIgnoreMenu[6]="{ProjUser} Project -> User"
    	gIgnoreActn[6]="menu gYesNoMenu gYesNoActn"
    fi
    
    # 8
    gPreCommitMenu[0]="Select the from -> to direction:"
    
    gPreCommitActn[0]="\n Define what level to copy from and what
level to copy to.\n\n For example, if you select 'Product -> User',
then that means /usr/share/doc/git-proj/hooks/pre-commit will be
copied to ~/.pre-commit\n\n An existing ~/.pre-commit will be copied
to ~/.pre-commit.~1~\n\n The config 'levels' and files are more
completely described in the user-doc 'gitproj Configuration
Documentation' (config.md)"

    gPreCommitMenu[1]="Back"
    gPreCommitActn[1]="back"
    gPreCommitMenu[2]="Help"
    gPreCommitActn[2]="help"
    gPreCommitMenu[3]="Quit, back to main"
    gPreCommitActn[3]="func exit 4"
    gPreCommitMenu[4]="{ProdUser} Product -> User"
    gPreCommitActn[4]="menu gYesNoMenu gYesNoActn"
    if $tInProj; then
        gPreCommitMenu[5]="{UserProj} User -> Project"
    	gPreCommitActn[5]="menu gYesNoMenu gYesNoActn"
    	gPreCommitMenu[6]="{ProjLocal} Project -> Local"
    	gPreCommitActn[6]="menu gYesNoMenu gYesNoActn"
    	gPreCommitMenu[7]="{LocalProj} Local -> Project"
    	gPreCommitActn[7]="menu gYesNoMenu gYesNoActn"
    	gPreCommitMenu[8]="{ProjUser} Project -> User"
   	gPreCommitActn[8]="menu gYesNoMenu gYesNoActn"
    fi
    
    # 3, 4, 5, 6
    gForceMenu[0]="Select the "force" option:"
    
    gForceActn[0]="\nIf you select 'Force copy', then the variables in
the 'from' file will replace the variables in the 'to' file.\n\n If
you select 'Only copy missing', then existing variables, in the 'to'
file, will not be replaced with variables in the 'from' file. But
missing variables will be copied from the 'from' file."
    
    gForceMenu[1]="Back"
    gForceActn[1]="back"
    gForceMenu[2]="Help"
    gForceActn[2]="help"
    gForceMenu[3]="Quit, back to main"
    gForceActn[3]="func exit 5"
    gForceMenu[4]="{force} Force copy"
    gForceActn[4]="menu gYesNoMenu gYesNoActn"
    gForceMenu[5]="{missing} Only copy missing"
    gForceActn[5]="menu gYesNoMenu gYesNoActn"

    gYesNoMenu[0]="Are the above actions correct?"

    gYesNoActn[0]="\n Selecting 'Yes' will make the changes you have
selected. If you do not want to continue, then you can Quit, to return
to the main menu, or select the Back options to update your
selections."
    
    gYesNoMenu[1]="Back"
    gYesNoActn[1]="back"
    gYesNoMenu[2]="Help"
    gYesNoActn[2]="help"
    gYesNoMenu[3]="Quit, back to main"
    gYesNoActn[3]="func exit 6"
    gYesNoMenu[4]="Yes, continue"
    gYesNoActn[4]="func fConfigExecAction \$gMenuTitle"

    while true; do
        (fComMenu "$gMenuTitle" gMainMenu gMainActn)
	if [ $? -eq 222 ]; then
	    break
	fi
    done
    return $?
} # fConfigMenu

# ========================================
fComPreProjSetGlobals
