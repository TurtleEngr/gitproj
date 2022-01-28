# Coding Convention

# Script Structure

## Configuration

- Copy an existing git-proj-CMD file, and gitproj-CMD.inc
file. The files should be in the same directory.
- Globally replace the "git proj CMD" with the name of the new CMD.
- Update the getopts and case statement, adding your script's options.
- TDD Loop: document (with POD), add tests, add validate functions
- TDD Loop: add unit test functions, add functions, test

## Block Organization

- Configuration - exit if errors
- Get Args - exit if errors
- Validate Args - exit if errors
- Verify external progs - exit if errors
- Verify connections work - exit if errors
- Read-only functional work - exit if errors
- Write functional work - now you are committed! Try to keep going if
      errors. Or rollback any work that was corrupted.
- Output results and/or launch the next process

To avoid a lot of rework and manual rollbacks, put-off **writes**
that cannot undone. Do as much as possible to make sure the script
will be able to complete write operations.

For example, **do not do this:** collect information, transform it,
write it to a DB, then start the next process on another
server. Whoops, that server cannot be accessed, so the DB update is
not valid! Gee, why didn't you verify all the connections you will
need, before committing to the DB?! Even if you did check, the
connection could have failed after the check, so maybe write to a tmp
DB, then when all is OK, then update the master DB with the tmp DB
changes.

Where ever possible make your scripts "re-entrant". Connections can
fail at anytime and scripts can be killed at anytime; How can any
important work be continued or work rolled-back? Planing for
"failures" is NOT planning to fail; it is what a professional engineer
does to design in quality.

## Variable Naming Convention

Prefix codes are used to show the **"scope"** of variables:

    gVar - global variable (may even be external to the script)
    pVar - a function parameter B<local>
    gpVar - global parameter, i.e. may be defined external to the script
    cVar - global constant (set once)
    tVar - temporary variable (usually B<local> to a function)
    fFun - function
    f[Prefix]FunName - Prefix is usually an abbreviation of the file it is in

All UPPERCASE variables are **only** used when they are required by
other programs or scripts.

If you have exported variables that are shared across scritps, then
this convention can be extended by using prefixes that are related to
where the variables are set.

## Global Variables

For more help, see the Globals section in fUsage.

    gpVerbose - -q -v, -V N
    gpDebug - -x, -xx, ..., -X N
    gErr - error code (0 = no error)
    gpCmdName - script's name taken from $0
    cCurDir - current directory
    gpBin - directory where the script is executing from
    gpDoc - directory for config/, hooks/, test/
    gpTest - test directory

## Documentation Format

POD is use to format the script's documentation. Sure MarkDown could
have been used, but it didn't exist 20 years ago. POD text can be
output as text, man, html, pdf, texi, just usage, and even MarkDown

