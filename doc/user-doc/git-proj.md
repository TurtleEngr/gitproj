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

- **init** - Initialize the git project repo
- **remote** - Define the remote git repo and raw location
- **clone** - Clone a previously saved project
- **push** - Push files to the "raw" remote (and optionally git)
- **pull** - Pull files from the "raw" remote> (and optionally git)
- **status** - Show the staus of "raw" files, and git
- **add** - Add a large binary file to the "raw" remote
- **config** - Redefine config values

## pSubCmdOpt

Use "git proj CMD -H usage" to get a quick summary of a command's options.

Use "git prog CMD -h" for full help.

## \[common-options\]

    -h
    -H usage|text|html|md|man|int|int-html|int-md
    -q, -v, -V N     (gpVerbose)
    -x, -xx..., -X N (gpDebug)

- **-h**

    Output this "long" usage help. See "-H long"

- **-H pStyle**

    pStyle is used to select the type of help and how it is formatted.

    Supported styles:

        short|usage - Output short usage help as text.
        long|text   - Output long usage help as text. All subcommands.
        html        - Output long usage help as html. All subcommands.
        md          - Output long usage help as markdown.
        man         - Output long usage help as a man page.
        int         - Output internal documentation as text.
        int-html    - Output internal documentation as html.
        int-md      - Output internal documentation as markdown.

- **-q**

    Set verbose (gpVerbose=0) to lowest level: 0

    Only very important log messages will be output.

- **-v**

    This sets the verbose lovel (gpVerbose=2) to 2, which is the default.

    At level 2, warning and notice messages will be output.

- **-V N**

    Set the verbose level to N (gpVerbose=N)

                 N
        0 - -q,-V0 critical, errors, and important warning are output
        1 -    -V1 warnings and above are output
        2 - -v,-V2  notice and above are output
        3 -    -V3 info and above are output

    The verbose level can also be set with env. var. gpVerbose. However
    the command line option will override the variable.

    gpVerbose and gpDebug control what log messages are output. This is
    what the log messages look like:

    \* Normal log messages:

        [Command] [warning|notice|info]: [Message] [File:LineNo](ErrCode)

    \* Error messages (crit will exit, err might continue):

        [Command] [crit|err]: Error: [Message] [File:LineNo](ErrCode)
        If gpDebug >= 2, a StackTrace will be output.

    \* An internal error. This is probably a defect in the code (collect all
    the output for a bug report):

        [Command] [crit|err]: Internal: Error: [Message] [File:LineNo](ErrCode)
        StackTrace: ...

    Key:

        [Command] - the top level command
        [crit|err|warning|notice|info|debug] - log levels
        [File:LineNo](ErrCode) - exacly where the error message came from (optional)

- **-x**

    Set the gpDebug level number. Add 1 for each -x argument.  Or you can
    set gpDebug before running the script. Or you can use the -X option.

- **-X N**

    Set the gpDebug level to N. The command line options will override the
    gpDebug env. var.

        0  - no debug messages
        >0 - "debug" messages
        1  - "debug-1" messages
        2  - "debug-2" and "debug-1" messages
        ...
        N  - "debug-N" and messages less than N

    Debug log messages look like this::

        [Command] [debug]: debug-N: [Message] [File:LineNo](ErrCode)

## Confiiguration

These are the main configuration files you will need to know about.

    /usr/share/doc/config/gitconfig
    ~/.gitconfig
    PROJ/.gitproj
    PROJ/.git/config

