# NAME git proj

This implements the "git proj" sub-command, for managing large binary
files, and repositories on external drives.

# SYNOPSIS

        git proj [-v] [-V] [--version] [-h] [-H pStyle]

        git proj [pSubCmd] [pSubCmdOpt] [pComOpt]

        pSubCmd: init, remote, clone, push, pull, status
        pSubCmdOpt: Just run: "git proj [pSubCmd] -H usage"
        [common-options]: [-h] [-H pStyle] [-q -v -V N] [-x... -X N]

# DESCRIPTION

The "git proj" sub-command supports using git repo for versioning files to
locally mounted external drives.

git-proj also mostly solves the issue with large binary files causing
git repo "bloat". Large files are not versioned, they are only
copied. So if a version important, just rename the file.  (symlinks
are used and versioned to point to the large binary files.)

This is a much simpler implementation than git-lfs for managing the
versioning of large binary files. Since the large files are not
versioned, i.e. only the last version is saved, this is a compromise
on having "perfect" version control of the files.

The main inspiration for this git-proj tool came from my desire to use
git to version video projects. But video files are HUGE, yet they
don't change much. Since most video files are rendered from files that
do not change, it is only important to version the video editor's
file, so that you can recreate a particular video file version. Since
rendering takes time you will want to save the rendered files. But
there is usually no need to save every rendered version.

Currently this tool only supports git and large files on a local
system. The "remotes" can be setup on mounted drives, and they can be
used to clone the files to other systems.

# OPTIONS

## pSubCmd

- **init** - Initialize the git project repo
- **remote** - Define the remote git repo and raw location
- **clone** - Clone a previously saved project
- **push** - Push files to the "raw" remote (and optionally git)
- **pull** - Pull files from the "raw" remote> (and optionally git)
- **status** - Show the status of "raw" files, and git
- **add** - TBD Add a large binary file to the "raw" remote
- **config** - TBD Redefine config values

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
        long|text   - Output long usage help as text. All sub-commands.
        html        - Output long usage help as html. All sub-commands.
        md          - Output long usage help as markdown.
        man         - Output long usage help as a man page.
        int         - Output internal documentation as text.
        int-html    - Output internal documentation as html.
        int-md      - Output internal documentation as markdown.

- **-q**

    Set verbose (gpVerbose=0) to lowest level: 0

    Only very important log messages will be output.

- **-v**

    This sets the verbose level (gpVerbose=2) to 2, which is the default.

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
        [File:LineNo](ErrCode) - exactly where the error message came from (optional)

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

# RETURN VALUE

The sub-commands will return 0, if there were no serious errors. Even
if 0, pay attention to the log messages that are output.

If a sub-command retuns a non-0 code, then some change is needed before
trying again. Hopefully the error and warning message describe what
needs to change.

# EXAMPLES

See  the tutorial document for some examples.

# ENVIRONMENT

HOME, USER, gpSysLog, gpFacility, gpVerbose, gpDebug, gpAuto

See the [gitproj Configuration Documentation](config.md) for a
complete list of env. vars.

# FILES

See [gitproj Configuration Documentation](config.md) for
details about these files and all the important gitproj variables.
The pre-commit hook and its config vars are also described.