Help for POD can be found at:
[perlpod - the Plain Old Documentation format](https://perldoc.perl.org/perlpod)

The documentation is embedded in the script so that it is more likely
to be updated. Separate doc files seem to **always** drift from the
code.  Feel free to delete any documentation, if the code is clear
enough. BUT **clean up your code** so that the code **really**
is clear.

The internal documentation uses POD commands that begin with
"=internal-". See fComInternalDoc() for how this is used.

# Git Config Definition Order

The last definition 'wins".

- 1. /etc/gitconfig - optional
- 2. /home/USER/.gitconfig
- 3. /home/USER/.gitproj.config.global (include.path at end of
.gitconfig)
- 4. GIT\_DIR/.git/config
- 5. GIT\_DIR/.gitproj.config.$HOSTNAME (include.path at end of
GIT\_DIR/.git/config)
- 6. Env. var. will override corresponding .git config vars.
- 7. Command line options will override env. var. and
corresponding .git config vars.

# Variable Naming Convention

- Globals that came from command line, config files, or external
to the scripts, begin with "gp" (global parameter). For example: gpVAR
(if they are not already defined before script (#7), set the initial
value to files #1 through #6. The command line option can always set
the value (#7)
- Global variables should begin with a "g"
- Global constants should begin with a "c"
- Local variables should begin with a "t" (temporary)
- git config vars are usually all lower case with words
separated by hyphens (-).
- Bash variables usually use CamelCase, with each word beginning
with an upper case letter (no hyphens or underscores).

# Function Naming Convention

- All gitproj functions begin with a "f"
- Functions in the include files, begin with command base. For
example: functions in gitproj-init.inc begin with "fInit".

# Coding Patterns

- At the top of include files define exports for all the globals
that the include file reads/writes to.
- At the end of an include file, call a function that will
define defaults for the important globals used by the include file.
\[optional\]
- For the user callable scripts, do minimal setup--mainly
collect the and validate the options. Include files with common
functions and functions specific to the script. Put as much as
possible into functions in the include file, so that the functions can
be directly tested with unit test scripts found in doc/test.
- Minimal vars: gpBin, cCurDir, gpDoc, gpTest if a test
script. All other vars can be defined from include files or from git
config vars.
- Define gpCmdName at the top of each each script that is called
by a user.
- Clean-Coding style (well I try).
    - Ifs
        - Avoid if/then/else. Replace with: if problem, then exit
        - Avoid long contents in ifs. Make functions.
        - Don't nest ifs more than 2 levels. One level is preferred.
        - Don't clutter the code with "pass-through" error-handling ifs. (see below)
        - Rather than using debug ifs, use fLog with its debug level
        support. Using TDD, the need for internal debug output should be
        reduced.
    - Identify problems or defaults early in a function so it can
    exit early.
    - Most of the code is structured so that if a function returns,
    you can assume it executed OK, or it took an acceptable default
    action. There should be no need to continually check exit codes from
    functions.
    - In other words, if there is a "fatal" error, cleanup, and exit
    with an error msg. Don't pass around exit codes, through multiple
    levels (one level is OK for the "utility" functions). Exit codes are
    mainly for the immediate calling function--if there was and error,
    exit, don't pass the error up to another level.
- Use TDD: Every function should have tests to exercise all the
input boundary conditions, and all of the error-handling states that
are unique to the function.
    - shunit2 is used for the TDD framework. See test/Makefile
    - TDD is done with the git development environment structure. It
    is NOT done with "installed" code.
    - The -p option is not used with the "read" command, because the
    prompt is not "captured" with the test scripts. So use "echo -n" for
    the prompts before the read command.
- fLog and fError messages
- Put this at the beginning or end of each file

            export tSrc=${BASH_SOURCE##*/}

- Put this in each function that calls fError or fLog

            local tSrc=${BASH_SOURCE##*/}

- in Error and Log always pass this argument:

            -l $tSrc:$LINENO

# File Include Pattern - prod

    gpBin=/usr/lib/git-core     # set when a CMD is run
    gpDoc=/usr/share/doc/gitproj

        $gpBin/git-proj-CMD
            source $gpBin/gitproj-com.inc
                fComSetGlobals
            source $gpBin/gitproj-CMD.inc
                fCMDSetGlobals

# File Include Pattern - dev

    gpBin=DIR/gitproj/git-core  # set when a CMD is run
    gpDoc=$gpBin/../doc or DIR/gitproj/doc

        $gpBin/git-proj-CMD
            source $gpBin/gitproj-com.inc
                fComSetGlobals
            source $gpBin/gitproj-CMD.inc
                fCMDSetGlobals

# File include pattern - dev-test

    gpTest=DIR/gitproj/test   # set when a test-*.inc is run
    gpBin=$gpTest/../git-core or DIR/gitproj/git-core
    gpDoc=$gpTest/../doc or DIR/gitproj/doc

        $gpTest/test-com.sh*
            source $gpTest/test.inc
                fTestSetupEnv
                source $gpBin/gitproj-com.inc
                    fComSetGlobals
            fComRunTests
                source $gpTest/shunit2.1*

        $gpTest/test-CMD.sh
            source $gpTest/test.inc
                fTestSetupEnv
                source $gpBin/gitproj-com.inc
                    fComSetGlobals
            fTestCreateEnv
            source $gpBin/gitproj-CMD.inc
                fCMDSetGlobals
            fTestConfigSetup
