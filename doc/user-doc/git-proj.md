# NAME git proj

This implements the "git proj" subcommand, for managing large binary
files, and repositiories on external drives.

# SYNOPSIS

        git proj [-v] [-V] [--version] [-h] [-H pStyle]

        git proj [pSubCmd] [pSubCmdOpt] [pComOpt]

        pSubCmd: init, remote, clone, push, pull, status
        pSubCmdOpt: Just run: "git proj [pSubCmd]"
        [common-options]: [-h] [-H pStyle] [-q -v -V N] [-x... -X N]

# DESCRIPTION

The "git proj" subcommand supports using git repo for versioning files to
locally mounted external drives.

git-proj also mostly solves the issue with large binary files causing
git repo "bloat". Large files are not versioned, they are only
copied. So if a version important, just rename the file.  (symlinks
are used and versioned to point to the large binary files.)

This is a much simpler implementation than git-lfs for managing the
versioning of large binary files. Since the large files are not
versioned, i.e. only the last version is saved, this is a comompromize
on having "perfect" version control of the files.

The main inspiration for the proj tool came from the need to
versioning video projects. Video files are HUGE, yet they don't change
much. Since most video files are rendered from files that do not
change, it is only important to version the video editor's file, so
that you can recreate a particular video file version. Since rendering
takes time you will want to save the rendered file. But there is usually
no need to save every rendered version.

# OPTIONS

## pSubCmd