All the User Documentation can also be found online in github at:
[/doc/user-doc](https://github.com/TurtleEngr/gitproj/tree/main/doc/user-doc)

## Configuration Files

These are the main configuration files you will need to know about.

    /usr/share/doc/config/gitconfig        # Product
    ~/.gitconfig                           # User
    PROJ/.gitproj                          # Project
    PROJ/.git/config                       # Local

## Other Files

    /usr/share/doc/config/gitignore        # Product
    ~/.gitignore                           # User
    PROJ/.gitignore                        # Project

    /usr/share/doc/hooks/pre-commit        # Product
    ~/.pre-commit                          # User
    PROJ/.pre-commit                       # Project
    PROJ/.git/hooks/pre-commit             # Local

# SEE ALSO

    git proj init
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj status
    git proj add
    git proj config

    /usr/share/doc/git-proj/user-doc/
        git-proj.html - all sub-commands in one file (html format)
        git-proj.md - all sub-commands in one file (markdown format)
        config.html - important configuration information
        config.md - important configuration information
        subcommands/
            git-proj-CMD.html
            git-proj-CMD.md
        tutorial/
            *.html
            *.md

    Online docs: L<https://github.com/TurtleEngr/gitproj/tree/main/doc|https://github.com/TurtleEngr/gitproj/tree/main/doc>

# CAVEATS

Currently gitproj only supports **local** git repos. The repos are
called "remote" but that is only because the repo could be put on a
"mounted" disk. Only the "origin" remote is supported. However each
workspace can have its own "origin" definition, because the mount
points could be different between systems.

An existing remote git repo can be used, BUT manual work will be
needed to set it up. A tutorial may be created to describe the
manual changes.

A future implementation could support git repos that are truly remote,
on other systems. When that is implemented, an existing repo could be
"upgraded" to be a gitproj repo. The "raw" file remote could also be
saved on other systems (via rsync, rclone, or even cvs).

The list of feature requests can be found at:
[enhancements](https://github.com/TurtleEngr/gitproj/labels/enhancement)

# DIAGNOSTICS

Use use the -x or -X options, or gpDebug env. var.  to turn on debug
levels. Larger numbers, more debug. There is only a little bit of
debug output, because the code uses "test code" to identify problems,
before the code is released. See the github repo for the development
process.

# BUGS

Please report bugs at: [issues](https://github.com/TurtleEngr/gitproj/issues).

Use the "Bug Report" or "Feature Request" templates to submit your issue.

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

When the local and remote git repos are setup, all the settings
will be saved to \[project\]/.git/config and \[project\]/.gitproj

# OPTIONS

- **-a**

    The -a option will automatically run the whole init process with
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

    The symlinks are only provided for backward compatibility; it would be
    best to remove those links and modify your code and apps to access
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
    sub-commands.

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

# EXAMPLES

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

    Automated mode. Use this in batch (non-interactive) scripts. See the
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
    to the local raw/. I.e. files might be deleted from the remote raw/.

    See the EXAMPLES section for a "safe" way to use this option.

- **-a**

    This turns on automated push for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option
    will be added. -d could be very destructive, so it needs to be
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

For example:

    cd PROJ
    mount REMOTE-DRIVE

    # Update remote with all local raw/ files
    git proj push

    # Get all remote raw/ files
    git proj pull

    # Review and remove fiies from local, preparing for permanent removal
    cd raw
    rm FILES

    # Remove raw/ files from remote, that are not in local raw/ dir
    git proj push -d

Note: Because is is so destructive, the -d option cannot be used with
the -a option. Also the -y option will be ignored--you must answer the
"delete?" prompt.

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
    to the remote raw/. I.e. files might be deleted from the local raw/.

    See the help EXAMPLES section, in "git proj push", for a "safe" way to
    use this option.

- **-a**

    This turns on automated pull for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option will
    be added. -d could be very destructive, so it must be used
    interactively.

- **-y|-n**

    These are only use with the -a option.

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

# EXAMPLES

    cd PROJ
    git proj pull
    (Error: remote not mounted)

    mount REMOTE-DRIVE
    git proj pull

    # Make raw/ identical to remote raw/, i.e. allow deletes in local dir
    # -v will show more details about what changed.
    git proj pull -d -v

    # Pull raw/ and git files from remote, using -a -y to answer all prompts
    git proj pull -gay

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

    Git status options. For example to give a "short" status and show
    ignored files, use::

        git proj status -g "-s --ignored"

- **-r "pRawOpt"**

    Raw options. Currently these options will be passed to the diff
    command. For example to show the files that are the same between local
    and remote:

        git proj status -r "-s"

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details. Or "man git-proj" for help with all the
    sub-commands.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

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

<div>
    <hr/>
</div>

# NAME git proj config

# SYNOPSIS

    NOT IMPLEMENTED YET
    git proj config [common-options]

# DESCRIPTION

This is a text menu driven tool for checking and tuning up your
git-proj configurations. There are 4 configuration "levels" to
consider: Product, User, Project, and Local. The standard git support
configs at the User and Local levels, and the usual --global and
\--local options can be used with "git config" to get and set
variables. Or you can edit the files directly. See the FILES section
for files listed by their levels.

The "git proj config" tool has two major sections: "Health Checks" and
"Actions".

The "problem" health check will report serious problems that should be
fixed (some fix-up help will be offered). The other health checks are
useful for finding areas that you might want to update.  For example,
in your user level configs may have changed and you might notice some
project level configs are different. You can then run and "action" to
make them consistent.

The "actions" are used to copy config variables and files between the
different "levels". Some actions will only copy things that are
missing, while other actions can "force" changes.

## Config Files

       ProductConfig    /usr/share/doc/git-proj/config/gitconfig
       UserConfig       ~/.gitconfig (--global)
       ProjectConfig    PROJ/.gitproj
       LocalConfig      PROJ/.git/config (--local)

       ProductIgnore    /usr/share/doc/git-proj/config/gitignore
       UserIgnore       ~/.gitignore
       ProjectIgnore    PROJ/.gitignore

       ProductPreCommit /usr/share/doc/git-proj/hooks/pre-commit
       UserPreCommit    ~/.pre-commit
       ProjectPreCommit PROJ/.pre-commit
       LocalPreCommit   PROJ/.git/hooks/pre-commit

## Main Menu Options

- (1) Quit, exit git-proj-config

    In the lower level menus, #1 will change to "Back" and "Quit" will move
    to #3.

- (2) Help

    This will be available in all menus.

- (3) Run health checks that look for problems \[errors\]
- (4) Run health checks that look for differences \[diff, warnings\]
- (5) Run all health checks and report their status \[info\]
- (6) Select actions to update configs or files

## Global User Level Health Checks

These are done even if not in a git-proj managed workspace.

The Validate Checks, check ProductConfig and --global.

- Validate "facility" name \[error\]
- Validate "bin" and "doc" locations \[error\]
- Validate all bool vars are only true/false \[error\]
- Validate all int vars only have numbers \[error\]
- Compare --global with ProductConfig \[diff\]
- Compare UserIgnore with ProductIgnore \[diff\]
- Compare UserPreCommit with ProductPreCommit \[diff\]
- if git-flow=true
    - is git-flow installed? \[warning\]
    - are git-flow section vars defined in --global? \[warning\]

## Local and Project Level Health Checks

These are only done if you are in a git-proj managed workspace.

The Validate Checks, check ProjectConfig and --local.

- If defined, validate "facility" name \[error\]
- If defined, validate "bin" and "doc" locations \[error\]
- Validate all gitproj bool vars are only true/false \[error\]
- Validate all gitproj int vars only have numbers \[error\]
- Compare ProjectConfig with --global (only list the diffs from
all gitproj sections and the diffs that are in .gitproj and also in
--global \[diff\]
- Compare ProjectConfig with --local (only list the diffs from
all gitproj sections and the diffs that are in ProjectConfig and also
in --global \[diff\]
- Compare ProjectIgnore with UserIgnore \[diff\]
- Compare ProjectPreCommit with UserPreCommit \[diff\]
- Compare LocalPreCommit with ProjectPreCommit \[diff\]
- Note the disabled pre-commit checks \[info\]
- gitproj.config.proj-name != the git top dir name \[warning\]
- gitproj.config.local-status - is TBD \[error\]
- remote.origin.url - is not defined \[warning\]

    "git proj init" needs to be run.

- gitproj.config.remote-status - is TBD \[error\]

    "git proj remote" needs to be run.

- gitproj.config.remote-raw-origin - is TBD \[warning\]

    "git proj remote" needs to be run.

- warn if gitproj.config.remote.min-space is < 2x existing space \[warning\]
- if gitproj.config.remote-raw-origin, report if not found. \[warning\]

    Suggest mounting it or running "git proj remote".

## Additional Checks

- List symlinks with problems \[error\]
- Report on all symlinks in PROJ \[info\]
- Check for big binary files, not in raw/ \[warning\]

## Run all health checks and report their status \[info\]

This will list all of the current config var settings and what file
they are set in. Other statistics about the course will also be
listed, for example, the number of files in raw/ and the space they
use.

### Select actions to update configs or files

Define the config sections or files that you want to update or copy."

- (1) Back
- (2) Help
- (3) Quit, back to main
- (4) Copy all config section: core, alias, git-flow, gitproj.config
- (5) Copy some config section: git-flow, gitproj.config
- (6) Only copy gitproj.config section
- (7) Only copy gitproj.hooks section
- (8) Copy gitignore file
- (9) Copy pre-commit file
- Select the configs or file to be moved:

## Config File Level Direction

These options are for all of the config files actions.

Define what level to copy from and what level to copy to.

For example, if you select 'Product -> User', then that means sections
from /usr/share/doc/git-proj/config/gitconfig will be copied to
~/.gitconfig

The config 'levels' and files are more completely described in the
user-doc 'gitproj Configuration Documentation' (config.md

- (4) Product -> Use
- (5) User -> Project
- (6) Project -> Local
- (7) Local -> Project
- (8) Project -> User
- Select the from/to:

## Only copy missing or Force

If you select 'Force copy', then the variables in the 'from' file will
replace the variables in the 'to' file.

If you select 'Only copy missing', then existing variables, in the
'to' file, will not be replaced with variables in the 'from' file. But
missing variables will be copied from the 'from' file.

- (4) Force copy
- (5) Only copy missing
- Select the "force" option:

## Copy gitignore file

Define what level to copy from and what level to copy to.

For example, if you select 'Product -> User', then that means
/usr/share/doc/git-proj/config/gitignore will be merged to
~/.gitignore

An existing ~/.gitignore will be copied to ~/.gitignore.bak The
updated ~/.gitignore will be sorted with duplicates and comments
removed.

The config 'levels' and files are more completely described in the
user-doc 'gitproj Configuration Documentation' (config.md)

- (4) Product -> User
- (5) User -> Project
- (6) Project -> User
- Select the from/to:

## Copy pre-commit file

Define what level to copy from and what level to copy to.

For example, if you select 'Product -> User', then that means
/usr/share/doc/git-proj/hooks/pre-commit will be copied to
~/.pre-commit

An existing ~/.pre-commit will be copied to ~/.pre-commit.~1~

The config 'levels' and files are more completely described in the user-doc
'gitproj Configuration Documentation' (config.md)

- (4) Product -> Use
- (5) User -> Project
- (6) Project -> Local
- (7) Local -> Project
- (8) Project -> User
- Select the from/to:

## Continue?

Selecting 'Yes' will make the changes you have selected. If you do not
want to continue, then you can Quit, to return to the main menu, or
select the Back options to update your selections.

- (1) Back
- (2) Help
- (3) Quit, back to main
- (4) Yes, continue

    Are the above actions correct?

# OPTIONS

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details. Or "man git-proj" for help with all the
    sub-commands.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

# FILES

## Product Level

- ProductConfig is short for /usr/share/doc/git-proj/config/gitconfig
- ProductIgnore is short for /usr/share/doc/git-proj/config/gitignore
- ProductPreCommit is short for /usr/share/doc/git-proj/hooks/pre-commit

## User Level

- --global is short for ~/.gitconfig
- UserIgnore is short for ~/.gitignore
- UserPreCommit is short for  ~/.pre-commit

    (Note: this may change to: ~/.git/hooks/pre-commit)

## Project Level

- ProjectConfig is short for ProjName/.gitproj
- ProjectIgnore is short for ProjName/.gitignore
- ProjectPreCommit is short for ProjName/.pre-commit

## Local Level

- --local is short for ProjName/.git/config
- LocalPreCommit is short for ProjName/.git/hooks/pre-commit

# SEE ALSO

    git proj
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj set
    git proj status
    git proj add   TODO
    git proj config
    git flow

# NOTES

The checks and actions do not cover "all" of the configuration changes
that you might like. "git proj config" is just a helper tool to point
out problems and differences that might matter.

You are free to edit the files directly. Just make sure the bool and
int vars are syntactically OK. Also the Product level configs should
probably be left unchanged, so that git-proj updates will not
overwrite your changes. Put the changes you want to keep at the
\--global or Project Levels.

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
