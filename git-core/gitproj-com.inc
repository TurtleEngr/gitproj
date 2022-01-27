#!/bin/bash

# --------------------------------
export cGitProjVersion cGetOrigin cGetTopDir cGetCurBranch cPID

export PWD gErr gSavedOptE gGetConfigOrigin gGetConfigValue gResponse

export gpAuto gpAutoMove gpBin gpCheckFileNames gpCheckForBigFiles \
    gpCheckInRaw gpCheckWhitespace gpCmdName gpDebug gpDoc gpFacility \
    gpGitFlow gpHookVerbose gpLocalRawDir gpLocalStatus gpLocalTopDir \
    gpMaxLoop gpMaxSize gpMountDir gpPath gpPreCommitEnabled gpProjName \
    gpProjStatus gpRemoteStatus gpSysLog gpVer gpVerbose gpYesNo

# --------------------------------
fComGitProjInternalDoc()
{
    return
    # This should be the first =internal-pod
    cat <<\EOF >/dev/null
=internal-pod

=internal-for text ========================================

=internal-for html <hr/>

=internal-head1 gitproj-com.inc Internal Documentation

=internal-for html <hr/>

=internal-head2 Common Script Functions

=internal-head3 try/catch functions

B<NOTE the env. var. part does not reliably work. So don't use the
"tc" functions until they are fixed.>

These functions were inspired by:
https://gist.github.com/e7d/e43e6586c1c2ecb67ae2

Changes: This version doesn't save the previous state of "set -e".  Exported
env. var. changes in the subshell are saved, so they can be restored
when the subshell is done.

=internal-head4 tcTry

Call this just before the required subshell section.

=internal-head4 tcSaveEnv

Call this when env changes must not be lost. This is called by the
other related commands.

=internal-head4 tcReturn

Call this at end of the tcTry subshell.
Calls tcSaveEnv.

=internal-head4 tcThrow

Call this to throw error (1..255) to tcCatch
Calls tcSaveEnv.

=internal-head4 tcCatch

Save throw or cmd errors, and restore any env. var. changes in the subshell.
If a commnd exits before a tcSaveEnv call, env. var. changes will be lost.
Calls tcSaveEnv.

=internal-head4 tcThrowErrors

Now, if a cmd returns non-zero, exit the try subshell.
Calls tcSaveEnv.

=internal-head4 tcIgnoreErrors

Now, if a cmd returns non-zero, execution will continue.
Calls tcSaveEnv.

=internal-head4 Sample Use

    #!/bin/bash
    . try_catch.sh

    export cFatal=100
    export cWarn=101
    export cmd1 cmd2 foo1 foo2 foo3

    fCmd1()
    {
        tcMsg="fCmd1 error"
        if [ $((RANDOM % 3)) = 1 ]; then
            rm /x/y/zz/y
        fi
        echo cmd1="fCmd1 ran OK"
    }

    fCmd2()
    {
        tcMsg="fCmd2 error"
        if [ $((RANDOM % 4)) = 1 ]; then
            rm /x/y/zz/y
        fi
        if [ $((RANDOM % 4)) = 1 ]; then
          tcThrow 42 "fCmd2 threw this"
        fi
        echo cmd2="fCmd2 ran OK"
    }

    fTestIt()
    {
        tcMessage="Default message for exits"
        tcTry
        (
            # This is an error, but execution will continue
            rm /x/y/zz/y

            tcThrowErrors
            # Commands will now go to tcCatch if non-zero return

            if [ $((RANDOM % 3)) = 1 ]; then
                tcThrow $cFatal "Throw message. Line=$LINENO"
            fi
            export foo1="this will not be set if tcThrow is run"
            tcSaveEnv

            if [ $((RANDOM % 3)) = 1 ]; then
                rm /x/y/zz/y
            fi
            export foo2="this is set because 'rm' was not run", and tcSaveEnv ran
            tcSaveEnv

            fCmd1
            fCmd2

            # This will fail after it is created, second run, if no throws
            mkdir foo-bar
            export foo3="This not be set on second run"

            tcReturn
        )
        tcCatch || {
            case $tcErr in
                $cFatal)
                    echo "Caught: Throw happened. $tcMsg"
                    ;;
                $cWarn)
                    echo "Caught: Warning. $tcMsg"
                    ;;
                *)  echo "Caught: Unknown exit: $tcErr. $tcMsg" ;;
            esac
        }
        echo
        echo foo1=$foo1
        echo foo2=$foo2
        echo foo3=$foo3
        echo cmd1=$cmd1
        echo cmd2=$cmd1
    } # fTestIt

    fTestIt

=internal-cut

EOF
} # fGitProjComInternalDoc

# ========================================
# Common Functions

# --------------------
tcTry()
{
    # Call this just before a subshell
    set +e
    export tcErr=0
    export tcMsg=""
    export tcEnvBefore=/tmp/env-before-$$.tmp
    export tcEnvAfter=/tmp/env-after-$$.tmp
    export tcEnvDiff=/tmp/env-diff-$$.sh
    env | sort >$tcEnvBefore
}

tcSaveEnv()
{
    # Call this when env changes must not be lost
    env | sort >$tcEnvAfter
}

tcReturn()
{
    # Call this at end of tcTry subshell
    tcSaveEnv
    exit 0
}

tcThrow()
{
    tcErr=${1:-0}
    tcMsg=${2:-$tcMsg}
    tcSaveEnv
    exit $tcErr
}

tcCatch()
{
    # Save error code, and set any env. var. changes, including tcMsg
    tcErr=$?
    set +e
    if [ -f $tcEnvAfter ]; then
        diff $tcEnvBefore $tcEnvAfter | grep '>' | sed 's/^> //; s/=/="/; s/$/"/' >$tcEnvDiff
        chmod a+rx $tcEnvDiff
        . $tcEnvDiff
        if [ $tcErr -eq 0 ]; then
            cd $PWD
        fi
    fi
    rm -f $tcEnvBefore $tcEnvAfter $tcEnvDiff >/dev/null 2>&1
    unset tcEnvBefore tcEnvAfter tcEnvDiff
    return $tcErr
}

tcThrowErrors()
{
    # Now if any cmd returns non-zero, exit the "try" subshell
    tcSaveEnv
    set -e
}

tcIgnoreErrors()
{
    # Now if any cmd returns non-zero, execution will continue
    tcSaveEnv
    set +e
}

