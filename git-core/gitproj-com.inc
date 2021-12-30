#!/bin/bash

# --------------------------------
export cConfigGlobal cConfigLocal cConfigHost cCurDir cGitProjVersion \
    cGetOrigin cGetTopDir cGetCurBranch cPID
export gErr gpBin gpCmdName gpDebug gpDoc gpFacility \
    gpLocalRawDir gpSysLog gpVerbose gpYesNo PWD \
    gGetConfigValue gGetConfigOrigin gResponse

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

=internal-head2 Template Use

=internal-head3 Configuration

 * Copy template.sh to your script file.
 * Your script, gitproj-com.inc, and gitproj-com.test need to be in the same directory.
 * Globally replace SCRIPTNAME with the name of your script file.
 * Update the getopts in the "Get Args Section". Add your script's options.
 * Loop: document (with POD), add tests, add validate functions
 * Loop: add unit test function, add functions, test

=internal-head3 Block Organization

 * Configuration - exit if errors
 * Get Args - exit if errors
 * Verify external progs - exit if errors
 * Run tests - if gpTest is set
 * Validate Args - exit if errors
 * Verify connections work - exit if errors
 * Read-only functional work - exit if errors
 * Write functional work - now you are committed! Try to keep going if errors
 * Output results and/or launch next process

To avoid a lot of rework and manual rollbacks, put-off I<writes> that
cannot undone. Do as much as possible to make sure the script will be able
to complete write operations.

For example, B<do not do this:> collect information, transform it,
write it to a DB, then start the next process on another server.
Whoops, that server cannot be accessed, so the DB update is not valid!
Gee, why didn't you verify all the connections you will need, before
committing to the DB?!  Even if you did check, the connection could
have failed after the check, so maybe write to a tmp DB, then when all
is OK, then update the master DB with the tmp DB changes.

Where ever possible make your scripts "re-entrant". Connections can
fail at anytime and scripts can be killed at anytime; How can any
important work be continued or work rolled-back? Planing for
"failures" is NOT planning to fail; it is what a professional engineer
does to design in quality.

=internal-head3 Variable Naming Convention

Prefix codes are used to show the B<"scope"> of variables:

 gVar - global variable (may even be external to the script)
 pVar - a function parameter I<local>
 gpVar - global parameter, i.e. may be defined external to the script
 cVar - global constant (set once)
 tVar - temporary variable (usually I<local> to a function)
 fFun - function

All UPPERCASE variables are I<only> used when they are required by other
programs or scripts.

If you have exported variables that are shared across scritps, then
this convention can be extended by using prefixes that are related to
where the variables are set.

=internal-head3 Global Variables

For more help, see the Globals section in fUsage.

 gpSysLog - -l
 gpVerbose - -v, -vv
 gpDebug - -x, -xx, ...
 gpTest - -t
 gErr - error code (0 = no error)
 gpCmdName - script's name taken from $0
 cCurDir - current directory
 gpBin - directory where the script is executing from
 gpDoc - directory for config/, hooks/, test/

=internal-head3 Documentation Format

POD is use to format the script's documentation. Sure MarkDown could
have been used, but it didn't exist 20 years ago. POD text can be
output as text, man, html, pdf, texi, just usage, and even MarkDown

Help for POD can be found at:
L<perlpod - the Plain Old Documentation format|https://perldoc.perl.org/perlpod>

The documentation is embedded in the script so that it is more likely
to be updated. Separate doc files seem to I<always> drift from the
code. Feel free to delete any documentation, if the code is clear
enough.  BUT I<clean up your code> so that the code I<really> is
clear.

The internal documentation uses POD commands that begin with "=internal-".
See fComInternalDoc() for how this is used.

Also TDD (Test Driven Development) should make refactoring easy,
because the tests are also embedded in the script.

=internal-head2 Common Script Functions

=internal-cut

EOF
} # fGitProjComInternalDoc

# ========================================
# Common Functions