See [gitproj Configuration Documentation](https://metacpan.org/pod/config.html) for details
about these files and all the important gitproj variables.
The pre-commit hook and its config vars are also described.

    remote-raw-origin

Default: -d pMountDir/ProjName.raw

Section: \[gitproj hook\]

TODO

## top-dir/.gitproj

TODO

## top-dir/.git/config

TODO - this needs to be rewritten.

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

                # Remote examples (not implemented. TODO)
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
                binary-file-size = 1k

## Global Env. Var. Config

See... TODO

# EXAMPLES

\[create a blank git-proj\]

\[create a git-proj from existing files\]

# ENVIRONMENT

See Globals section for details.

HOME, USER, HOSTNAME, gpSysLog, gpFacility, gpVerbose, gpDebug, gpAuto

# FILES

Config files:

    ~/.gitconfig - --global (gitproj sections setup with 'init' or 'clone')
    PROJ/.gitproj - set up with 'init' and used by 'clone'
    PROJ/.git/config --local (setup with 'init' or 'clone')

Subcommand files:

    /usr/share/doc/git-proj/
        VERSION - current installed version (git proj --version)
        CHANGES.md - changes for each version
        README.md - getting started
        LICENSE - GNU GPL V3
    /usr/share/doc/git-proj/config/
        gitconfig - global config template
        gitignore - default ignore file
    /usr/share/doc/git-proj/hooks/
        pre-commit - see gitproj.hooks section for configuring this
    /usr/share/doc/git-proj/contrib/
        bash-fmt - format bash scripts
        rm-trailing-sp - fix pre-commit whitespace issues
    /usr/share/doc/git-proj/user-doc/
        git-proj.html - all documentation in one file (git proj -h)
        git-proj.md - all documentation in one file (git proj -h)
        git-proj-CMD*.html (git proj CMD -h)
        git-proj-CMD*.md (git proj CMD -h)
    /usr/lib/git-core/git-proj/
        git-proj - get overall help "git proj -h"
        git-proj-CMD - called with "git proj CMD"
        gitproj-CMD.inc - all the code for CMD

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
"mounted" disk. Only the "origin" remote is supported. However each
workspace can have its own "origin" definition, because the mount
points could be different between systems.

An existing remote git repo can be used, BUT manual work will be
needed to set it up. TODO

A future implementation could support git repos that are truly remote,
on other systems. When that is implemented, an existing repo could be
"upgraded" to be a gitproj repo. The "raw" file remote could also be
saved on other systems (via rsync, rclone, or even cvs).

Use use the -x or -X options, or gpDebug env. var.  to turn on debug
levels. Larger numbers, more debug. There is only a little bit of
debug output, because the code uses "test code" to identify problems,
before the code is released. See the github repo for the development
process.

# BUGS

Please report bugs at: [issues](https://github.com/TurtleEngr/gitproj/issues)

# AUTHOR

TurtleEngr

# HISTORY

(c) Copyright 2022 by TurtleEngr

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
        -H usage|text|html|md|man|int|int-html|int-md
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
    git proj add   TODO
    git proj config TDO
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
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)
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

    TODO

- **-a**

    Automated mode. Use this in batch (non-interactve) scripts. See the
    \-y|-n options.

- **-y|-n**

    These are only used if -a option is defined. Where there are
    "exceptions" these will be used for the default answer: -y continue,
    \-n quit.

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

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

    git proj push [-g] [-d] [-a] [-y|-n] [common-options]

# DESCRIPTION

    rsync will be used top copy the '[ProjName]/raw/' files to
    [remote-raw-origin]/[ProjName].raw.

# OPTIONS

- **-g**

    If the -g option is given then run:

        "git push origin [current-branch]"

- **-d**

    If the -d option is used, then the remote raw/ will be made identical
    to the local raw/. I.e. files might be deleated from the remote raw/.

    See the EXAMPLES section for a "save" way to use this option.

- **-a**

    This turns on automated push for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option
    will be added. -d could be very distructive, so it needs to be
    run interactively.

- **-y|-n**

    If -y, then push files to the remote raw/

    If -n, then just show what would be pushed to the remote raw/

    Default: -n

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

The rsync (and rclone) updates of the files in raw/ are not versioned, so
be very careful with the -d option; files will be permanently deleted.

For a "safe" way, of updating the files in raw/, is to do a "push",
then "pull" with no -d. Then manually remove files you don't want in
the local raw/. Now run "push" with -d, to update the remote raw/

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

    git proj pull [-g] [-d] [-a] [-y|-n] [common-options]

# DESCRIPTION

rsync will be used top copy the \[remote-raw-origin\]/\[ProjName\].raw/ files
to '\[ProjName\]/raw/'.

If the -g option is given then run:

    "git pull origin [current-branch]"

# OPTIONS

- **-g**

    If the -g option is given then run:

        "git push origin [current-branch]"

- **-d**

    If the -d option is used, then the local raw/ will be made identical
    to the remote raw/. I.e. files might be deleated from the local raw/.

    See the help EXAMPLES section, in "git proj push", for a "save" way to
    use this option.

- **-a**

    This turns on automated push for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option
    will be added. -d could be very distructive, so it needs to be
    run interactively.

- **-y|-n**

    If -y, then pull files from the remote raw/

    If -n, then just show what would be pulled from the remote raw/

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

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

        git proj status -g "-s --ignored"

- **-r "pRawOpt"**

    Raw options. Currently these will be options passed to the diff
    command. (This will be added after the default options: -qr) For
    example:

        git proj status -r "-s"

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
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
    git proj add   TODO
    git proj config TODO
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