# --------------------
fComIntroText()
{
    local tSrc=${BASH_SOURCE##*/}

    cat <<EOF | fComFmt

======================================================================

You now have a local git repository project (${gpProjName}) setup at
${gpLocalTopDir} with support for managing large binary files (larger
than $gpMaxSize).

~/.gitconfig will include the ~/.gitproj.config.global file, which
will be used for defaults when you create other "git proj" projects on
your system.  The global variables can be overridden by a project's
config definitions. See: Project/.gitproj.config.locall and
Project/.gitproj.config.\$HOSTNAME You should version these files with
your project.

When you "clone" your git project on another system, a new
Project/.gitproj.config.\$HOSTNAME file will be created, so that
differences between systems are saved. For example, mount points and
filesystem differences will be saved.

For a more complete overview type: "git proj -h" or "man
git-proj"

For a quick list of the git proj subcommands just type: "git proj"

All subcommands will output help text with the -h option. For example:
"git proj SubCmd -h

EOF
    return 0
} # fComIntroText

# --------------------
fComConfigCopy()
{
    local pForce=0
    local pSource=""
    local pDest=""
    local pIncPat=""
    local pExcPat="@@"
    local tSrc=${BASH_SOURCE##*/}
    local tVerbose=""
    declare -l tVar
    local tValue
    local tFound
    local OPTIND=1
    local OPTARG
    local tArg

    if [ $gpVerbose -ge 2 ]; then
        tVerbose='-v'
    fi

    while getopts :fs:d:i:e: tArg; do
        # shellcheck disable=SC2213,SC2220
        case $tArg in
            f) pForce=1 ;;
            s) pSource="${OPTARG}" ;;
            d) pDest="${OPTARG}" ;;
            i) pIncPat="${OPTARG}" ;;
            e) pExcPat="${OPTARG}" ;;
            :)
                fError -n -i -m "fComConfigCopy: Value required for option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
            \?)
                fError -n -i -m "ComConfigCopy: Unknown option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
        esac
    done

    if [ -z "$pSource" ] || [ ! -r $pSource ]; then
        fError -n -i -m "fComConfigCopy: -s $pSource is missing or not readable" -l $tSrc:$LINENO
        return 1
    fi
    if [ -z "$pDest" ] || [ ! -w $pDest ]; then
        fError -n -i -m "fComConfigCopy: -d $pDest is missing or not writable" -l $tSrc:$LINENO
        return 1
    fi

    cp $tVerbose --backup=t $pDest $pDest.bak
    if [ $? -ne 0 ]; then
        fError -n -i -m "fComConfigCopy: " -l $tSrc:$LINENO
        return 1
    fi

    git config -f $pSource --list | while read tSrcConfig; do
        tVar=${tSrcConfig%=*}
        tValue="${tSrcConfig#*=}"
        tFound=$(echo $tVar | grep -E "$pIncPat" | grep -Ev "$pExcPat")
        if [ -z "$tFound" ]; then
            continue
        fi
        if [ $pForce -ne 0 ]; then
            git config -f $pDest $tVar $tValue
            continue
        fi
        if git config -f $pDest --get $tVar >/dev/null 2>&1; then
            continue
        fi
        git config -f $pDest $tVar "$tValue"
    done
    if diff -q $pDest $pDest.bak; then
        rm -f $pDest.bak
    fi

    return 0
    # ----------------------
    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComConfigCopy

=internal-head4 Synopsis

    fComConfigCopy [-f] -s pSource -d pDest [-i pInclPat] [-e pExclPat]

=internal-head4 Description

Copy values from git config file pSource to pDest. If -f (force), the pSource
values will override the pDest values, otherwise the pSource value will
only be copied to pDest if it does not exist there.

The -i pIncPat is a "grep" pattern that only selects the variable
names that match the pattern. For example:
S<-i 'gitproj\.config\.|gitflow\.'>

The -e pExcPat is a "grep" pattern that removes variable names that
match the pattern. For example: S<-e 'gitproj.config.remote-raw-origin'>
pExcPat is applied after pIncPat.

pSource and pDest files must exist. pSource must be readable, and
pDest must be writable.

A backup copy of pDest is made with this command:
S<cp -backup=t $pDest $pDest.bak>

=internal-cut
EOF
} # fComConfigCopy

# --------------------
fComConfigUpdateLocal()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir
    local tExclude="local-status|local-host|remote-status|remote-raw-origin"

    if ! fComMustBeInProjRepo $PWD >/dev/null 2>&1; then
        fError -i -n -m "fComConfigUpdateLocal: Missing: $tTopDir/.gitproj" -l $tSrc:$LINENO
        return 1
    fi

    tTopDir=$($cGetTopDir)
    if [ -f $tTopDir/.gitproj ] && [ $tTopDir/.git/config -ot $tTopDir/.gitproj ]; then
        return 0
    fi

    fComConfigCopy -f -s $tTopDir/.git/config -d $tTopDir/.gitproj \
        -i 'gitproj\.' -e "$tExclude"

    # Force TBD values?
    ## fComConfigCopy -f -s $gpDoc/config/gitconfig -d -d $tTopDir/.gitproj \
    ##    -i $tExclude

    git add $tTopDir/.gitproj
    git commit -m "Updated .gitproj" $tTopDir/.gitproj
    return 0

    # or just be noisy?
    # if ! git ls-files --error-unmatch $tTopDir/.gitproj >/dev/null 2>&1; then
    #     fError -n -m "You must version $tTopDir/.gitproj" -l $tSrc:$LINENO
    # fi
} # fComConfigUpdateLocal