# --------------------
fComIntroText()
{
    local tSrc=${BASH_SOURCE##*/}

    cat <<EOF | fComFmt

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

For a more complete overview type: "git proj -h | more"

For a quick list of the git proj subcommands just type: "git proj"

All subcommands will output help text with the -h option. For example:
"git proj SubCmd -h | more"

[$tSrc:$LINENO]
EOF
    return 0
} # fComIntroText

# --------------------
fComFirstTimeSet()
{
    local tSrc=${BASH_SOURCE##*/}
    local tInclude
    local tUserName
    local tUserEmail

    if [ ! -f $HOME/.gitconfig ]; then
        fLog -p info -m "Create: $HOME/.gitconig" -l $tSrc:$LINENO
        cp $gpDoc/config/gitconfig.default $HOME/.gitconfig
    fi

    tUserName=$(fComGetConfig -g -k "user.name")
    tUserEmail=$(fComGetConfig -g -k "user.email")
    if [ -z "$tUserName" ] || [ -z "$tUserEmail" ]; then
        fError -m "user.name and user.email must be in your ~/.gitconfig file. See $cgDoc/config/gitconfig.default for some other useful settings." -l $tSrc:$LINENO
    fi

    if [ ! -f $HOME/cConfigGlobal ]; then
        fLog -p info -m "Create: ~/$cConfigGlobal" -l $tSrc:$LINENO
        cp $gpDoc/config/gitproj.config.global $HOME/$cConfigGlobal
    fi
    tInclude=$(fComGetConfig -g -k "include.path" -v $cConfigGlobal)
    if [ -z "$tInclude" ]; then
        fLog -p info -m "include.path=$cConfigGlobal" -l $tSrc:$LINENO
        fComSetConfig -a -g -k "include.path" -v $cConfigGlobal
    fi
} # fComFirstTimeSet

# -------------------
fComSetGlobals()
{
    local tSrc=${BASH_SOURCE##*/}

    # -------------------
    # Set current directory location in PWD and cCurDir, because with cron
    # jobs PWD is not set.
    if [ -z "$PWD" ]; then
        PWD=$(pwd)
    fi
    if [ -z "$cCurDir" ]; then
        cCurDir=$PWD
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

    if [ -z "$gpDoc" ]; then
        gpDoc=/usr/share/doc/gitproj
    fi
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

    # Included in ~/.gitproj.config (Use -G to set config vars to this)
    # Note: ~/$cConfigGlobal does not work all the time, so use:
    # $HOME/$cConfigGlobal  However ~ is OK for use in literals.
    # shellcheck disable=SC2088
    cConfigGlobal='.gitproj.config.global'

    # This is put in cTop, project dir. (Use -L to set config vars to this)
    cConfigLocal='.gitproj.config.local'

    # This is put in cTop, project dir. (Use -H to set config vars to this)
    # This is included by .git/config
    cConfigHost=".gitproj.config.$HOSTNAME"

    # cConfigGlobal, cConfigLocal, and $cConfigLocal should be
    # versioned.

    # Use:
    #     tOrigin=$($cGetOrigin)
    #     if $cGetOrigin >/dev/null 2>&1; then echo "In a git dir"; fi
    cGetOrigin='git config --get remote.origin.url'

    # Use:
    #     tTop=$($cGetTopDir); cd $tTop
    cGetTopDir='git rev-parse --show-toplevel'

    # Use:
    #     tBranch=$($cGetCurBranch)
    cGetCurBranch='git rev-parse --abbrev-ref HEAD'

    # -------------------
    # Setup log variables
    # -x
    gpDebug=${gpDebug:-0}
    # -v
    gpVerbose=${gpVerbose:-0}
    # -l
    gpSysLog=${gpSysLog:-true}
    gpFacility=${gpFacility:-user}
    gErr=0

    return

    cat <<EOF >/dev/null
=internal-pod

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

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true")
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)

    gpAuto=0
    gpLocalTopDir=$PWD
    gpProjName=$(fComGetConfig -k "gitproj.config.name" -d "TBD")
    if [ "$gpProjName" = "TBD" ]; then
        gpProjName="${gpLocalTopDir##*/}"
    fi
    
    gpLocalRawDir=$gpLocalTopDir/raw

    gpMaxSize=$(fComGetConfig -k "gitproj.hook.binary-file-size-limit" -d "10k")
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648") #20g

    gpAutoMove="true"
    gpGitFlow="false"
    gpGitFlowPkg=$(fComGetConfig -k "gitproj.config.git-flow-pkg" -d "git-flow")

    gpProjStatus=$(fComGetConfig -k "gitproj.config.proj-status" -d "installed")
    gpLocalStatus=$(fComGetConfig -k "gitproj.config.local-status" -d "not-defined")
    gpRemoteStatus=$(fComGetConfig -k "gitproj.config.remote-status" -d "not-defined")

    gpCheckFileNames=true
    gpCheckForBigFiles=true
    gpPreCommitEnabled=true
    gpHookSource=hooks/pre-commit
    gpMountDir=TBD
    gpRemoteRawDir=TBD

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
    #    gpHookSource=hooks/pre-commit
    #    gpMountDir=TBD
    #    gpRemoteRawDir=TBD

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
    local tComReq='logger pod2text pod2usage'
    local tComOpt='pod2html pod2man pod2markdown tidy shunit2.1 gawk sed tr fmt'

    for tProg in $pOptional $tComOpt; do
        if ! which $tProg >/dev/null 2>&1; then
            echo "Optional: Missing $tProg"
            tErr=1
        fi
    done

    for tProg in $pRequired $tComReq; do
        if ! which $tProg >/dev/null 2>&1; then
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
    return

    cat <<EOF >/dev/null
=internal-pod

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
    local tTidy='tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no'
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
            fLog -p warning -m "Unknown style: $pStyle so usiing text" -l $tSrc:$LINENO
            ;;
    esac

    # Validate
    if ! which $tProg >/dev/null 2>&1; then
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
            cat $pFileList | $tProg --title="$pTitle" | $tTidy
        else
            if [ "$pStyle" = "short" ]; then
                # shellcheck disable=SC2002
                cat $pFileList | pod2text | head -n 10
            else
                # shellcheck disable=SC2002
                cat $pFileList | $tProg
            fi
        fi
    else
        if [ "$pStyle" = "html" ]; then
            # shellcheck disable=SC2002
            cat $pFileList | fComInternalDoc | $tProg --title="$pTitle" | $tTidy
        else
            # shellcheck disable=SC2002
            cat $pFileList | fComInternalDoc | $tProg
        fi
    fi

    gErr=1
    exit 1

    cat <<EOF >/dev/null
=internal-pod

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

    if [ "${gpSysLog:-true}" = "true" ]; then
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
            fLog -p warning -m "Too many responses." -l $tSrc:$LINENO
            tResponse="QUIT"
            break
        fi
        echo
        case $tResponse in
            QUIT)
                fLog -p warning -m "Quitting." -l $tSrc:$LINENO
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
            fLog -p warning -m "Invalid selection" -l $tSrc:$LINENO
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

    if [ "$gpYesNo" = "Yes" ]; then
        gResponse="Yes"
        return 0
    fi
    if [ "$gpYesNo" = "No" ]; then
        gResponse="No"
        return 1
    fi

    echo -n "$pPrompt [y/n]? "
    read -r tResult
    tResult=${tResult%${tResult#[a-z]}}
    if [ "$tResult" = "y" ]; then
        gResponse="Yes"
        return 0
    fi
    gResponse="No"
    return 1
    cat <<EOF >/dev/null
=internal-pod

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
    local tSrc=${BASH_SOURCE##*/}
    local tDebugLevel
    local tLevel
    local tLine
    local tErr

    # Use this to turn PID on/off in logs
    local tPID_Flag=0
    local tPID=""
    local tPID_Opt=""

    # Set any missing globals
    gpSysLog=${gpSysLog:-true}
    gpFacility=${gpFacility:-user}
    gpVerbose=${gpVerbose:-0}
    gpDebug=${gpDebug:-0}

    case $pLevel in
        emerg | emergency)
            pLevel=emerg
            ;;
        alert)
            :
            ;;
        crit | critical)
            pLevel=crit
            ;;
        err | error)
            pLevel=err
            ;;
        warning | warn)
            pLevel=warning
            ;;
        notice | info | debug | debug-*)
            :
            ;;
        *)
            pMsg="Internal: (Unknown: $pLevel [$tSrc:$LINENO]) $pMsg"
            pLevel=err
            ;;
    esac
    tLevel=$pLevel

    # Check debug
    if [ $gpDebug -eq 0 ] && [ "${pLevel%-*}" = "debug" ]; then
        return
    fi
    if [ $gpDebug -ne 0 ] && [ "${pLevel%%-*}" != "$pLevel" ]; then
        tDebugLevel=${pLevel##*-}
        if [ $tDebugLevel -gt $gpDebug ]; then
            return
        fi
        tLevel=debug
    fi

    # Check verbose
    if [ $gpVerbose -eq 0 ] && ([ "$pLevel" = "notice" ] || [ "$pLevel" = "info" ]); then
        return
    fi
    if [ $gpVerbose -eq 1 ] && [ "$pLevel" = "info" ]; then
        return
    fi

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
    return

    cat <<EOF >/dev/null
=internal-pod

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
    local tSrc=${BASH_SOURCE##*/}
    local OPTARG=1
    local OPTIND
    local tArg

    while getopts :m:p:l:e:i tArg; do
        case $tArg in
            m) pMsg="${OPTARG}" ;;
            l) pLine="${OPTARG}" ;;
            p) pLevel="${OPTARG}" ;;
            e) pErr="${OPTARG}" ;;
            i) pInt=1 ;;
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

    fComFmtLog $pLevel "$pMsg" $pLine $pErr
    return

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 fLog -m pMsg [-p pLevel] [-l $LINENO] [-e pErr]

