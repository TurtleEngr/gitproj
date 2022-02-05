<div>
    <hr/>
</div>

# gitproj-com.inc Internal Documentation

<div>
    <hr/>
</div>

## Common Script Functions

### try/catch functions

**NOTE the env. var. part does not reliably work. So don't use the
"tc" functions until they are fixed.**

These functions were inspired by:
https://gist.github.com/e7d/e43e6586c1c2ecb67ae2

Changes: This version doesn't save the previous state of "set -e".  Exported
env. var. changes in the subshell are saved, so they can be restored
when the subshell is done.

#### tcTry

Call this just before the required subshell section.

#### tcSaveEnv

Call this when env changes must not be lost. This is called by the
other related commands.

#### tcReturn

Call this at end of the tcTry subshell.
Calls tcSaveEnv.

#### tcThrow

Call this to throw error (1..255) to tcCatch
Calls tcSaveEnv.

#### tcCatch

Save throw or cmd errors, and restore any env. var. changes in the subshell.
If a command exits before a tcSaveEnv call, env. var. changes will be lost.
Calls tcSaveEnv.

#### tcThrowErrors

Now, if a cmd returns non-zero, exit the try subshell.
Calls tcSaveEnv.

#### tcIgnoreErrors

Now, if a cmd returns non-zero, execution will continue.
Calls tcSaveEnv.

#### Sample Use

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

<div>
    <hr/>
</div>

### fComConfigCopy

#### Synopsis

    fComConfigCopy [-f] -s pSource -d pDest [-i pInclPat] [-e pExclPat]

#### Description

Copy values from git config file pSource to pDest. If -f (force), the pSource
values will override the pDest values, otherwise the pSource value will
only be copied to pDest if it does not exist there.

The -i pIncPat is a "grep" pattern that only selects the variable
names that match the pattern. For example:
\-i 'gitproj\\.config\\.|gitflow\\.'

The -e pExcPat is a "grep" pattern that removes variable names that
match the pattern. For example: -e 'gitproj.config.remote-raw-origin'
pExcPat is applied after pIncPat.

pSource and pDest files must exist. pSource must be readable, and
pDest must be writable.

A backup copy of pDest is made with this command:
cp -backup=t $pDest $pDest.bak

<div>
    <hr/>
</div>

### fComSetGlobals

Set initial values for all of the globals use by this script. The ones
that begin with "gp" can usually be overridden by setting them before
the script is run.

<div>
    <hr/>
</div>

### fComCheckDeps "pRequired List" "pOptional List"

Check for required and optional programs or scripts used by this script.
If any required programs are missing, exit the script.

<div>
    <hr/>
</div>

### fComInternalDoc

This function collects all of the "internal-pod" documentation from
stdin and it outputs to stdout.

<div>
    <hr/>
</div>

### fComUsage -f pFileList -s pStyle \[-t pTitle\] \[-i\] \[-a\]

- **-f pFileList** - list of file names
- **-s pStyle** - output style

        short|usage - usage only (does not work with -i or -a)
        man         - all, man format (does not work with -i or -a)
        long|text   - all text format
        html        - all, html format (see -t)
        md          - all, markdown format

- **-t** - title for HTML style
- **-i** - internal doc only (see fComInternalDoc)

<div>
    <hr/>
</div>

### fComYesNo pPrompt

If gpYesNo = "Yes", set gResponse to "y", and return 0.

If gpYesNo = "No", set gResponse to "n", and return 1.

If gpYesNo is not set, output the pPrompt, then "read" the response.

If the response begins with a "y", then that is the same as gpYesNo = "Yes"

If the response is not a "y", then that is the same as gpYesNo = "No"

<div>
    <hr/>
</div>

### fComFmtLog pLevel "pMsg" pLine pErr

This function formats and outputs a consistent log message output.
See: fLog and fError.

<div>
    <hr/>
</div>

### fLog -m pMsg \[-p pLevel\] \[-l $LINENO\] \[-e pErr\] \[-f\]

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

    if gpDebug = 0, don't output any debug msgs
    if gpDebug > 0, output any "debug" msgs, or
    if gpDebug = N1, output "debug-N2" msgs, if N1 >= N2