# --------------------
fComFirstTimeSet()
{
    local tSrc=${BASH_SOURCE##*/}
    local tInclude
    local tUserName
    local tUserEmail
    local tVerbose

    if [ $gpVerbose -ge 2 ]; then
        tVerbose='-v'
    fi

    if [ ! -f $HOME/.gitconfig ]; then
        fLog -p info -m "Create: $HOME/.gitconfig" -l $tSrc:$LINENO
        cp $tVerbose --backup=t $gpDoc/config/gitconfig $HOME/.gitconfig
        return 0
    fi

    tUserName=$(fComGetConfig -g -k "user.name")
    tUserEmail=$(fComGetConfig -g -k "user.email")
    if [ -z "$tUserName" ] || [ -z "$tUserEmail" ]; then
        fError -m "user.name and user.email must be in your ~/.gitconfig file. See $gpDoc/config/gitconfig for some other useful settings." -l $tSrc:$LINENO
    fi

    fComConfigCopy -s $gpDoc/config/gitconfig -d $HOME/.gitconfig
    return 0
} # fComFirstTimeSet

# -------------------
fComSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}

    # -------------------
    # Set current directory location in PWD, because with cron jobs PWD
    # is not set.
    if [ -z "$PWD" ]; then
        PWD=$(pwd)
    fi

    # -------------------
    # Define the location of the com script if not already set
    if [ -z "$gpBin" ]; then
        gpBin=${0%/*}
        if [ "$gpBin" = "." ]; then
            gpBin=$PWD
        fi
        cd $gpBin >/dev/null 2>&1
        gpBin=$PWD
        cd - >/dev/null 2>&1
    fi

    # -------------------
    # Constants

    gpDoc=${gpDoc:-/usr/share/doc/git-proj}
    if [ ! -d $gpDoc ]; then
        gpDoc=$gpBin/../doc
    fi
    if [ ! -d $gpDoc ]; then
        fError -m "Error: Cannot find gitproj doc directory: $gpDoc" -l $tSrc:$LINENO
    fi
    cd $gpDoc >/dev/null 2>&1
    gpDoc=$PWD
    cd - >/dev/null 2>&1

    cGitProjVersion=$(cat $gpDoc/VERSION)

    # Include in ~/.gitconfig (Use -g to set config vars to this)
    # Note: ~/.gitconfig does not work all the time, so use:
    # $HOME/.gitconfig or use --global

    # Include in PROJ/.gitproj (Use -L to set config vars to this)
    # Include in PROJ/.git/config (Use -l to set config vars to this)

    cGetOrigin='git config --get remote.origin.url'
    # Use: tOrigin=$($cGetOrigin)
    #      if $cGetOrigin >/dev/null 2>&1; then echo "In a git dir"; fi

    cGetTopDir='git rev-parse --show-toplevel'
    # Use: tTop=$($cGetTopDir); cd $tTop

    cGetCurBranch='git rev-parse --abbrev-ref HEAD'
    # Use: tBranch=$($cGetCurBranch)

    gpMaxLoop=${gpMaxLoop:-20}
    # Used internally to limit number of invalid responses to prompts.
    # For testing this is usually reduced.

    # -------------------
    # Setup log variables
    # -x
    gpDebug=${gpDebug:-0}

    # -v
    gpVerbose=${gpVerbose:-2}

    # -l
    gpSysLog=${gpSysLog:-false}
    gpFacility=${gpFacility:-user}

    gErr=0

    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComSetGlobals

Set initial values for all of the globals use by this script. The ones
that begin with "gp" can usually be overridden by setting them before
the script is run.

=internal-cut

EOF
} # fComSetGlobals

# --------------------
fComPreProjSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTrim

    fComFirstTimeSet

    gpVer=$cGitProjVersion

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "false")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpVerbose=$(fComGetConfig -k "gitproj.config.verbose" -i -d 2)

    gpAuto=0
    gpLocalTopDir=$PWD
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "TBD")
    if [ "$gpProjName" = "TBD" ]; then
        gpProjName="${gpLocalTopDir##*/}"
    fi

    gpLocalRawDir=$gpLocalTopDir/raw
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #20g

    gpAutoMove="true"
    gpGitFlow="false"
    gpGitFlowPkg=$(fComGetConfig -k "gitproj.config.git-flow-pkg" -d "git-flow")

    gpProjStatus=$(fComGetConfig -k "gitproj.config.proj-status" -d "installed")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")

    gpMountDir=TBD
    gpRemoteRawOrigin=TBD

    # Hook section
    gpHookVerbose=$(fComGetConfig -k "gitproj.hook.verbose" -d true)
    gpPreCommitEnabled=$(fComGetConfig -k "gitproj.hook.pre-commit-enabled" -d true)
    gpCheckFileNames=$(fComGetConfig -k "gitproj.hook.check-file-names" -d true)
    gpCheckInRaw=$(fComGetConfig -k "gitproj.hook.check-in-raw" -d true)
    gpCheckWhitespace=$(fComGetConfig -k "gitproj.hook.check-whitespace" -d true)
    gpCheckForBigFiles=$(fComGetConfig -k "gitproj.hook.check-for-big-files" -d true)
    gpMaxSize=$(fComGetConfig -k "gitproj.hook.binary-file-size-limit" -d "10k")

    # These will be redefined later
    #    gpLocalTopDir
    #    gpProjName, gitproj.config.name
    #    gpLocalRawDir=$gpLocalTopDir/raw
    #    gpGitFlow="false"
    #    gpLocalStatus, gitproj.config.local-status
    #    gpRemoteStatus, gitproj.config.remote-status
    #    gpCheckFileNames=true
    #    gpCheckForBigFiles=true
    #    gpPreCommitEnabled=true
    #    gpMountDir=TBD
    #    gpRemoteRawOrigin=TBD

    return 0
} # fComPreProjSetGlobals

# --------------------------------
fComCheckDeps()
{
    local pRequired="$1"
    local pOptional="$2"
    local tSrc=${BASH_SOURCE##*/}
    local tProg
    local tErr=0
    local tComReq='logger pod2text pod2usage rsync'
    local tComOpt='pod2html pod2man pod2markdown tidy shunit2.1 gawk sed tr fmt'

    for tProg in $pOptional $tComOpt; do
        if ! command -v $tProg >/dev/null 2>&1; then
            echo "Optional: Missing $tProg"
            tErr=1
        fi
    done

    for tProg in $pRequired $tComReq; do
        if ! command -v $tProg >/dev/null 2>&1; then
            echo "Required: Missing $tProg"
            tErr=2
        fi
    done

    if [ $tErr -eq 2 ]; then
        fError -m "Missing one or more required programs." -l $tSrc:$LINENO
    fi
    if [ $tErr -eq 1 ]; then
        fLog -p warning -m "Missing some some optional programs." -l $LINENO
    fi
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-for text ========================================

=internal-for html <hr/>

=internal-head3 fComCheckDeps "pRequired List" "pOptional List"

Check for required and optional programs or scripts used by this script.
If any required programs are missing, exit the script.

=internal-cut
EOF
} # fComCheckDeps

# --------------------------------
fComInternalDoc()
{
    local tSrc=${BASH_SOURCE##*/}

    awk '
        /^=internal-pod/,/^=internal-cut/ {
            sub(/^=internal-/,"=");
            print $0;
        }
    '
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComInternalDoc

This function collects all of the "internal-pod" documentation from
stdin and it outputs to stdout.

=internal-cut
EOF
} # fComInternalDoc

# --------------------------------
fComUsage()
{
    local pFileList=""
    local pStyle="long"
    local pInt=0
    local pTitle="$gpCmdName Usage"
    local tSrc=${BASH_SOURCE##*/}
    local tProg=""
    local tTidy='tidy -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no'
    local tHtmlOpt='--cachedir=/tmp --index'
    local OPTIND=1
    local OPTARG
    local tArg

    while getopts :f:s:t:i tArg; do
        case $tArg in
            f) pFileList="$pFileList $OPTARG" ;;
            s) pStyle="$OPTARG" ;;
            t) pTitle="$OPTARG" ;;
            i) pInt=1 ;;
            :) fError -i -m "fComUsage: Value required for option: -$OPTARG" -l $tSrc:$LINENO ;;
            \?) fError -i "fComUsage: Unknown option: $OPTARG" -l $tSrc:$LINENO ;;
        esac
    done
    shift $((OPTIND - 1))
    if [ $# -ne 0 ]; then
        fError -i -m "fComUsage: Unknown option: $*" -l $tSrc:$LINENO
    fi

    case $pStyle in
        short | usage)
            tProg=pod2usage
            if [ $pInt -ne 0 ]; then
                # This fails, don't know why
                # tProg='pod2text | head -n 24'
                tProg=pod2text
            fi
            ;;
        man)
            tProg=pod2man
            if [ $pInt -ne 0 ]; then
                tProg=pod2text
            fi
            ;;
        long | text)
            tProg=pod2text
            ;;
        html)
            tProg=pod2html
            ;;
        md)
            tProg=pod2markdown
            ;;
        *)
            tProg=pod2text
            fLog -p warning -m "Unknown style: '$pStyle' so usiing text" -l $tSrc:$LINENO
            ;;
    esac

    # Validate
    if ! command -v $tProg >/dev/null 2>&1; then
        fLog -p warning -m "Could not find $tProg so using text" -l $tSrc:$LINENO
        tProg=pod2text
    fi

    # shellcheck disable=SC2116
    pFileList="$(echo $pFileList)"
    if [ -z "$pFileList" ]; then
        fError -i -m "fComUsage missing pFileList" -l $tSrc:$LINENO
    fi
    for tFile in $pFileList; do
        if [ ! -r $tFile ]; then
            fError -i -m "fComUsage cannot read: $tFile" -l $tSrc:$LINENO
        fi
    done

    if [ $pInt -eq 0 ]; then
        if [ "$pStyle" = "html" ]; then
            # shellcheck disable=SC2002
            cat $pFileList |
                $tProg --title="$pTitle" $tHtmlOpt |
                $tTidy 2>/dev/null | more
        else
            if [ "$pStyle" = "short" ]; then
                # shellcheck disable=SC2002
                cat $pFileList | pod2text | head -n 10
            else
                # shellcheck disable=SC2002
                cat $pFileList | $tProg | more
            fi
        fi
    else
        if [ "$pStyle" = "html" ]; then
            # shellcheck disable=SC2002
            cat $pFileList | fComInternalDoc |
                $tProg --title="$pTitle" $tHtmlOpt |
                $tTidy 2>/dev/null | more
        else
            # shellcheck disable=SC2002
            cat $pFileList | fComInternalDoc | $tProg | more
        fi
    fi

    gErr=1
    exit 1

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComUsage -f pFileList -s pStyle [-t pTitle] [-i] [-a]