- **init - Initialize the git project repo**

        git proj init [-l [pDirPath]] [-e pDirPath] [-r [pDirPath]

- **clone - Clone a previously saved project**

        git proj clone pExternalPath

- **add - Add a large binary file to the "raw" remote**

        git proj add pFilePath

- **push - Push files to the "raw" remote**

        git proj push [-b]

- **pull - Pull files from the "raw" remote**

        git proj pull [-b]

- **config - Redefine config values**

        git proj config [-l pExternalPath] [-e pRawPath]

- **status - Show the staus of "raw" files, and more**

        git proj status [-b] [-v]

## pSubCmdOpt

- **git proj \[pSubCmd\]**

    This outputs short usage help for pSubCmd.

- **git proj \[pSubCmd -h\]**

    This outputs full usage help for pSubCmd.

- **git proj \[pSubCmd -H pStyle\]**

    See **-H pStyle** in pComOpt section, for the output styles.

## \[common-options\]

minimum

- **-h**

    Output this "long" usage help. See "-H long"

- **-H pStyle**

    pStyle is used to select the type of help and how it is formatted.

    Supported styles:

            short|usage - Output short usage help as text.
            long|text   - Output long usage help as text. All subcommands.
            man         - Output long usage help as a man page.
            html        - Output long usage help as html. All subcommands.
            md          - Output long usage help as markdown.
            int         - Output internal documentation as text.
            int-html    - Output internal documentation as html.
            int-md      - Output internal documentation as markdown.

- **-q**

    Set verbose to lowest level: 0

    Only very important log messages will be output.

- **-v**

    This sets the verbose lovel to 2, which is the default.

    At level 2, warning and notice messages will be output.

- **-V N**

    Set the verbose level to N.

        0 - critical, errors, and important warning are output
        1 - warnings and above are output
        2 - notice and above are output
        3 - info and above are output

    The verbose level can also be set with env. var. gpVerbose. However
    the command line option will override the variable.

- **-x**

    Set the gpDebug level number. Add 1 for each -x argumen.  Or you can set
    gpDebug before running the script. Or you can use the -X option.

        "fLog -p debug" messages will be output if gpDebug != 0.
        "fLog -p debug-N" messages will be output if gpDebug >= N.

    See: fLog Internal documentation for more details.

- **-X N**

    Set the gpDebug level to N. The command line options will override the
    gpDebug env. var.

        0 - no debug messages
        >0 - "debug" messages
        1 - "debug-1" messages
        2 - "debug-2" and "debug-1" messages
        ...
        N - "debug-N" and messages less than N

## ~/.gitconfig

Source: gpDoc/config/gitconfig

If this doesn't exist, git proj init will create it from "Source"

Section: \[gitproj config\]

git proj init, will copy this... TBD

    remote-min-size = 20g

This is the minium space that should be available on the external
drive.  The command will not continue if there is not enough space.
The available space must also be twice the size of the space used by
ProjName.raw.

    remote-raw-origin

Default: -d pMountDir/ProjName.raw

Section: \[gitproj hook\]

TBD

## top-dir/.gitproj

TBD

## top-dir/.git/config

TBD - this needs to be rewritten.

Where HOSTNAME will be set to $HOSTNAME. This allows for different
locations of file, based on the host. For example the remote-raw-url
(mount point) could be very different between hosts.

Initial Source: ~/.gitconfig Sections \[gitproj \*\]

This will be created when git project repo is first created on a host.

This will be put at the beginning of the config file, so that common
project defaults can be defined. Then .gitproj.config.HOSTNAME can
override variables. Any changed to the variables in
.gitproj.config.local variables will be written to
.gitproj.config.HOSTNAME

Uncomment the variables that should override ~/.gitconfig or
~/.gitproj.config.global. If the variables are host specific, then the
variable should be put in the correspoinding .gitproj.config.$HOSTNAME
file.

        [gitproj "config"]
                # Expected version, only first N must be the same.
                # Use backward compatible code, or exit.
                # Warn if second N is different
                ver = 0.1.2

                # States: not-installed, installed, config-errors
                proj-status = not-installed

                syslog = true
                facility = user

                bin = /usr/lib/git-core
                # bin = $(git --exec-path)

                doc = /usr/share/doc/git-proj
                test = /usr/share/doc/git-proj/test

                # See .gitproj.config.local and/or .gitproj.config.$HOSTNAME

                # States: not-defined, defined, config-errors
                local-status = not-set-up

                # States: not-defined, defined, multiple-defined
                remote-status = not-defined

                # origin-url with path and .git remote
                proj-name = TBD

                # This should only be changed on the matching host
                # git rev-parse --show-toplevel
                local-top-dir = TBD

                # Set by init. Changing these will require manual repair.
                # If no ~ or /, this is relative to top-dir

                git-flow-pkg = TBD

                # Local mount examples
                remote-raw-origin = TBD
                #remote.origin.url = /MOUNT-DIR/DIR/NAME.git

                # Remote examples (not implemented. TBD)
                #remote-raw-url = joe@example.com:/repo/git/video-2021-09-24/NAME.raw
                #remote.origin-url=USER@example.com:/repo/git/video-2021-09-24/NAME.git
                #remote.origin-url=git@github.com:TurtleEngr/gitproj.git

        [gitproj "hook"]
                # If pre-commit-enabled = true, pre-commit hook script will be
                # installed into .git/hooks/
                # Relative to gpDoc
                source = hooks/pre-commit
                pre-commit-enabled = true
                check-file-names = true
                check-for-big-files = true
                # End size with b, k, m, or g
                binary-file-size-limit = 1k

## Global Env. Var. Config

These are globals that may affect how the script runs. Just about all
of these globals that begin with "gp" can be set and exported before
the script is run. That way you can set your own defaults, by putting
them in your ~/.bashrc or ~/.bash\_profile files.

Global variable precedence (the last one to set the gp variable, wins):

    * internal hardcoded default
    * variable in ~/.gitconfig
    * variable in PROJ/.git/config
    * env. var.
    * command line option

Notation below: gp\[Var\], -\[cli-opt\], \[git.config.var\], (default)

- **gpSysLog, NA, gitproj.config.syslog, (false)**

    If set to 0, log messages will only be sent to stderr.

    If set to 1, log messages will be sent to stderr and syslog.

    Default: false

- **gpFacility, NA, gitproj.config.facility, (user)**

    Log messages sent to syslog will be sent to the "facility" specified
    by by gpFacility.

    "user" log messages will be sent to /var/log/user.log, or
    /var/log/syslog, or /var/log/messages.log

    See: fLog

    Default: user

    Allowed facility names:

        local0 through local7 - local system facilities
        user - misc scripts, generic user-level messages
        auth - security/authorization messages
        authpriv - security/authorization messages (private)
        cron - clock daemon (cron and at)
        daemon - system daemons without separate facility value
        ftp - ftp daemon
        kern - kernel  messages  (these  can't be generated from user processes)
        lpr - line printer subsystem
        mail - mail subsystem
        news - USENET news subsystem
        syslog - messages generated internally by syslogd(8)
        uucp - UUCP subsystem

    These are some suggested uses for the localN facilities:

        local0 - system or application configuration
        local1 - application processes
        local2 - web site errors
        local3 - web site access
        local4 - backend processes
        local5 - publishing
        local6 - available
        local7 - available

- **gpVerbose, -q, -v, -V N, gitproj.config.verbose, (2)**

        -q - gpVerbose=0
        -v - gpVerbose=2
        -v N - gpVerbose=N

        gpVerbose  = 0 - output error messages (corrections must be made)
        gpVerbose >= 1 - output warnings messages (corrections may be needed)
        gpVerbose >= 2 - output notice messages (important information)
        gpVerbose >= 3 - output info messages (give more information)

    Default: 2

    Normal log message:

        Command [warning, notice, info]: Message [File:LineNo](ErrCode)

    Error messages (crit will exit, err might continue):

        Command [crit, err]: Error: Message [File:LineNo](ErrCode)

    An internal error. This is probably a defect in the code (collect all
    the output for a bug report):

        Command [crit, err]: Internal: Error: Message [File:LineNo](ErrCode)
        StackTrace:

- **gpDebug, -x, -X N, NA, (0)**

    Default: 0

    There is no config variable for gpDebug.

    If set to 0, all "debug" and "debug-N" level messages will be skipped.

    If not 0, all "debug" level messages will be output.

    If "debug-N" level is used, then if gpDebug is >= N, then the log
    message will be output, otherwise it is skipped.

- **gpAuto, -a, NA, (false)**
- **gpYesNo, -y, -n, NA, (No)**

    If gpAuto is true, then gpYesNo can be used to define default answers.

- **gpAutoMove, NA, NA, (false)**
- **gpBin, NA, gitproj.config.bin, (/usr/lib/git-core)**

    The location of the executing command will override this.

- **gpDoc, NA, gitproj.config.doc, (/usr/share/doc/git-proj)**

    If not found, then set to: $gpBin/../doc  If still not found: error.

- **gpCheckFileNames, NA, gitproj.hook.check-file-names, (true)**
- **gpCheckForBigFiles, NA, gitproj.hook.check-for-big-files, (true)**
- **gpCheckInRaw, NA, gitproj.hook.check-in-raw, (true)**
- **gpCheckWhitespace, NA, gitproj.hook.check-whitespace, (true)**

    See rm-trailing-sp to fix.

- **gpAllowTabs, NA, gitproj.hook.allow-tabs, (false)**

    See rm-trailing-sp to fix (-t option)

- **gpHookVerbose, NA, gitproj.hook.verbose, (true)**
- **gpPreCommitEnabled, NA, gitproj.hook.pre-commit-enable, (true)**
- **gpMaxSize, NA, gitproj.hook.binary-file-size-limit, (10k)**
- **NA, NA, gitproj.config.remote-min-size, (20g)**
- **gpGitFlow, NA, gitproj.config.flow, (true)**

# ENVIRONMENT

See Globals section for details.

HOME, USER, HOSTNAME, gpSysLog, gpFacility, gpVerbose, gpDebug, gpAuto

# FILES

    ~/.gitconfig
    ~/.gitproj.config.global
    PROJ/.gitproj.config.local
    PROJ/.gitproj.config.$HOSTNAME
    PROJ/.git/config

# SEE ALSO

    git proj init
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj status

# CAVEATS

Currently gitproj only supports **local** git repos. The repos are
called "remote" but that is only because the repo could be put on a
"mounted" disk.

A future implementation could support git repos that are truly remote,
on other systems. When that is implemented, an existing repo could be
"upgraded" to a gitproj repo.

A remote repo implies the "raw" files will also be managed on a remote
or local system. I.e. the raw files could be on most any servers that
is supported by rsync or rclone.

# AUTHOR

TurtleEngr

# HISTORY

(c) Copyright 2021 by TurtleEngr

<div>
    <hr/>
</div>

# NAME git proj init

# SYNOPSIS

    git proj init -l pLocalDir [-a] [-s pMaxSize] [-m] [-f]
                  [common-options]

# DESCRIPTION

This will create a local git repo with branches. If git-flow is
installed can be setup too. After "git proj init" is done, run
"git proj remote" to setup remote for git and raw files.

If there is a -a option, "git proj init" will be run with all the
default options, which can be overridden with other options.

If there is no -a option, you will be prompted for the settings.  See
the OPTION section for details.

When the local and remote git repos are setup, all the setings
will be saved to \[project\]/.git/config and \[project\]/.gitproj

# OPTIONS

- **-a**

    The -a option will automattically run the whole init process with
    default settings. The other options can be defined to override the
    default settings.

- **-l pLocalDir**

    Define the existing project directory. The last directory will be used
    for the name of the project. Required.

    Prompt:

        Dir (-l) [$PWD]? $gpLocalTopDir
            So the project Name will be: ${gpLocalTopDir##*/}

- **-s pMaxSize**

    Define the "size" for large binary files. Default 10K

    Prompt:

        Size (-s) [10K]?

- **-m**

    Prompt:

        Binary files greater than [pSize] were found in your project dir:
            [file list]

    The listed files can be moved to the project's "raw" directory. Dirs
    will be created in the raw directory that correspond to the project's
    directory. A symlink will replace the moved file. The symlink will
    point to \[raw\].

    The symlinks are only provided for backward compatability; it would be
    best to remove those links and modifiy your code and apps to access
    the files directly from the raw directories.

    Prompt:

        Move the files (-m) [y/n]?

- **-f**

    If git-flow is installed.

    Prompt:

        Setup git-flow (-f) [y/n]?

- **\[common-options\]**

        -h
        -H pStyle
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details. Or "man git-proj" for help with all the
    subcommands.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# SEE ALSO

    git proj
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj set
    git proj status
    git proj add   TBD
    git proj config TBD
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr

<div>
    <hr/>
</div>

# NAME git proj remote

# SYNOPSIS

    git proj remote [-a] [-d pMountDir] [common-options]

# DESCRIPTION

This will create a remote git repo on an external drive.

(Future versions may support repos on remote computers, where you have
rsync access.)

# OPTIONS

- **-d pMountDir**

    This command is run after "git proj init" to setup a remote location.
    After this is setup, "git proj push" and "git proj pull" can be used
    to push/pull git and raw file changes.

    Export the git repo to an external drive (or another local dir) This
    is usually the removable drive's "top" directory.  Ideally the top
    directory should be different across a set of external drives, so that
    the local "origin" can be used to make sure the proper git repo is
    found on the drive. The git "origin" will be set to
    pMountDir/ProjName.git And "remote-raw-origin" will be set to
    pMountDir/ProjName.raw

- **\[common-options\]**

        -h                     (-H text)
        -H pStyle
        -q | -v | -V N         (gpVerbose level)
        -x | -xx... | -X N     (gpDebug level)
        -y | -n                (only used with -a option)

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# SEE ALSO

    git proj
    git proj init
    git proj clone
    git proj add
    git proj push
    git proj pull
    git proj config
    git proj status
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr

<div>
    <hr/>
</div>

# NAME git proj clone

# SYNOPSIS

    git proj clone -d pRemoteDir [-a] [-y|-n] [common-options]

# DESCRIPTION

Clone a local git repo from the external pRemoteDir.

Use this script get a local copy of the remote git repo frome a
mounted drive.

# OPTIONS

- **-d pRemoteDir**

    TBD

- **-a**

    Automated mode. Use this in batch (non-interactve) scripts. See the
    \-y|-n options.

- **-y|-n**

    These are only used if -a option is defined. Where there are
    "exceptions" these will be used for the default answer: -y continue,
    \-n quit.

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

    Run "git proj -h"  for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

The drive is mounted at: /mnt/usb-video and the the repo is
at: video-2019-04-01/trip.git So use this to get a local copy.

proj-get-local -d /mnt/usb-video/video-2019-04-01/trip.git

# SEE ALSO

    git proj
    git proj remote
    git proj clone
    git proj add
    git proj push
    git proj pull
    git proj set
    git proj status
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr

<div>
    <hr/>
</div>

# NAME git proj push

# SYNOPSIS

    git proj push [-b] [-y|-n] [common-options]

# DESCRIPTION

    rsync will be used top copy the '[ProjName]/raw/' files to
    [remote-raw-origin]/[ProjName].raw.

If the -b option is given then run:

    "git push origin [current-branch]"

# OPTIONS

- **-b**
- **-y|-n**

    If -y, then default to "yes" to all prompts to continue.

    If -n, then default to "no" to all prompts to continue.

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# SEE ALSO

    git proj
    git proj init
    git proj remote
    git proj clone
    git proj add
    git proj push
    git proj pull
    git proj config
    git proj status
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr

<div>
    <hr/>
</div>

# NAME git proj pull

# SYNOPSIS

    git proj pull [-b] [-y|-n] [common-options]

# DESCRIPTION

rsync will be used top copy the \[remote-raw-origin\]/\[ProjName\].raw/ files
to '\[ProjName\]/raw/'.

If the -b option is given then run:

    "git pull origin [current-branch]"

# OPTIONS

- **-b**
- **-y|-n**

    If -y, then default to "yes" to all prompts to continue.

    If -n, then default to "no" to all prompts to continue.

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# SEE ALSO

    git proj
    git proj init
    git proj remote
    git proj clone
    git proj add
    git proj push
    git proj pull
    git proj config
    git proj status
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr

<div>
    <hr/>
</div>

# NAME git proj status

# SYNOPSIS

    git proj status [-g "pGitOpt"] [-r "pRawOpt"] [common-options]

# DESCRIPTION

    Do a "git status"
    Verify gitproj.config.remote-raw-origin is defined and mounted.

    Verify origin is set to a path that exists (if mounted).

    Give a "diff" (-qr) of the raw files, local vs remote (if mounted)

# OPTIONS

- **-g "pGitOpt"**

    Git status options. For example:

    git proj status -g "--short --ignored"

- **-r "pRawOpt"**

    Raw options. Currently these will be options passed to the diff
    command. (This will be added after the default options: -qr) For
    example:

    git proj status -r "-s"

- **\[common-options\]**

        -h
        -H pStyle
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details. Or "man git-proj" for help with all the
    subcommands.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# SEE ALSO

    git proj
    git proj init
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj add   TBD
    git proj config TBD
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
