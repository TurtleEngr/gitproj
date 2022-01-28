# gitproj Configuration Documentation

# Precedence

There are over 20 config variables for managing the git-proj
subcommands. Their "scope" is defined by the file they are
in. Definitions "closer" to the project directory have higher
precedence. Think of this as a sieve, with variables looked for in the
following order, where the last definition wins.

       Scope           Access    Locaton
    1. System          --file    /etc/gitconfig
       gitproj, global --file    /usr/share/doc/config/gitconfig
    2. Global, user    --global  ~/.gitconfig
       gitproj, local  --file    PROJ/.gitproj
    3. Local, proj     --local   PROJ/.git/config
    4. Worktree        --file    PROJ/.git/config.worktree

If the "Access" is defined for "set" or "get", then that is the only
location "git config" will use.

git-proj doesn't do anything with "/etc/config" or
".git/config.worktree".

## System, /etc/gitconfg

    /etc/gitconfg

Currently gitproj does not do anything with this file. Variables that
should be set for all users on a system can be put in this file. Since
gitproj defines all of the gitproj variables in a user's ~/.gitconfig,
the /etc/gitconfig changes will not have any impact on the gitproj
variables.

## gitproj, global, /usr/share/doc/git-proj/config/gitconfig

    L</usr/share/doc/git-proj/config/gitconfig|../config/gitconfig>

gitproj default variable values for all users and projects are defined
in this file. The values are only copied at selected times and unless
specified, they will only add missing vars, not changing user or
project vars.

This file is mainly used when creating an initial project with "git proj init" or "git proj clone". It is also used by "git proj config".

The /usr/share/doc/git-proj/config/gitconfig file is not used directly
by "git config".  It is only used to setup the ~/.gitconfig file.

When a new version of gitproj is installed, new vars and new default
values may be added. Package manager options can be used to replace or
keep the existing config file (see man dpkg --force-confdef,
\--force-confnew, or --force-confold).

    [core]
        # These options work will between Linux and Windows
        filemode = true
        autocrlf = false
        ignorecase = true
        excludesFile = ~/.gitignore
        logAllRefUpdates = true
        quotePath = false
        trustctime = false
    [user]
        # This is required!
        name = YourName
        email = YourEmail@example.com
    [alias]
        # Add, remove, or change these
        br = branch
        br-all = branch -r
        ci = commit
        co = checkout
        df = diff
        origin = config --get remote.origin.url
        raw-origin = config --get remote-raw-origin
        push-tags = push --tags origin
        revert-all = reset --hard HEAD
        st = status
        top = rev-parse --show-toplevel
        tracked = remote show origin
    [color]
        # Change if desired
        ui = never
    [branch "develop"]
        # Change if desired
        mergeoptions = --log
    [gitflow "branch"]
        master = main
        main = main
        develop = develop
    [gitflow "prefix"]
        feature = feature/
        release = release/
        hotfix = hotfix/
        bugfix = bugfix/
        support = support/
    [gitproj "config"]
        proj-status = installed
        syslog = false
        facility = user
        bin = /usr/lib/git-core
        doc = /usr/share/doc/git-proj
        git-flow-pkg = git-flow
        git-flow = true         # set by init in PROJ/.gitroj
        # The TBDs are changed when in PROJ/.gitroj and PROJ/.git/config
        local-status=TBD        # set by init and clone
        local-host=TBD          # set by init and clone
        proj-name = TBD         # set by init and clone
        remote-status=TBD       # set by remote and clone
        remote-raw-origin=TBD   # set by remote and clone
        remote-min-space = 20g  # set by remote and clone
        verbose = 2
    [gitproj "hook"]
        verbose = true
        pre-commit-enabled = true
        check-file-names = true         # only these ch in names: [-_.a-zA-z0-9]
        check-in-raw = true             # don't allow ci to PROJ/raw/?
        check-whitespace = true         # don't allow trailing spaces
        check-for-tabs = false          # don't allow tabs for tab-ext-list
        tab-ext-list = c sh md html pod
        check-for-big-files = true      # don't allow files > binary-file-size
        binary-file-size = 10k

## Global, user, --global, ~/.gitconfig

    ~/.gitconfig
    ~/.gitignore