=internal-over 4

=internal-item B<-f pFileList> - list of file names

=internal-item B<-s pStyle> - output style

 short|usage - usage only (does not work with -i or -a)
 man         - all, man format (does not work with -i or -a)
 long|text   - all text format
 html        - all, html format (see -t)
 md          - all, markdown format

=internal-item B<-t> - title for HTML style

=internal-item B<-i> - internal doc only (see fComInternalDoc)

=internal-back

=internal-cut
EOF
} # fComUsage

# ------------------
fComStackTrace()
{
    local i
    local tSrc
    local tLine
    local tName
    local tOutput

    # Credit: https://gist.github.com/ahendrix/7030300
    if [ ${#FUNCNAME[@]} -le 2 ]; then
        return 0
    fi

    if [ "${gpSysLog:-false}" = "true" ]; then
        tOutput="logger -s -t ${gpCmdName:-fComStackTrace} -p ${gpFacility:-user}.debug"
    else
        tOutput="cat"
    fi

    tSrc=${BASH_SOURCE[0]}
    tSrc=${tSrc##*/}
    tLine=${BASH_LINENO[0]}
    tName="${FUNCNAME[1]}()"
    echo "Stack trace at: ${tSrc}:${tLine} ${tName}" | $tOutput
    for ((i = 1; i < ${#FUNCNAME[@]} - 1; i++)); do
        tSrc=${BASH_SOURCE[$i + 1]}
        tSrc=${tSrc##*/}
        tLine=${BASH_LINENO[$i]}
        tName="${FUNCNAME[$i]}()"
        echo " $i: ${tSrc}:${tLine} ${tName}" | $tOutput
    done

    return 0
} # fComStackTrace

# --------------------
fComSelect()
{
    PS3="$1"
    local pDirList="$2"
    local pHelp="$3"
    local tSrc=${BASH_SOURCE##*/}
    local tResponse
    local tStatus=1
    local tMaxLoop=30
    export gResponse

    # Loop until a valid selection, or QUIT or OTHER
    select tResponse in QUIT HELP $pDirList; do
        let --tMaxLoop
        if [ $tMaxLoop -le 0 ]; then
            fLog -f -p warning -m "Too many responses." -l $tSrc:$LINENO
            tResponse="QUIT"
            break
        fi
        echo
        case $tResponse in
            QUIT)
                fLog -f -p warning -m "Quitting." -l $tSrc:$LINENO
                break
                ;;
            HELP)
                echo 1>&2
                echo "$pHelp" 1>&2
                echo 1>&2
                continue
                ;;
        esac
        if [ -n "$tResponse" ]; then
            tStatus=0
            break
        fi
        if [ -z "$tResponse" ]; then
            fLog -f -p warning -m "Invalid selection" -l $tSrc:$LINENO
            continue
        fi
    done

    PS3=""
    gResponse=$tResponse
    return $tStatus
} # fComSelect

# ------------------
fComYesNo()
{
    local pPrompt="$1"
    local tSrc=${BASH_SOURCE##*/}
    declare -l tResult

    if [ ${gpAuto:-0} -ne 0 ]; then
        if [ "$gpYesNo" = "Yes" ]; then
            gResponse="Yes"
            return 0
        fi
        if [ "$gpYesNo" = "No" ]; then
            gResponse="No"
            return 1
        fi
        return 1
    fi

    echo -n "$pPrompt [y/n]? "
    read -r tResult
    if [ "${tResult::1}" = "y" ]; then
        gResponse="Yes"
        return 0
    fi
    gResponse="No"
    return 1

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComYesNo pPrompt

If gpYesNo = "Yes", set gResponse to "y", and return 0.

If gpYesNo = "No", set gResponse to "n", and return 1.

If gpYesNo is not set, output the pPrompt, then "read" the response.

If the response begins with a "y", then that is the same as gpYesNo = "Yes"

If the response is not a "y", then that is the same as gpYesNo = "No"

=internal-cut
EOF
} # fComYesNo

# ------------------
fComFmtLog()
{
    local pLevel=$1
    local pMsg="$2"
    local pLine=$3
    local pErr=$4
    local pForce=$5
    local tSrc=${BASH_SOURCE##*/}
    local tDebugLevel
    local tLevel
    local tLine
    local tErr

    # Use this to turn PID on/off in logs (hard-coded)
    local tPID_Flag=0
    local tPID=""
    local tPID_Opt=""

    # Set any missing globals
    gpSysLog=${gpSysLog:-false}
    gpFacility=${gpFacility:-user}
    gpVerbose=${gpVerbose:-0}
    gpDebug=${gpDebug:-0}

    tLevel=$pLevel
    case $pLevel in
        emerg | emergency)
            pLevel=emerg
            tLevel=$pLevel
            ;;
        alert)
            :
            ;;
        crit | critical)
            pLevel=crit
            tLevel=$pLevel
            ;;
        err | error)
            pLevel=err
            tLevel=$pLevel
            ;;
        warning | warn)
            if [ $pForce -eq 0 ] && [ $gpVerbose -eq 0 ]; then
                return
            fi
            pLevel=warning
            tLevel=$pLevel
            ;;
        notice)
            if [ $pForce -eq 0 ] && [ $gpVerbose -lt 2 ]; then
                return
            fi
            ;;
        info)
            if [ $pForce -eq 0 ] && [ $gpVerbose -lt 3 ]; then
                return
            fi
            ;;
        debug | debug-*)
            tLevel=debug
            if [ $gpDebug -eq 0 ]; then
                return
            fi
            if [ $gpDebug -ne 0 ] && [ "${pLevel%%-*}" != "$pLevel" ]; then
                tDebugLevel=${pLevel##*-}
                if [ $tDebugLevel -gt $gpDebug ]; then
                    return
                fi
            fi
            ;;
        *)
            pMsg="Internal: (Unknown: $pLevel [$tSrc:$LINENO]) $pMsg"
            pLevel=err
            tLevel=$pLevel
            ;;
    esac

    # LineNo format
    tLine=""
    if [ -n "$pLine" ]; then
        tLine="[$pLine]"
    fi

    # Err format
    tErr=""
    if [ -n "$pErr" ]; then
        tErr="($pErr)"
    fi

    if [ $tPID_Flag -eq 1 ]; then
        tPID="[$tPID]"
        tPID_Opt="-i"
    fi

    # Output
    if [ "$gpSysLog" = "false" ]; then
        echo "${gpCmdName}$tPID $pLevel: $pMsg $tLine$tErr" 1>&2
    else
        logger -s $tPID_Opt -t $gpCmdName -p $gpFacility.$tLevel "$pLevel: $pMsg $tLine$tErr"
    fi
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComFmtLog pLevel "pMsg" pLine pErr

This function formats and outputs a consistent log message output.
See: fLog and fError.

=internal-cut
EOF
} # fComFmtLog

