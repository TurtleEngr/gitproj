#!/bin/bash

# --------------------
export cGetOrigin cGetTopDir cTop

export gpBin gpInProj gpTopDir \
    gpCmdName gpDebug gpDoc gpFacility \
    gpGitFlow gpHookVerbose gpLocalRawDir gpLocalStatus \
    gpLocalTopDir gpMaxSize gpPath gpPreCommitEnabled gpProjName \
    gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose

# --------------------
fCheckForErrors()
{
    local tSrc=${BASH_SOURCE##*/}
    local tConfFileList
    local tFile
    local tKey
    local tValue

    echo
    echo '----------------------------------------'
    echo "Errors - things that must be fixed"
    echo

    tConfFileList="$gpDoc/config/gitconfig $HOME/.gitconfig"
    if $gpInProj; then
        fConfFileList="$fConfFileList $gpTopDir/.gitproj $gpTopDir/.git/config"
    fi

    for tFile in $tConfFileList; do

        tValue=$(fComGetConfig -f $tFile -k "gitproj.config.facility" -d "null")
        echo $tValue | grep -Eq 'auth|authpriv|cron|daemon|ftp|lpr|mail|news|syslog|user|uucp|local[0-7]|null'
        if [ $? -ne 0 ]; then
            echo
            fError -n -m "Invalid facility name, $tValue, in $tFile. It can only be: auth, authpriv, cron, daemon, ftp, lpr, mail, news, syslog, user, uucp, or local[0-7]" -l $tSrc:$LINENO
        fi

        tValue=$(fComGetConfig -f $tFile -k "gitproj.config.bin" -d "null")
        if [ "$tValue" != "null" ]; then
            if [[ ! -d $tValue ]]; then
               echo
               fError -n -m "Invalid gitproj.config.bin, $tValue, in $tFile"  -l $tSrc:$LINENO
            fi
        fi

#       tValue=$(fComGetConfig -f $tFile -k "gitproj.config.doc" -d "null")
#       if [ "$tValue" != "null" ]; then
#           if [[ ! -d $tValue || ! -f $tValue/config/gitconfig ]]; then
#              echo
#              fError -n -m "$tValue directory not found. Set in gitproj.config.doc in $tFile"  -l $tSrc:$LINENO
#           fi
#       fi

        if [[ ! -d $gpDoc || ! -f $gpDoc/config/gitconfig ]]; then
            echo
            fError -n -m "$gpDoc directory not found."  -l $tSrc:$LINENO
        fi

        for tKey in \
            core.filemode \
            core.autocrlf \
            core.ignorecase \
            core.logAllRefUpdates \
            core.quotePath \
            core.trustctime \
            gitproj.config.syslog \
            gitproj.config.git-flow \
            gitproj.hook.verbose \
            gitproj.hook.pre-commit-enabled \
            gitproj.hook.check-file-names \
            gitproj.hook.gitproj.hook.check-in-raw \
            gitproj.hook.check-whitespace \
            gitproj.hook.check-for-tabs \
            gitproj.hook.check-for-big-files \
            ; do
            tValue=$(fComGetConfig -f $tFile -k "$tKey" -d "null")
            if [ "$tValue" = "null" ]; then
                continue
            fi
            if [[ "$tValue" != "false" && "$tValue" != "true" ]]; then
                echo
                fError -n -m "$tKey is $tValue, in file $tFile. It should only be 'true' or 'false'" -l $tSrc:$LINENO
            fi
        done

        for tKey in \
            gitproj.config.remote-min-space \
            gitproj.config.verbose \
            gitproj.hook.binary-file-size \
            ; do
            tValue=$(fComGetConfig -f $tFile -k "$tKey" -d "null")
            if [ "$tValue" = "null" ]; then
                continue
            fi
            echo ${tValue,,} | grep -Eq '[0-9][0-9]*[kmg]*'
            if [ $? -ne 0 ]; then
                echo
                fError -n -m "$tKey is $tValue, in file $tFile. It should only be an integer" -l $tSrc:$LINENO
            fi
        done
    done

    # ----------
    # Look for all required vars

    tConfFileList=""
    tValue=$(fComGetConfig -k gitproj.config.git-flow -d "null")
    if [ "$tValue" != "null" ]; then
            tConfFileList="gitflow.prefix.feature gitflow.prefix.release gitflow.prefix.hotfix gitflow.prefix.bugfix gitflow.prefix.support"
    fi

    if $gpInProj; then
        tConfFileList="$tConfFileList branch.main.remote branch.develop.remote remote.origin.url"
    fi

    for tKey in \
        gitproj.config.proj-status \
        gitproj.config.syslog \
        gitproj.config.facility \
        gitproj.config.bin \
        gitproj.config.doc \
        gitproj.config.git-flow-pkg \
        gitproj.config.git-flow \
        gitproj.config.local-status \
        gitproj.config.local-host \
        gitproj.config.proj-name \
        gitproj.config.remote-status \
        gitproj.config.remote-raw-origin \
        gitproj.config.remote-min-space \
        gitproj.config.verbose \
        gitproj.hook.verbose \
        gitproj.hook.pre-commit-enabled \
        gitproj.hook.check-file-names \
        gitproj.hook.check-in-raw \
        gitproj.hook.check-whitespace \
        gitproj.hook.check-for-tabs \
        gitproj.hook.tab-exclude-list \
        gitproj.hook.check-for-big-files \
        gitproj.hook.binary-file-size \
        $tConfFileList \
        ; do
        tValue=$(fComGetConfig -k $tKey -d "null")
        if [ "$tValue" = "null" ]; then
            echo
            fError -n -m "$tKey is not defined anywhere." -l $tSrc:$LINENO
        fi
    done

    # ----------
    for tFile in $gpDoc/config/gitconfig $HOME/.gitconfig; do
        for tKey in \
            gitproj.config.local-status \
            gitproj.config.local-host \
            gitproj.config.proj-name \
            gitproj.config.remote-status \
            gitproj.config.remote-raw-origin \
            ; do
            tValue=$(fComGetConfig -f $tFile -k $tKey -d "null")
            if [[ "$tValue" != "null" && "$tValue" != "TBD" ]]; then
                echo
                fError -n -m "$tKey should be TBD or NOT set in $tFile" -l $tSrc:$LINENO
            fi
        done
    done

    if $gpInProj; then
        for tKey in \
            gitproj.config.local-host \
            gitproj.config.remote-status \
            gitproj.config.remote-raw-origin \
            ; do
            tValue=$(fComGetConfig -f $gpTopDir/.gitproj -k $tKey -d "null")
            if [[ "$tValue" != "null" && "$tValue" != "TBD" ]]; then
                echo
                fError -n -m "$tKey should be TBD or NOT set in $gpTopDir/.gitproj" -l $tSrc:$LINENO
            fi
        done
    fi

    # ----------
    # List symlinks with problems [error]
    if $gpInProj; then
        cd $gpTopDir >/dev/null 2>&1
        for tFile in $(find . -type l); do
            if [ ! -e $tFile ]; then
                echo
                fError -n -m "Symlink $tFile is broken" -l $tSrc:$LINENO
            fi
        done
        cd - >/dev/null 2>&1
    fi

    # ----------
    if $gpInProj; then
        tFile=$gpTopDir/.git/config
        for tKey in \
            gitproj.config.local-status \
            gitproj.config.local-host \
            gitproj.config.proj-name \
            gitproj.config.remote-status \
            gitproj.config.remote-raw-origin \
            ; do
            tValue=$(fComGetConfig -f $tFile -k $tKey -d "null")
            if [[ "$tValue" = "null" || "$tValue" = "TBD" ]]; then
                echo
                fError -n -m "$tKey is not set in $tFile. Run git-proj-init or git-proj-remote" -l $tSrc:$LINENO
            fi
        done
    fi

    return 0
} # fCheckForErrors

fCheckForWarnings()
{
    local tSrc=${BASH_SOURCE##*/}
    local tKey
    local tValue
    local tValue2
    local tMount

    echo
    echo '----------------------------------------'
    echo "Warnings - things that should be looked at"
    echo

    # gitignore diffs
    if ! diff -q $gpDoc/config/gitignore $HOME/.gitignore >/dev/null 2>&1; then
        echo
        fLog -f -p warning -m "$gpDoc/config/gitignore ~/.gitignore are different" -l $tSrc:$LINENO
    fi
    if $gpInProj; then
        if ! diff -q $HOME/.gitignore $gpTopDir/.gitignore >/dev/null 2>&1; then
            echo
            fLog -f -p warning -m "~/.gitignore $gpTopDir/.gitignore are different" -l $tSrc:$LINENO
        fi
    fi

    # pre-commit diffs
    if ! diff -q $gpDoc/hooks/pre-commit $HOME/.pre-commit >/dev/null 2>&1; then
        echo
        fLog -f -p warning -m "$gpDoc/hooks/pre-commit ~/.pre-commit are different" -l $tSrc:$LINENO
    fi
    if $gpInProj; then
        if ! diff -q $HOME/.pre-commit $gpTopDir/.pre-commit >/dev/null 2>&1; then
            echo
            fLog -f -p warning -m "~/.pre-commit $gpTopDir/.pre-commit are different" -l $tSrc:$LINENO
        fi
        if ! diff -q $gpTopDir/.pre-commit $gpTopDir/.git/hooks/pre-commit >/dev/null 2>&1; then
            echo
            fLog -f -p warning -m "$gpTopDir/.pre-commit $gpTopDir/.git/hooks/pre-commit are different" -l $tSrc:$LINENO
        fi
    fi

    # Note disabled pre-commit checks [info]
    if $gpInProj; then
        for tKey in \
            gitproj.hook.pre-commit-enabled \
            gitproj.hook.check-in-raw \
            gitproj.hook.check-for-big-files \
            ; do
            tValue=$(fComGetConfig -k $tKey)
            if [ "$tValue" != "true" ]; then
                echo
                fLog -f -p warning -m "$tKey should be 'true'" -l $tSrc:$LINENO
            fi
        done
        tValue=$(fComGetConfig -k gitproj.hook.binary-file-size -d null)
        if [ "$tValue" = "null" ]; then
            echo
            fError -n -m "gitproj.hook.binary-file-size is not set" -l $tSrc:$LINENO
        fi
        if [ ! -f $HOME/.gitignore ]; then
            echo
	    fError -n -m "Missing ~/.gitignore" -l $tSrc:$LINENO
	else
            if ! grep -q '/raw/' $HOME/.gitignore; then
                echo
            	fError -n -m "/raw/ was not found in ~/.gitignore." -l $tSrc:$LINENO
	    fi
	fi
        if [ ! -f $gpTopDir/.gitignore ]; then
            echo
	    fError -n -m "Missing PROJ/.gitignore" -l $tSrc:$LINENO
	else
            if ! grep -q '/raw/' $gpTopDir/.gitignore; then
                echo
            	fError -n -m "/raw/ was not found in PROJ/.gitignore" -l $tSrc:$LINENO
            fi
	fi
    fi

    # ----------
    if $gpInProj; then
        if [ "$gpProjName" != "${gpTopDir##*/}" ]; then
            echo
            fLog -f -p warning -m "proj-name != ${gpTopDir##*/}" -l $tSrc:$LINENO
        fi

	# Compare PROJ/.gitproj to ~/.gitconfig
        for tKeyValue in $(git config --list -f $gpTopDir/.gitproj | \
	  grep -Ev 'TBD|proj-name|local-status|remote-status'); do
            tKey=${tKeyValue%%=*}
            tValue=${tKeyValue##*=}
            tValue2=$(fComGetConfig -g -k $tKey -d "null")
            if [ "$tValue" != "$tValue2" ]; then
                echo
                fLog -f -p warning -m "$tKey in $gpTopDir/.gitproj is different from ~/.gitconfig ($tValue vs $tValue2)" -l $tSrc:$LINENO
            fi
            tValue2=$(fComGetConfig -l -k $tKey -d "null")
            if [ "$tValue" != "$tValue2" ]; then
                echo
                fLog -f -p warning -m "$tKey in $gpTopDir/.git/config is different from $gpTopDir/.gitproj ($tValue2 vs $tValue)" -l $tSrc:$LINENO
            fi
        done
    fi

    # ----------
    # Check space on remote
    if $gpInProj; then
    tMount=$(fComGetConfig -l -k "gitproj.config.remote-raw-origin")
        if fComIsRemoteMounted; then
            fComCheckDirSpace "$tMount" "$gpRemoteMinSpace"
        else
            echo
            fLog -f -p warning -m "$tMount is not mounted" -l $tSrc:$LINENO
        fi
    fi

    # TBD Check for big binary files, not in raw/ [warning]

    return 0
} # fCheckForWarnings

fCheckStatus()
{
    local tSrc=${BASH_SOURCE##*/}
    local tKey
    local tValue
    local tFile

    echo
    echo '----------------------------------------'
    echo 'List all config variables and where they are set'
    echo
    for tKey in \
        core.filemode \
        core.autocrlf \
        core.ignorecase \
        core.excludesFile \
        core.logAllRefUpdates \
        core.quotePath \
        core.trustctime \
        gap \
        user.name \
        user.email \
        gap \
        gitflow.branch.master \
        gitflow.branch.main \
        gitflow.branch.develop \
        gitflow.prefix.feature \
        gitflow.prefix.release \
        gitflow.prefix.hotfix \
        gitflow.prefix.bugfix \
        gitflow.prefix.support \
        gap \
        gitproj.config.proj-status \
        gitproj.config.syslog \
        gitproj.config.facility \
        gitproj.config.bin \
        gitproj.config.doc \
        gitproj.config.git-flow-pkg \
        gitproj.config.git-flow \
        gitproj.config.local-status \
        gitproj.config.local-host \
        gitproj.config.proj-name \
        gitproj.config.remote-status \
        gitproj.config.remote-raw-origin \
        gitproj.config.remote-min-space \
        gitproj.config.verbose \
        gap \
        gitproj.hook.verbose \
        gitproj.hook.pre-commit-enabled \
        gitproj.hook.check-file-names \
        gitproj.hook.check-in-raw \
        gitproj.hook.check-whitespace \
        gitproj.hook.check-for-tabs \
        gitproj.hook.tab-include-list \
        gitproj.hook.tab-exclude-list \
        gitproj.hook.check-for-big-files \
        gitproj.hook.binary-file-size \
        ; do
        if [ "$tKey" = "gap" ]; then
            echo
            continue
        fi
        tValue=$(git config --get --show-origin $tKey)
        tValue=$(echo $tValue | tr '\t' ' ')
        tFile=${tValue%% *}
        tValue=${tValue#file:* }
        printf "%-33s = %-10s\t%s\n" $tKey $tValue $tFile
    done

    if $gpInProj; then
        echo
        echo '----------------------------------------'
        echo "$gpProjName/raw/ Report"
        echo
        tNumRaw=$(find $gpTopDir/raw -type f | wc -l)
        tSizeRaw=$(du -sm $gpTopDir/raw | awk '{print $1}')
        echo -n "There are $tNumRaw files in raw/ using: "
        echo "${tSizeRaw}MB"
    fi

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
            echo
            fError -i -m "Invalid option: $pStatus" -l $tSrc$LINENO -e 20
            ;;
    esac

    return 0
} # fConfigHealth

fConfigUpdate()
{
    local pInc=$1
    local pFiles=$2
    local pForce=$3
    local tSrc=${BASH_SOURCE##*/}
    local tFrom
    local tTo
    local tForce
    local tExclude

    if [ "$pForce" = "force" ]; then
        tForce='-f'
    fi

    case $pFiles in
        ProdUser)
            tFrom=/usr/share/doc/git-proj/config/gitconfig
            tTo=$HOME/.gitconfig
            ;;
        UserProj)
            tFrom=$HOME/.gitconfig
            tTo=$gpTopDir/.gitproj
            tExclude='^gitproj\.config\.local-status|^gitproj\.config\.remote-status|^gitproj\.config\.proj-name'
            ;;
        ProjLocal)
            tFrom=$gpTopDir/.gitproj
            tTo=$gpTopDir/.git/config
            tExclude='^gitproj\.config\.local-status|^gitproj\.config\.local-host|^gitproj\.config\.proj-name|^gitproj\.config\.remote-status|^gitproj\.config\.remote-raw-origin'
            ;;
        LocalProj)
            tFrom=$gpTopDir/.git/config
            tTo=$gpTopDir/.gitproj
            tExclude="^gitproj\.config\.local-host|^gitproj\.config\.remote-raw-origin"
            ;;
        ProjUser)
            tFrom=$gpTopDir/.gitproj
            tTo=$HOME/.gitconfig
            tExclude="^gitproj\.config\.local-status|^gitproj\.config\.remote-status"
            ;;
        *)
            echo
            fError -i -m "Unknown option: $pFiles" -l $tSrc:$LINENO -e 20
            ;;
    esac

    fComConfigCopy $tForce -s $tFrom -d $tTo -i "$pInc" -e "$tExclude"
    return $?
} # fConfigUpdate