If the ~/.gitconfig is not defined, it will be copied from
/usr/share/doc/config/gitconfig. Also if ~/.gitignore is not defined,
it will be copyied from /usr/share/doc/config/gitignore.

"git proj init" and "git proj clone" will update ~/.gitconfig
from /usr/share/doc/config/gitconfig, for any vars that are not
defined in ~/.gitconfig. Existing variables will not be chnaged.

## gitproj, local, PROJ/.gitproj

    PROJ/.gitproj
    PROJ/.gitignore

The PROJ/.gitproj file is not used directly by "git config".  It is
only used to setup the initial PROJ/.git/config file.

"git proj init" will create PROJ/.gitproj from the gitproj sections in
~/.gitconfig (also filling in "missing" vars from
/usr/share/doc/config/gitconfig). PROJ/.gitproj should be versioned,
for use by git proj clone

"git proj clone" will add vars from PROJ/.gitproj to PROJ/.git/config
afer the files are created. If PROJ/.gitproj is missing, it will
create it with the "init" process.

git-proj commands that change gitproj section vars in PROJ/.git/config
will also change the vars in PROD/.gitproj, so that later clones can
update PROJ/.git/config.

## Local, proj, --local, PROJ/.git/config

    PROJ/.git/config

"git proj init" and "git proj clone" will copy all of the
vars in PROJ/.gitproj to PROJ/.git/config.

## /usr/share/doc/git-proj/hooks/pre-commit, ~/.pre-commit, PROJ/.pre-commit

    L</usr/share/doc/git-proj/hooks/pre-commit|../hooks/pre-commit>
    ~/.pre-commit (optinal)
    PROJ/.pre-commit (optinal)
    PROJ/.git/hooks/pre-commit

The pre-commit hook file is created with a precidence similar to the
config vars.

"git proj init" and "git proj clone" will create the
PROJ/.git/hooks/pre-commit file. If PROJ/.pre-commit exists, that file
will be used. If ~/.pre-commit exits, that file will be used. And
finally the /usr/share/doc/git-proj/hooks/pre-commit file will be
used.

# Config Variables and Environment Variables

These are globals and vars that effect how the script runs. Just about
all of the env. vars. that begin with "gp" can be set and exported
before the script is run. That way you can set your own defaults, by
putting them in your ~/.bashrc or ~/.bash\_profile files.

Global variable precedence (the last one to set the gp variable,
wins).  (Internally, this is not implemented in this order, but this
is the logical result.)

    * internal hardcoded default
    * variable in ~/.gitconfig
    * variable in PROJ/.git/config
    * env. var.
    * command line option

## Notation: gp\[Var\]; -\[A\]; git.config.\[var\]; (default)

    gp[Var];          - a global env. var. name (NA means there is none)
    -[A];             - a CLI option (-NA means there is none)
    git.config.[var]; - git config var name (NA means there is none)
    (default)         - its default value, if not defined anywhere

## gpAuto; -a; NA; (false)

This is used to run the scripts in batch mode.

## gpAutoMove; -m; NA; (false)

This it used by the "git proj init" command.

## gpYesNo; -y; -n; NA; (No)

If gpAuto is true, then gpYesNo can be used to define default answers.

## NA; -NA, gitproj.config.proj-status; (TBD)

Once the gitproj package is installed and configured, this will be
changed to "installed"

## gpSysLog; -NA; gitproj.config.syslog; (false)

If set to 0, log messages will only be sent to stderr.

If set to 1, log messages will be sent to stderr and syslog.

Default: false

## gpFacility; -NA; gitproj.config.facility; (user)

Log messages sent to syslog will be sent to the "facility" specified
by by gpFacility.

"user" log messages will be sent to /var/log/user.log, or
/var/log/syslog, or /var/log/messages.log

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

## gpBin; -NA; gitproj.config.bin; (/usr/lib/git-core)

The location of the executing command will override this.

## gpDoc; -NA; gitproj.config.doc; (/usr/share/doc/git-proj)

If not found, then set to: $gpBin/../doc  If still not found: error.

## gpDebug; -x, -xx..., -X N; NA; (0)