# ------------------
fLog()
{
    local pLevel="info"
    local pMsg=""
    local pLine=""
    local pErr=""
    local pInt=0
    local pForce=0
    local tSrc=${BASH_SOURCE##*/}
    local OPTARG=1
    local OPTIND
    local tArg

    while getopts :m:p:l:e:if tArg; do
        case $tArg in
            m) pMsg="${OPTARG}" ;;
            l) pLine="${OPTARG}" ;;
            p) pLevel="${OPTARG}" ;;
            e) pErr="${OPTARG}" ;;
            i) pInt=1 ;;
            f) pForce=1 ;;
            :) echo "Internal: Value required for option: -$OPTARG [$tSrc:$LINENO]" ;;
            \?) echo "Internal: Unknown option: $OPTARG [$tSrc:$LINENO]" ;;
        esac
    done
    shift $((OPTIND - 1))
    if [ $# -ne 0 ]; then
        shift $#
    fi

    if [ $pInt -eq 1 ]; then
        pMsg="Internal: $pMsg"
    fi

    fComFmtLog "$pLevel" "$pMsg" "$pLine" "$pErr" "$pForce"
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fLog -m pMsg [-p pLevel] [-l $LINENO] [-e pErr] [-f]

pLevel - emerg alert crit err warning notice info debug debug-N

 emerg, alert, crit, and err levels will always be output.

 emerg, alert, crit - exit gErr (or 1 if gErr=0, or not -e)
 err - return gErr (or 1 if gErr=0, or not -e)
 all other levels - return 0

 (fError defaults to "crit". -n changes it too "err")
 (-i adds "Internal: to message, an it calls fComStackTrace)

 -q   set gpVerbose=0
 -v   set gpVerbose=2 (default)
 -V N set gpVerbose=N (0..4)
 gitproj.config.verbose=N set gpVerbose=N

 if gpVerbose = 0, don't output warning, notice or info
 if gpVerbose = 1, don't output notice, info
 if gpVerbose = 2, don't output info (default verbose level)
 if gpVerbose >= 3, output all non-debug levels
 if -f, ignore the gpVerbose setting

 -x - set gpDebug=1
 -xx - set gpDebug=2
 -X N - set gpDebug=N (0..100)

 if gpDebug = 0, don't output andy debug msgs
 if gpDebug > 0, output any "debug" msgs, or
 if gpDebug = N1, output "debug-N2" msgs, if N1 >= N2

See Globals: gpSysLog, gpFacility, gpVerbose, gpDebug

=internal-head4 fLog Examples:

 local tFile=${BASH_SOURCE##*/}
 local tFile=file.inc
 gpCmdName=program.sh

 fError -m "Message" -l $tFile:$LINENO -e 8

    Output: program.sh crit: Error: Message [file.inc:234](8)
    gErr=8
    exit 8

 fError -n -m "Message" -l $tFile:$LINENO -e 8

    Output: program.sh err: Error: Message [file.inc:234](8)
    gErr=8
    return 8

 fError -i [-n] -m "Message" -l $tFile:$LINENO -e 8

    Output: program.sh err: Internal: Error: Message [file.inc:234](8)
    gErr=8
    call: fComStackTrace
    return 8, if -n
    exit 8, if ! -n

 fLog -p err -m "Message" -l $tFile:$LINENO -e 8

    Output: program.sh err: Message [file.inc:234](8)
    gErr=8
    return 8

 fLog -p warning -m "Output if gpVerbose" -e 8

    if gpVerbose >= 1
    Output: program.sh warning: Message (8)
    gErr=8
    return 0

 fLog -p notice -m "Notice msg"  -e 8 -l $LINENO

    if gpVerbose >= 2
    Output: program.sh notice: Notice msg [234]
    gErr=0
    return 0

 fLog -p info -m "Info message"

    if gpVerbose >= 3
    Output: program.sh info: Info message
    gErr=0
    return 0

 fLog -p debug -m "Output only if $gpDebug > 0" -l $LINENO

    Output: program.sh debug: Output only if $gpDebug > 0 [234]
    return 0

 fLog -p debug-3 -m "Output only if $gpDebug >= 3" -l $LINENO

    Output: program.sh debug-3: Output only if $gpDebug >= 3 [234]
    return 0

=internal-cut
EOF
} # fLog

# --------------------------------
fError()
{
    local pMsg="Error"
    local pLine=""
    local pErr=1
    local pInt=""
    local pNoExit=0
    local tSrc=${BASH_SOURCE##*/}
    local tLevel=crit
    local OPTIND=1
    local OPTARG
    local tArg

    while getopts :m:l:e:in tArg; do
        case $tArg in
            m) pMsg="${OPTARG}" ;;
            l) pLine="${OPTARG}" ;;
            e) pErr="${OPTARG}" ;;
            i) pInt="-i" ;;
            n)
                pNoExit=1
                tLevel=err
                ;;
            :)
                echo "Internal: Value required for option: -$OPTARG [$tSrc:$LINENO]"
                fComStackTrace
                ;;
            \?)
                echo "Internal: Unknown option: $OPTARG [$tSrc:$LINENO]"
                fComStackTrace
                ;;
        esac
    done
    shift $((OPTIND - 1))
    if [ $# -ne 0 ]; then
        pMsg="$pMsg $*"
    fi
    if [ $# -ne 0 ]; then
        shift $#
    fi

    pMsg="Error: $pMsg"
    fLog -p $tLevel $pInt -m "$pMsg" -l $pLine -e $pErr

    if [ -n "$pInt" ] || [ $gpDebug -ge 2 ]; then
        fComStackTrace
    fi

    if [ -z "$pInt" ] && [ $pNoExit -eq 0 ]; then
        fUsage usage
        exit 1
    fi

    if [ $pNoExit -eq 0 ]; then
        exit 1
    fi
    return $gErr

    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fError -m pMsg [-l $LINENO] [-e pErr] [-i]

This will call: fLog -p crit -m "pMsg" -l pLine -e pErr

If no -i, then "fUsage short", will be called.

=internal-cut
EOF
} # fError

# --------------------
fComGit()
{
    local tSrc=${BASH_SOURCE##*/}
    local tOpt
    local tStatus

    tOpt=$(echo "$@" | sed 's/-/\-/g; s/"/\\\"/g')

    fLog -p info -l $tSrc:$LINENO -m "git $tOpt"
    git "$@"
    tStatus=$?
    if [ $tStatus -ne 0 ]; then
        fLog -p warning -m "Fix this then try again." -l $tSrc:$LINENO
    fi
    return $tStatus
    cat <<EOF
=internal-pod

=internal-for html <hr/>

=internal-head3 fComGit [pArgs...]

TBD: replace this with set/try/catch

This is a wrapper for "git". If gpVerbose is >= 2, the git call will
will be echoed. If the git call exits with a non-0, then an error
message is output and the script exits (if not called in a sub-shell).

This is used to make the code look "clean". You don't need to clutter
the code with if statements with every git command. I.e. this supports
the "design pattern" where, if a command exits, it is assumed all is
OK, because if it failed, the script would exit.

=internal-cut
EOF
} # fComGit

# --------------------------------
fComSetConfig()
{
    local pScope=""
    local pType=""
    local pKey=""
    local pValue=""
    local pAdd=""
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir
    local OPTIND=1
    local OPTARG
    local tArg
    while getopts :abf:gik:lLv: tArg; do
        # shellcheck disable=SC2213,SC2220
        case $tArg in
            f)
                if [ ! -f ${OPTARG} ]; then
                    fLog -p warning -m "Could not find ${OPTARG}, it will be created" -l "$tSrc:$LINENO"
                    touch ${OPTARG}
                    if [ ! -f ${OPTARG} ]; then
                        fError -n -m "Could not create ${OPTARG} in $PWD" -l "$tSrc:$LINENO"
                        return 1
                    fi
                fi
                ;;
            l)
                if ! fComMustBeInGitRepo $PWD; then
                    return 1
                fi
                ;;
            L)
                if ! fComMustBeInProjRepo $PWD; then
                    return 1
                fi
                tTopDir="$($cGetTopDir 2>&1)"
                ;;
        esac
        case $tArg in
            a) pAdd="--add" ;;
            b) pType="--bool" ;;
            f) pScope="--file ${OPTARG}" ;;
            g) pScope="--global" ;;
            i) pType="--int" ;;
            k) pKey="${OPTARG}" ;;
            l) pScope="--local" ;;
            L) pScope="--file $tTopDir/.gitproj" ;;
            v) pValue="${OPTARG}" ;;
            :)
                fError -n -i -m "fComSetConfig: Value required for option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
            \?)
                fError -n -i -m "fComSetConfig:: Unknown option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
        esac
    done
    if [ -z "$pKey" ]; then
        fError -n -i -m "fComSetConfig: missing key" -l $tSrc:$LINENO
        return 1
    fi
    if [ -z "$pValue" ]; then
        fError -n -i -m "fComSetConfig: missing value" -l $tSrc:$LINENO
        return 1
    fi

    git config $pScope $pAdd $pType $pKey "$pValue"
    return $?

    # ----------------------
    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComGetConfig

=internal-head4 Synopsis

    fComSetConfig -k pKey -v pValue [-g|-l|-L|-f pFile] [-b|-i] [-a]

=internal-head4 Description

=internal-head4 Options

Only one of these will be used: -g, -l, -L, -f. The last one in the
list of options will be used if there is more than one.  If none, then
-l is the default.

Only one or none of these can be specified: -b, -i. If more than one,
then the last one will be used.

=internal-over 4

=internal-item B<-k pKey>

This option is required.

=internal-item B<-v pValue>

This option is required.

=internal-item B<-g>

Write to ~/.gitconfig (--global)

=internal-item B<-l>

You must be in a git workspace for this option to work. (--local)

Write to GIT_DIR/.git/config

=internal-item B<-L>

Write to GIT_DIR/.gitproj

=internal-item B<-f pFile>

Write to pFile.

=internal-item B<-b>

The value will be normalized to "true" or "false".

=internal-item B<-i>

The value will be converted to a single "byte" value, based on
its suffix.

Valid suffixes: k, K, m, M, g, G

These will be converted to lowercase before being sent to "git config".

=internal-item B<-a>

Use this option if you need to specify a key with multiple values.
If not specified, then any matching key in the file, being written to,
will replace the value.

=internal-back

=internal-cut
EOF
} # fComSetConfig

# --------------------------------
fComGetConfig()
{
    local pScope=""
    local pType=""
    local pKey=""
    local pDefault=""
    local pFilter=""
    local pExitOnError=0
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir
    local tValue=""
    local tExit="-n"

    # Globals set by fComGetConfig
    gGetConfigValue=""
    gGetConfigOrigin="not-found"

    local OPTIND=1
    local OPTARG
    local tArg
    while getopts :bd:ef:gik:lLv: tArg; do
        # shellcheck disable=SC2213,SC2220
        case $tArg in
            f)
                if [ ! -f ${OPTARG} ]; then
                    fError $tExit -m "Could not find ${OPTARG}" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            l)
                if ! fComMustBeInGitRepo $PWD; then
                    return 1
                fi
                ;;
            L)
                if ! fComMustBeInProjRepo $PWD; then
                    return 1
                fi
                tTopDir="$($cGetTopDir 2>&1)"
                ;;
        esac
        case $tArg in
            b) pType="--bool" ;;
            d) pDefault="${OPTARG}" ;;
            e)
                pExitOnError=1
                tExit=""
                ;;
            f) pScope="--file ${OPTARG}" ;;
            g) pScope="--global" ;;
            i) pType="--int" ;;
            k) pKey="${OPTARG}" ;;
            l) pScope="--local" ;;
            L) pScope="--file $tTopDir/.gitproj" ;;
            v) pFilter="${OPTARG}" ;;
            :)
                fError $tExit -i -m "fComGetConfig: Value required for option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
            \?)
                fError $tExit -i -m "fComGetConfig: Unknown option: $OPTARG" -l $tSrc:LINENO
                return 1
                ;;
        esac
    done
    if [ -z "$pKey" ]; then
        fError $tExit -i -m "fComGetConfig: missing key" -l $tSrc:$LINENO
        return 1
    fi

    gGetConfigValue=$(git config --get --includes $pScope $pType $pKey $pFilter)

    if [ $pExitOnError -ne 0 ]; then
        if [ -z "$gGetConfigValue" ]; then
            fError -m "Unexpected: $pKey is not defined!" -l $tSrc:$LINENO
        fi

        if [ -n "$pDefault" ] && [ "$gGetConfigValue" = "$pDefault" ]; then
            fError -m "Unexpected: $pKey should not be set to: $gGetConfigValue" -l $tSrc:$LINENO
        fi

        if [ "$gGetConfigValue" = "TBD" ] || [ "$gGetConfigValue" = "not-defined" ]; then
            fError -m "Unexpected: $pKey should not be set to: $gGetConfigValue" -l $tSrc:$LINENO
        fi
    fi

    if [ -z "$gGetConfigValue" ] && [ -n "$pDefault" ]; then
        gGetConfigOrigin=default
        gGetConfigValue=$pDefault
        echo "$gGetConfigValue"
        return 0
    fi

    if [ -z "$gGetConfigValue" ]; then
        gGetConfigOrigin=not-found
        echo ""
        return 0
    fi

    gGetConfigOrigin=$(git config --show-origin --get --includes $pScope $pType $pKey $pFilter)
    gGetConfigOrigin=${gGetConfigOrigin% *}
    echo "$gGetConfigValue"
    return 0

    # ----------------------
    cat <<EOF >/dev/null
=internal-pod

=internal-for html <hr/>

=internal-head3 fComGetConfig

=internal-head4 Synopsis

    fComGetConfig -k pKey [-g|-l|-L|-f pFile] [-b|-i]
                  [-d pDefault] [-e] [-v pFilter]

=internal-head4 Description