fConfigMergeIgnore()
{
    local pFrom=$1
    local pTo=$2
    local tSrc=${BASH_SOURCE##*/}

    if [ ! -f $pFrom ]; then
        echo
        fError -n -m "Missing: $pFrom" -l $tSrc:$LINENO -e 20
	return 1
    fi
    if [ ! -f $tTo ]; then
        echo
        cp -v $pFrom $tTo
        return $?
    fi

    echo
    cp -v --backup=t  $tTo $tTo.bak
    cat $pFrom $pTo | grep -v '^#' | sort -fu >t.tmp
    mv -f t.tmp $pTo

    return 0
} # fConfigMergeIgnore

fConfigIgnore()
{
    local pFiles=$1
    local tSrc=${BASH_SOURCE##*/}
    local tFrom
    local tTo

    case $pFiles in
        ProdUser)
            tFrom=$gpDoc/config/gitignore
            tTo=$HOME/.gitignore
            ;;
        UserProj)
            tFrom=$HOME/.gitignore
            tTo=$gpTopDir/.gitignore
            ;;
        ProjUser)
            tFrom=$gpTopDir/.gitignore
            tTo=$HOME/.gitignore
            ;;
        *)
            echo
            fError -i -m "Unknown option: $pFiles" -l $tSrc:$LINENO -e 20
            ;;
    esac

    fConfigMergeIgnore $tFrom $tTo
    return $?
} # fConfigIgnore