pLevel - emerg alert crit err warning notice info debug debug-N

 if gpVerbose = 0, don't output notice or info
 if gpVerbose = 1, don't output info
 if gpVerbose >= 2, output all logs

See Globals: gpSysLog, gpFacility, gpVerbose, gpDebug

=internal-head4 fLog Examples:

 fLog -p warning -m "Missing awk" -l $LINENO -e 8
 fLog -p notice -m "Output only if -v"  -e 8 -l $LINENO
 fLog -p info -m "Output only if -vv" -l $LINENO
 fLog -p debug -m "Output only if $gpDebug > 0" -l $LINENO
 fLog -p debug-3 -m "Output only if $gpDebug > 0 and $gpDebug <= 3" -l $LINENO

=internal-cut
EOF
} # fLog

# --------------------------------
fError()
{
    local pMsg="Error"
    local pLine=""
    local pErr=1
    local pInt=0
    local pNoExit=0
    local tSrc=${BASH_SOURCE##*/}
    local OPTIND=1
    local OPTARG
    local tArg

    while getopts :m:l:e:in tArg; do
        case $tArg in
            m) pMsg="${OPTARG}" ;;
            l) pLine="${OPTARG}" ;;
            e) pErr="${OPTARG}" ;;
            i) pInt=1 ;;
            n) pNoExit=1 ;;
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

    if [ $pInt -eq 1 ]; then
        pMsg="Internal: $pMsg"
    fi
    fLog -p crit -l $pLine -e $pErr -m "$pMsg"
    if [ $pInt -eq 1 ] || [ $gpDebug -ge 2 ]; then
        fComStackTrace
    fi
    if [ $pInt -eq 0 ] && [ $pNoExit -eq 0 ]; then
        fUsage usage
    fi

    if [ $pNoExit -eq 0 ]; then
        exit 1
    fi
    return 1
    cat <<EOF >/dev/null