If pKey is found, the last defined value is echoed. And the value is
saved in global gGetConfigValue. And the file location of the value is
saved in gGetConfigOrigin.

If the key is not found, and -d pDefault is set then gGetConfigValue
is set to pDefault, and gGetConfigOrigin is set to "default".

If the key is not found, and there is no -d option, then
gGetConfigValue is set to "", and gGetConfigOrigin is set to
"not-found", and "" is returned.

Command Returns

    0 - OK (even for no value found)
    1 - Errors

=internal-head4 Options

Only one of these will be used: -g, -l, -L, -f. The last one in the list
of options will be if there is more than one.  If none, then all the
config files will be used.

Only one or none of these can be specified: -b, -i. If more than one,
then the last one will be used.

=internal-over 4

=internal-item B<-k pKey>

This option is required. The last file were this key is found will be
used to set the value returned.

=internal-item B<-g>

Look in ~/.gitconfig Includes are followed so ~/gitproj.config.global
will be included.

=internal-item B<-l>

You must be in a git workspace for this option to work.

Look in GIT_DIR/.git/config Includes are followed so these files will
be included: GIT_DIR/.gitproj and
GIT_DIR/.git/config

=internal-item B<-f pFile>

This will look in pFile for pKey. Includes will be followed.

=internal-item B<-b>

The value will be normalized to "true" or "false".

Valid values in the files:

=internal-item B<-i>

The value will be converted to a single "byte" value, based on
its suffix.

Valid suffixes: k, m, g

Note: fComSetConf with -i will allow: [kKmMgG]. It will convert the
values to lower-case, when saving them in the config files.

=internal-item B<-d pDefault>

If pKey is not found, then pDefault will be returned, and gGetConfigOrigin
will be set to "default".

=internal-item B<-e>

If -e, then do some simple validation checks:

* if the pKey is not found, then fError will be called and the script
will terminate. I.e. the config var is required.

* or if the value for pKey is equal to pDefault, fError will be called
and the script will terminate. I.e. the config var is still set to
it's intitial "undefined" value.

* or if the value for pKey is equal to "TBD" or "not-defined", fError
will be called and the script will terminate. I.e. the config var is
still set to it's intitial "undefined" value.

Note: -e will only exit the whole script if it is not in a subshell.
I.e. don't use var=\$(cmd) style.

=internal-item B<-v pFilter>

pFilter is used as a pattern match for the value found. This is useful
for times when there could be duplicate pKeys with different
values. For example there could be multiple "include.path" keys.

=internal-back

=internal-cut
EOF
} # fComGetConfig

# --------------------------------
fComUnsetConfig()
{
    local pScope=""
    local pKey=""
    local pAll="--unset"
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir
    local OPTIND=1
    local OPTARG
    local tArg

    while getopts :af:gk:lL tArg; do
        # shellcheck disable=SC2213,SC2220
        case $tArg in
            f)
                if [ ! -f ${OPTARG} ]; then
                    fError -n -m "Could not find ${OPTARG}" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            l)
                if ! fComMustBeInGitRepo $PWD; then
                    return 1
                fi
                ;;
            L)
                if ! fComMustBeInProjRepo $PWD; then
                    return 1
                fi
                tTopDir="$($cGetTopDir 2>&1)"
                ;;
        esac
        case $tArg in
            a) pAll="--unset-all" ;;
            f) pScope="--file ${OPTARG}" ;;
            g) pScope=--global ;;
            k) pKey="${OPTARG}" ;;
            l) pScope=--local ;;
            L) pScope="--file $tTopDir/.gitproj" ;;
            :)
                fError -n -i -m "fComUnsetConfig: Value required for option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
            \?)
                fError -n -i -m "ComUnsetConfig: Unknown option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
        esac
    done

    if [ -z "$pKey" ]; then
        fError -n -i -m "fComUnsetConfig: missing key" -l $tSrc:$LINENO
        return 1
    fi

    git config $pAll $pScope $pKey
} # fComUnsetConfig

