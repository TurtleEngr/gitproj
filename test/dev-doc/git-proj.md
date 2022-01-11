<div>
    <hr/>
</div>

# gitproj-com.inc Internal Documentation

## Template Use

### Configuration

- Copy template.sh to your script file.
- Your script, gitproj-com.inc, and gitproj-com.test
need to be in the same directory.
- Globally replace SCRIPTNAME with the name of your
script file.
- Update the getopts in the "Get Args Section". Add
your script's options.
- Loop: document (with POD), add tests, add validate functions
- Loop: add unit test function, add functions, test

### Block Organization

- Configuration - exit if errors
- Get Args - exit if errors
- Verify external progs - exit if errors
- Run tests - if gpTest is set
- Validate Args - exit if errors
- Verify connections work - exit if errors
- Read-only functional work - exit if errors
- Write functional work - now you are committed! Try to keep going if errors
- Output results and/or launch next process

To avoid a lot of rework and manual rollbacks, put-off _writes_ that
cannot undone. Do as much as possible to make sure the script will be able
to complete write operations.

For example, **do not do this:** collect information, transform it,
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

### Variable Naming Convention

Prefix codes are used to show the **"scope"** of variables:

    gVar - global variable (may even be external to the script)
    pVar - a function parameter I<local>
    gpVar - global parameter, i.e. may be defined external to the script
    cVar - global constant (set once)
    tVar - temporary variable (usually I<local> to a function)
    fFun - function

All UPPERCASE variables are _only_ used when they are required by other
programs or scripts.

If you have exported variables that are shared across scritps, then
this convention can be extended by using prefixes that are related to
where the variables are set.

### Global Variables

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

### Documentation Format

POD is use to format the script's documentation. Sure MarkDown could
have been used, but it didn't exist 20 years ago. POD text can be
output as text, man, html, pdf, texi, just usage, and even MarkDown

Help for POD can be found at:
[perlpod - the Plain Old Documentation format](https://perldoc.perl.org/perlpod)

The documentation is embedded in the script so that it is more likely
to be updated. Separate doc files seem to _always_ drift from the
code. Feel free to delete any documentation, if the code is clear
enough.  BUT _clean up your code_ so that the code _really_ is
clear.

The internal documentation uses POD commands that begin with "=internal-".
See fComInternalDoc() for how this is used.

<div>
    <hr/>
</div>

<div>
    <hr/>
</div>

## Common Script Functions

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

### fLog -m pMsg \[-p pLevel\] \[-l $LINENO\] \[-e pErr\]

pLevel - emerg alert crit err warning notice info debug debug-N

    if gpVerbose = 0, don't output notice or info
    if gpVerbose = 1, don't output info
    if gpVerbose >= 2, output all logs

See Globals: gpSysLog, gpFacility, gpVerbose, gpDebug

#### fLog Examples:

    fLog -p warning -m "Missing awk" -l $LINENO -e 8
    fLog -p notice -m "Output only if -v"  -e 8 -l $LINENO
    fLog -p info -m "Output only if -vv" -l $LINENO
    fLog -p debug -m "Output only if $gpDebug > 0" -l $LINENO
    fLog -p debug-3 -m "Output only if $gpDebug > 0 and $gpDebug <= 3" -l $LINENO

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

    fComSetConfig -k pKey -v pValue [-g|-G-l|-L|-H|-f pFile] [-b|-i] [-a]

#### Description

#### Options

Only one of these will be used: -g, -l, -L, -H -f. The last one in the
list of options will be used if there is more than one.  If none, then
\-l is the default.

Only one or none of these can be specified: -b, -i. If more than one,
then the last one will be used.

- **-k pKey**

    This option is required.

- **-v pValue**

    This option is required.

- **-g**

    Write to ~/.gitconfig

- **-l**

    You must be in a git workspace for this option to work.

    Write to GIT\_DIR/.git/config

- **-L**

    Write to GIT\_DIR/$cConfigLocal

- **-H**

    Write to GIT\_DIR/$cConfigHost

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

    fComGetConfig -k pKey [-g|-G-l|-L|-H|-f pFile] [-b|-i]
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

Only one of these will be used: -g, -l, -f. The last one in the list
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
    be included: GIT\_DIR/$cConfigLocal and
    GIT\_DIR/$cConfigHost

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
    it's intitial "undefined" value.

    \* or if the value for pKey is equal to "TBD" or "not-defined", fError
    will be called and the script will terminate. I.e. the config var is
    still set to it's intitial "undefined" value.

    Note: -e will only exit the whole script if it is not in a subshell.
    I.e. don't use var=\\$(cmd) style.

- **-v pFilter**

    pFilter is used as a pattern match for the value found. This is useful
    for times when there could be duplicate pKeys with different
    values. For example there could be multiple "include.path" keys.