=internal-pod

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

=internal-head3 fComGit [pArgs...]

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
fComMkHostConfig()
{
    local tSrc=${BASH_SOURCE##*/}
    local tTop

    tTop=$(git rev-parse --show-toplevel)
    mkdir -p $tTop/gitproj 2>&1
    touch $tTop/gitproj/config.$HOSTNAME
    git config --local include.path $tTop/gitproj/config.$HOSTNAME
    cd $tTop >/dev/null 2>&1
    git add gitproj/config.$HOSTNAME
} # fComMkHostConfig

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
    while getopts :abf:gGHik:lLv: tArg; do
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
            G)
                if [ ! -f $HOME/$cConfigGlobal ]; then
                    fError -n -m "Could not find ~/$cConfigGlobal. See 'git proj init'" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            H | l | L)
                tTopDir="$($cGetTopDir 2>&1)"
                if [ $? -ne 0 ]; then
                    fError -n -m "You are not in a git dir: $PWD" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
        esac
        case $tArg in
            a) pAdd="--add" ;;
            b) pType="--bool" ;;
            f) pScope="--file ${OPTARG}" ;;
            g) pScope="--global" ;;
            G) pScope="--file $HOME/$cConfigGlobal" ;;
            H) pScope="--file $tTopDir/$cConfigHost" ;;
            i) pType="--int" ;;
            k) pKey="${OPTARG}" ;;
            l) pScope="--local" ;;
            L) pScope="--file $tTopDir/$cConfigLocal" ;;
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