# --------------------
fComSaveVar2Config()
{
    local pFile="$1"
    local pKey="$2"
    local pVarName="$3"
    local tSrc=${BASH_SOURCE##*/}
    local tValue

    tValue=$(eval echo \$$pVarName)
    if [ -z "$tValue" ]; then
        tValue="Undefined"
    fi
    # TBD: Use fComSetConfig? because it has more error handling.
    ##fComSetConfig -f $pFile -k $pKey -v "$tValue"
    git config --replace-all --file $pFile $pKey "$tValue"
    if [ $? -ne 0 ]; then
        fLog -i -p warning -m "There was problem saving $pVarName value to $pKey to file $pFile" -l $tSrc:$LINENO
        gErr=1
    fi
} # fComSaveVar2Config

fComCheckPkg()
{
    local pPkg=$1
    local tSrc=${BASH_SOURCE##*/}
    local tStatus

    dpkg -l $pPkg 2>/dev/null | grep -q "^ii *$pPkg"
    tStatus=$?
    fLog -p debug-2 -m "fComCheckPkg $pPkg $tStatus" -l $tSrc:$LINENO
    return $tStatus
} # fComCheckPkg

# --------------------
fComMustBeInGitRepo()
{
    local pDir=$1
    local tSrc=${BASH_SOURCE##*/}

    if [ ! -d $pDir ]; then
        fError -n -m "$pDir does not exist." -l $tSrc:$LINENO
        return 1
    fi

    cd $pDir >/dev/null 2>&1
    if [ -d $PWD/.git ]; then
        cd - >/dev/null 2>&1
        return 0
    fi
    if $cGetTopDir >/dev/null 2>&1; then
        cd - >/dev/null 2>&1
        return 0
    fi
    cd - >/dev/null 2>&1
    fError -n -m "You must be in a git workspace for this command." -l $tSrc:$LINENO
    return 1
} # fComMustBeInGitRepo

# --------------------
fComMustNotBeInGit()
{
    local pDir=$1
    local tSrc=${BASH_SOURCE##*/}
    local tFound=0
    local tGitDir

    if [ ! -d $pDir ]; then
        fError -n -m "$pDir does not exist." -l $tSrc:$LINENO
        return 1
    fi
    cd $pDir >/dev/null 2>&1
    if $cGetTopDir >/dev/null 2>&1; then
        git branch
        tFound=1
    fi
    #    if [ $tFound -eq 0 ]; then
    #        tGitDir=$(find . -type d -name .git)
    #        if [ -n "$tGitDir" ]; then
    #            tFound=2
    #        fi
    #    fi
    cd - >/dev/null 2>&1
    if [ $tFound -ne 0 ]; then
        fError -n -m "$pDir must NOT be in a git repo" -l $tSrc:$LINENO
        return 1
    fi
    return 0
} # fComMustNotBeInGit

# --------------------
fComMustBeInProjRepo()
{
    local pDir=$1
    local tSrc=${BASH_SOURCE##*/}
    local tTopDir

    if ! fComMustBeInGitRepo $pDir; then
        return 1
    fi

    if [ ! -f $HOME/.gitconfig ]; then
        fError -n -m "Missing ~/.gitconfig, run 'git proj init'" -l $tSrc:$LINENO
        return 1
    fi

    tTopDir=$($cGetTopDir)

    if [ ! -f $tTopDir/.gitproj ]; then
        fError -n -m "This git workspace is not setup for gitproj." -l $tSrc:$LINENO
        if [ $gpDebug -ge 2 ]; then
            fComStackTrace
        fi
        return 1
    fi

    return 0
} # fComMustBeInProjRepo

fComAllMustBeReadable()
{
    local pDir=${1:-$PWD}
    local tSrc=${BASH_SOURCE##*/}
    local tFound

    tFound=$(find $pDir -type d \! -executable -print 2>/dev/null)
    if [ -n "$tFound" ]; then
        fError -n -m "All directories must be executable, under $pDir" -l $tSrc:$LINENO
        return 1
    fi

    tFound=$(find $pDir \! -readable -print 2>/dev/null)
    if [ -n "$tFound" ]; then
        fError -n -m "All directories and files must be readable, under $pDir" -l $tSrc:$LINENO
        return 1
    fi

    return 0
} # fComAllMustBeReadable

# --------------------
fComIsGitRemoteAvailable()
{
    # TBD not used yet
    local pLevel="${1:-debug}"
    local tSrc=${BASH_SOURCE##*/}
    local tOrigin

    tOrigin=$(fComGetConfig -k "remote.origin.url" -d "TBD")
    if [ "$tOrigin" = "TBD" ]; then
        fLog -p $pLevel -m "remote.origin.url is not defined." -l $tSrc:$LINENO
        return 1
    fi
    if git ls-remote $tOrigin >/dev/null 2>&1; then
        return 0
    fi
    fLog -p $pLevel -m "$tOrigin was not found. Try again after mounting it or run 'git proj remote' to change the remote.raw.dir location." -l $tSrc:$LINENO
    return 1
} # fComIsGitRemoteAvailable

# --------------------
fComIsRawRemoteAvailable()
{
    # TBD not used yet
    local pLevel="${1:-debug}"
    local tSrc=${BASH_SOURCE##*/}

    gpRemoteRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-origin" -d "TBD")
    if [ "$gpRemoteRawOrigin" = "TBD" ]; then
        fLog -p $pLevel -m "gitproj.config.remote-raw-origin is not defined." -l $tSrc:$LINENO
        return 1
    fi
    # TBD: change this to use rclone
    if [ -d $gpRemoteRawOrigin ]; then
        return 0
    fi
    fLog -p $pLevel -m "$gpRemoteRawOrigin was not found. Try again after mounting it or run 'git proj remote' to change the remote.raw.dir location." -l $tSrc:$LINENO
    return 1
} # fComIsRawRemoteAvailable

# --------------------
fComIsRemoteMounted()
{
    local pLevel="${1:-debug}"
    local tSrc=${BASH_SOURCE##*/}

    # Assumes we are in a gitproj managed dir
    # And this assumes remotes are local disks

    gpRemoteRawOrigin=$(fComGetConfig -k "gitproj.config.remote-raw-origin" -d "TBD")
    if [ "$gpRemoteRawOrigin" = "TBD" ]; then
        fLog -p $pLevel -m "gitproj.config.remote-raw-origin is not defined." -l $tSrc:$LINENO
    fi
    if [ -d $gpRemoteRawOrigin ]; then
        return 0
    fi
    fLog -p $pLevel -m "$gpRemoteRawOrigin was not found. Try again after mounting it or run 'git proj remote' to change the remote.raw.dir location." -l $tSrc:$LINENO
    return 1
} # fComIsRemoteMounted

# --------------------
fComGetProjGlobals()
{
    local tSrc=${BASH_SOURCE##*/}

    # Assumes we in git dir, and one that is managed by gitproj.
    if ! fComMustBeInProjRepo $PWD; then
        fUsage usage
        exit 1
    fi

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "false" 2>&1)
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)
    gpLocalTopDir=$($cGetTopDir 2>/dev/null)

    # Called this way, so that the -e option will exit script if errors
    fComGetConfig -k "gitproj.config.proj-name" -d "TBD" -e >/dev/null
    gpProjName=$gGetConfigValue

    fComGetConfig -k "gitproj.config.remote-raw-origin" -d "TBD" -e >/dev/null
    gpRemoteRawOrigin=$gGetConfigValue
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648" 2>&1) # 2g

    fComGetConfig -k "gitproj.config.local-status" -d "not-defined" -e >/dev/null
    gpLocalStatus=$gGetConfigValue

    fComGetConfig -k "gitproj.config.remote-status" -d "not-defined" -e >/dev/null
    gpRemoteStatus=$gGetConfigValue

    fComGetVer

    return 0
} # fComGetProjGlobals

fComGetVer()
{
    local tSrc=${BASH_SOURCE##*/}
    local tInsVer
    local tInsMajor
    local tInsMinor
    local tLocVer
    local tLocMajor
    local tLocMinor
    local tTrim

    if ! fComMustBeInProjRepo $PWD; then
        fError -m "Quitting" -l $tSrc:$LINENO
    fi

    # Get the expected Version for the "current" project
    tLocVer=$(fComGetConfig -k "gitproj.config.ver")
    if [ -z "$tLocVer" ]; then
        gpVer=$cGitProjVersion
        fLog -i -p warning -m "gitproj.config.ver was not found. Setting it to: $gpVer" -l "$tSrc:$LINENO"
        fComSetConfig -L -k "gitproj.config.ver" -v "$gpVer"
        fComSetConfig -l -k "gitproj.config.ver" -v "$gpVer"
    fi
    gpVer=$tLocVer

    if [ "$gpVer" = "$cGitProjVersion" ]; then
        return 0
    fi

    # Prep: tInsVer. Only the first two # are important
    tTrim=${cGitProjVersion#*.*.}
    tInsVer=${cGitProjVersion%.$tTrim}
    tInsMajor=${tInsVer%.*}
    tInsMinor=${tInsVer#*.}

    # Prep: tLocVer. Only the first two # are important
    tTrim=${gpVer#*.*.}
    tLocVer=${gpVer%.$tTrim}
    tLocMajor=${tLocVer%.*}
    tLocMinor=${tLocVer#*.}

    if [ $tLocMajor -gt $tInsMajor ]; then
        fError -n -m "Expected version $tLocVer The installed version $cGitProjVersion needs to be upgraded to $tLocVer or greater." -l $tSrc:$LINENO
        return 1
    fi

    if [ $tLocMajor -lt $tInsMajor ]; then
        fError -n -m "Expected version $tLocVer  The project needs to be upgraded to the installed version $cGitProjVersion" -l $tSrc:$LINENO
        return 1
    fi

    if [ $tLocMinor -ne $tInsMinor ]; then
        fLog -p warning -m "Minor version difference. Expected version $tLocVer, but installed version is $tInsVer" -l $tSrc:$LINENO
        return 0
    fi

    return 0
} # fComGetVer

fComFmt()
{
    # Call fmt, setting the width based on the the terminal COLUMNS
    # width.  Set to COLUMNS - 5, but no less than 30 and no more than
    # 130 characters wide.

    local tWidth

    if [ -z "$COLUMNS" ]; then
        COLUMNS=80
    fi
    let tWidth=COLUMNS-5
    if [ $tWidth -lt 30 ]; then
        tWidth=30
    fi
    if [ $tWidth -gt 130 ]; then
        tWidth=130
    fi
    fmt -w $tWidth
} # fComFmt

# ========================================
fComSetGlobals