fConfigCopyPreCommit()
{
    local pFrom=$1
    local pTo=$2
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

    case $pFiles in
        ProdUser)
            tFrom=$gpDoc/hooks/pre-commit
            tTo=$HOME/.pre-commit
            ;;
        UserProj)
            tFrom=$HOME/.pre-commit
            tTo=$gpTopDir/.pre-commit
            ;;
        ProjLocal)
            tFrom=$gpTopDir/.pre-commit
            tTo=$gpTopDir/.git/hooks/pre-commit
            ;;
        LocalProj)
            tFrom=$gpTopDir/.git/hooks/pre-commit
            tTo=$gpTopDir/.git/hooks/pre-commit
            ;;
        ProjUser)
            tFrom=$gpTopDir/.git/hooks/pre-commit
            tTo=$HOME/.pre-commit
            ;;
        *)
            echo
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
            fConfigUpdate '^core\.|^alias\.|^git-flow\.|^gitproj\.config\.' ${tArgList[2]} ${tArgList[3]}
            ;;
        copy-some)
            fConfigUpdate '^git-flow\.|^gitproj\.config\.' ${tArgList[2]} ${tArgList[3]}
            ;;
        copy-gitproj)
            fConfigUpdate '^gitproj\.config\.' ${tArgList[2]} ${tArgList[3]}
            ;;
        copy-hooks)
            fConfigUpdate '^gitproj\.hooks\.' ${tArgList[2]} ${tArgList[3]}
            ;;
        gitignore)
            fConfigIgnore ${tArgList[2]}
            ;;
        pre-commit)
            fConfigPreCommit ${tArgList[2]}
            ;;
        *)
            echo
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

    gpInProj=false
    gpTopDir=""
    if fComMustBeInProjRepo $PWD >/dev/null 2>&1; then
        gpInProj=true
        gpTopDir=$($cGetTopDir)
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
    if $gpInProj; then
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
    if $gpInProj; then
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
    if $gpInProj; then
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
    gForceMenu[0]="Select the 'force' option:"

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
