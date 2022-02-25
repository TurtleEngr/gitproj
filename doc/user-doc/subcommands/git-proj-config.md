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