=internal-head2 fComGetConfig 

=internal-head3 Synopsis

    fComSetConfig -k pKey -v pValue [-g|-G-l|-L|-H|-f pFile] [-b|-i] [-a]

=internal-head3 Description

=internal-head3 Options

Only one of these will be used: -g, -l, -L, -H -f. The last one in the
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

Write to ~/.gitconfig

=internal-item B<-l>

You must be in a git workspace for this option to work.

Write to GIT_DIR/.git/config

=internal-item B<-L>

Write to GIT_DIR/$cConfigLocal

=internal-item B<-H>

Write to GIT_DIR/$cConfigHost

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
    local tValue=""
    local tExit="-n"

    # Globals set by fComGetConfig
    gGetConfigValue=""
    gGetConfigOrigin="not-found"

    local OPTIND=1
    local OPTARG
    local tArg
    while getopts :bd:ef:gGHik:lLv: tArg; do
        # shellcheck disable=SC2213,SC2220
        case $tArg in
            f)
                if [ ! -f ${OPTARG} ]; then
                    fError $tExit -m "Could not find ${OPTARG}" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            G)
                if [ ! -f $HOME/$cConfigGlobal ]; then
                    fError $tExit -m "Could not find ~/$cConfigGlobal. See 'git proj init'" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            H | l | L)
                tTopDir="$($cGetTopDir 2>&1)"
                if [ $? -ne 0 ]; then
                    fError $tExit -m "You are not in a git dir: $PWD" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
        esac
        case $tArg in
            b) pType="--bool" ;;
            d) pDefault="${OPTARG}" ;;
            e) pExitOnError=1
               tExit=""
               ;;
            f) pScope="--file ${OPTARG}" ;;
            g) pScope=--global ;;
            G) pScope="--file $HOME/$cConfigGlobal" ;;
            H) pScope="--file $tTopDir/$cConfigHost" ;;
            i) pType="--int" ;;
            k) pKey="${OPTARG}" ;;
            l) pScope=--local ;;
            L) pScope="--file $tTopDir/$cConfigLocal" ;;
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

=internal-head2 fComGetConfig 

=internal-head3 Synopsis

    fComGetConfig -k pKey [-g|-G-l|-L|-H|-f pFile] [-b|-i]
                  [-d pDefault] [-e] [-v pFilter]

=internal-head3 Description

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

=internal-head3 Options