See Globals: gpSysLog, gpFacility, gpVerbose, gpDebug

#### fLog Examples:

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

<div>
    <hr/>
</div>

### fError -m pMsg \[-l $LINENO\] \[-e pErr\] \[-i\]

This will call: fLog -p crit -m "pMsg" -l pLine -e pErr

If no -i, then "fUsage short", will be called.

<div>
    <hr/>
</div>

### fComGit \[pArgs...\]

TODO: replace this with set/try/catch

This is a wrapper for "git". If gpVerbose is >= 2, the git call will
will be echoed. If the git call exits with a non-0, then an error
message is output and the script exits (if not called in a sub-shell).

This is used to make the code look "clean". You don't need to clutter
the code with if statements with every git command. I.e. this supports
the "design pattern" where, if a command exits, it is assumed all is
OK, because if it failed, the script would exit.

<div>
    <hr/>
</div>

### fComGetConfig

#### Synopsis

    fComSetConfig -k pKey -v pValue [-g|-l|-L|-f pFile] [-b|-i] [-a]

#### Description

#### Options

Only one of these will be used: -g, -l, -L, -f. The last one in the
list of options will be used if there is more than one.  If none, then
\-l is the default.

Only one or none of these can be specified: -b, -i. If more than one,
then the last one will be used.

- **-k pKey**

    This option is required.

- **-v pValue**

    This option is required.

- **-g**

    Write to ~/.gitconfig (--global)

- **-l**

    You must be in a git workspace for this option to work. (--local)

    Write to GIT\_DIR/.git/config

- **-L**

    Write to GIT\_DIR/.gitproj

- **-f pFile**

    Write to pFile.

- **-b**

    The value will be normalized to "true" or "false".

- **-i**

    The value will be converted to a single "byte" value, based on
    its suffix.

    Valid suffixes: k, K, m, M, g, G

    These will be converted to lowercase before being sent to "git config".

- **-a**

    Use this option if you need to specify a key with multiple values.
    If not specified, then any matching key in the file, being written to,
    will replace the value.

<div>
    <hr/>
</div>

### fComGetConfig

#### Synopsis

    fComGetConfig -k pKey [-g|-l|-L|-f pFile] [-b|-i]
                  [-d pDefault] [-e] [-v pFilter]

#### Description

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

#### Options

Only one of these will be used: -g, -l, -L, -f. The last one in the list
of options will be if there is more than one.  If none, then all the
config files will be used.

Only one or none of these can be specified: -b, -i. If more than one,
then the last one will be used.

- **-k pKey**

    This option is required. The last file were this key is found will be
    used to set the value returned.

- **-g**

    Look in ~/.gitconfig Includes are followed so ~/gitproj.config.global
    will be included.

- **-l**

    You must be in a git workspace for this option to work.

    Look in GIT\_DIR/.git/config Includes are followed so these files will
    be included: GIT\_DIR/.gitproj and
    GIT\_DIR/.git/config

- **-f pFile**

    This will look in pFile for pKey. Includes will be followed.

- **-b**

    The value will be normalized to "true" or "false".

    Valid values in the files:

- **-i**

    The value will be converted to a single "byte" value, based on
    its suffix.

    Valid suffixes: k, m, g

    Note: fComSetConf with -i will allow: \[kKmMgG\]. It will convert the
    values to lower-case, when saving them in the config files.

- **-d pDefault**

    If pKey is not found, then pDefault will be returned, and gGetConfigOrigin
    will be set to "default".

- **-e**

    If -e, then do some simple validation checks:

    \* if the pKey is not found, then fError will be called and the script
    will terminate. I.e. the config var is required.

    \* or if the value for pKey is equal to pDefault, fError will be called
    and the script will terminate. I.e. the config var is still set to
    it's initial "undefined" value.

    \* or if the value for pKey is equal to "TBD" or "not-defined", fError
    will be called and the script will terminate. I.e. the config var is
    still set to it's initial "undefined" value.

    Note: -e will only exit the whole script if it is not in a subshell.
    I.e. don't use var=\\$(cmd) style.

- **-v pFilter**

    pFilter is used as a pattern match for the value found. This is useful
    for times when there could be duplicate pKeys with different
    values. For example there could be multiple "include.path" keys.