See the "common-options" section in [git-proj](https://metacpan.org/pod/git-proj.html) for details.

There is no config variable for gpDebug.

## NA; -NA, gitproj.config.git-flow-pkg, (git-flow)

## gpGitFlow; -NA; gitproj.config.flow; (true)

If true, git-flow will be setup for the project.

## NA; -NA, gitproj.config.local-status; (TBD)

When a project has been successfully setup by git proj init or
git proj clone, this will be set to "defined" in PROJ/.gitproj and
PROJ/.git/config.  If should always be "TBD" in ~/.gitconfig

## NA; -NA, gitproj.config.local-host; (TBD)

When a project has been successfully setup by git proj init or
git proj clone, this will be set to "$HOSTNAME" in PROJ/.git/config
only.

## gpProjName; -NA, gitproj.config.proj-name; (TBD)

When a project has been successfully setup by git proj init or
git proj clone, this will be set to the project's name in
PROJ/.gitproj and PROJ/.git/config. Usually this is the top direcory
name. If should always be "TBD" in ~/.gitconfig

## NA; -NA, gitproj.config.remote-status; (TBD)

When git proj remote runs OK this is changed to "defined", in
PROJ/.gitproj and PROJ/.git/config. If should always be "TBD" in
~/.gitconfig

## gpRemoteRawOrigin, -d, gitproj.config.remote-raw-origin, (TBD)

This is set by git proj remote and git proj clone to the remote
raw location. It is set in PROJ/.git/config. If should always be "TBD"
in ~/.gitconfig and PROJ/.gitproj.

## gpRemoteMinSpace; -NA; gitproj.config.remote-min-space; (20g)

This is the minium space that should be available on the external
drive.  The git proj remote command will not continue if there is
not enough space.  The available space should be at least twice the
size of the space used by ProjName.raw.

## gpVerbose; -q, -v, -V N; gitproj.config.verbose; (2)

See the "common-options" section in [git-proj](https://metacpan.org/pod/git-proj.html) for details.

## gpVer, -NA; gitproj.config.ver, ()

This is set by git proj init in the PROJ/.gitproj and
PROJ/.git/config files. The value will be the currently installed
version of gitproj. The version is defined in the file:
/usr/share/doc/git-proj/VERSION

git proj clone will compare ver in PROJ/.gitproj to
/usr/share/doc/git-proj/VERSION. If the major or minor numbers of the
checked out project are greater than the installed version there could
be problems. If the installed version is greater, this should not be a
problem, because gitproj tried to be backward compatable. And it might
try to upgrade the project.

A warning message will be output if there are differences.

## gpHookVerbose; -NA; gitproj.hook.verbose; (true)

If false, there will be output from pre-commit only if problems are
identified.

## gpPreCommitEnabled; -NA; gitproj.hook.pre-commit-enable; (true)

Use this to turn all pre-commit check off.

## gpCheckFileNames; -NA; gitproj.hook.check-file-names; (true)

If true, only these chacters can be used in file and directory names:
\[-\_.a-zA-z0-9\]

## gpCheckInRaw; -NA; gitproj.hook.check-in-raw; (true)

If true, files in the top raw/ dir will not be alloed in the git
repository. The files should have been ignored with the rule in
.gitignore, but maybe someone "forced" the add.

Setting this to "false" defeats the whole purpose for gitproj.

## gpCheckWhitespace; -NA; gitproj.hook.check-whitespace; (true)

If true, don't allow trailing spaces in text files.

See rm-trailing-sp to fix.

## gpAllowTabs; -NA; gitproj.hook.check-for-tabs; (false)

If true, don't allow tabs for files with the extentions found in:
gitproj.hook.tabs-ext-list

To fix, use "rm-trailing-sp -f FILE"

## gpTabExtList; -NA; gitproj.hook.tabs-ext-list; (c sh md html pod)
	tab-ext-list = c sh md html pod

List the file extentions for filse that will be checked for tabs.

## gpCheckForBigFiles; -NA; gitproj.hook.check-for-big-files; (true)

If true, look for large binary files. Ones greater than
gitproj.hook.binary-file-size will not be allowed into the git repo.

## gpMaxSize; -i, --int; gitproj.hook.binary-file-size; (10k)

Valid suffixes: k, m, g

If no suffix, then bytes are assumed.