Only one of these will be used: -g, -l, -f. The last one in the list
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
be included: GIT_DIR/$cConfigLocal and
GIT_DIR/$cConfigHost

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
    local OPTIND=1
    local OPTARG
    local tArg
    while getopts :af:gGHk:lL tArg; do
        # shellcheck disable=SC2213,SC2220
        case $tArg in
            f)
                if [ ! -f ${OPTARG} ]; then
                    fError -n -m "Could not find ${OPTARG}" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            G)
                if [ ! -f $HOME/$cConfigGlobal ]; then
                    fError -n -m "Could not find ~/$cConfigGlobal. See 'git proj init'" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            H)
                tTopDir="$($cGetTopDir 2>&1)"
                if [ $? -ne 0 ]; then
                    fError -n -m "You are not in a git dir: $PWD" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            l)
                tTopDir="$($cGetTopDir 2>&1)"
                if [ $? -ne 0 ]; then
                    fError -n -m "You are not in a git dir: $PWD" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
            L)
                tTopDir="$($cGetTopDir 2>&1)"
                if [ $? -ne 0 ]; then
                    fError -n -m "You are not in a git dir: $PWD" -l "$tSrc:$LINENO"
                    return 1
                fi
                ;;
        esac
        case $tArg in
            a) pAll="--unset-all" ;;
            f) pScope="--file ${OPTARG}" ;;
            g) pScope=--global ;;
            G) pScope="--file $HOME/$cConfigGlobal" ;;
            H) pScope="--file $tTopDir/$cConfigHost" ;;
            k) pKey="${OPTARG}" ;;
            l) pScope=--local ;;
            L) pScope="--file $tTopDir/$cConfigLocal" ;;
            :)
                fError -n -i -m "Internal fComUnsetConfig: Value required for option: $OPTARG" -l $tSrc:$LINENO
                return 1
                ;;
            \?)
                fError -n -i -m "Internal fComUnsetConfig: Unknown option: $OPTARG" -l $tSrc:$LINENO
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
    if [ $tFound -eq 0 ]; then
        tGitDir=$(find . -type d -name .git)
        if [ -n "$tGitDir" ]; then
            tFound=2
        fi
    fi
    cd - >/dev/null 2>&1
    if [ $tFound -ne 0 ]; then
        fError -n -m "$pDir must NOT be in a git repo, or a directory above a git repo" -l $tSrc:$LINENO
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

    if [ ! -f $HOME/$cConfigGlobal ]; then
        fError -n -m "Missing ~/$cConfigGlobal, run 'git proj init'" -l $tSrc:$LINENO
        if [ $gpDebug -ge 2 ]; then
            fComStackTrace
        fi
        return 1
    fi

    tTopDir=$($cGetTopDir)

    if [ ! -f $tTopDir/$cConfigLocal ]; then
        fError -n -m "This git workspace is not setup for gitproj, run 'git proj init'" -l $tSrc:$LINENO
        if [ $gpDebug -ge 2 ]; then
            fComStackTrace
        fi
        return 1
    fi

    if [ ! -f $tTopDir/$cConfigHost ]; then
        fError -n -m "Missing $cConfigHost, run 'git proj config'" -l $tSrc:$LINENO
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
fComIsRemoteMounted()
{
    local pWarn="$1"
    local tSrc=${BASH_SOURCE##*/}

    # Assumes we are in a gitproj managed dir

    gpRemoteRawDir=$(fComGetConfig -k "gitproj.config.remote-raw-dir" -d "None")
    if [ "$gpRemoteRawDir" = "None" ]; then
        fError-m "gitproj.config.remote-raw-dir is not defined." -l $tSrc:$LINENO
    fi
    if [ -d $gpRemoteRawDir ]; then
        return 0
    fi

    if [ -n "$pWarn" ]; then
        # Just return error code if not found
        return 1
    fi
    fError -m "$gpRemoteRawDir was not found. Try again after mounting it or run 'git proj config' to change the remote.raw.dir location." -l $tSrc:$LINENO
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

    gpSysLog=$(fComGetConfig -k "gitproj.config.syslog" -d "true" 2>&1)
    gpFacility=$(fComGetConfig -k "gitproj.config.facility" -d user)
    gpLocalTopDir=$($cGetTopDir 2>/dev/null)

    # Called this way, so that the -e option will exit script if errors
    fComGetConfig -k "gitproj.config.proj-name" -d "TBD" -e >/dev/null
    gpProjName=$gGetConfigValue

    fComGetConfig -k "gitproj.config.remote-raw-dir" -d "TBD" -e  >/dev/null
    gpRemoteRawDir=$gGetConfigValue
    gpRemoteMinSpace=$(fComGetConfig -i -k "gitproj.config.remote-min-space" -d "2147483648" 2>&1)  # 2g

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
        fLog -i -p warning -m 'gitproj.config.ver was not found. Setting it to: $gpVer' -l "$tSrc:$LINENO"
        return 1
        fComSetConfig -L -k "gitproj.config.ver" -v "$gpVer"
        fComSetConfig -H -k "gitproj.config.ver" -v "$gpVer"
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
export tSrc=${BASH_SOURCE##*/}
fComSetGlobals
