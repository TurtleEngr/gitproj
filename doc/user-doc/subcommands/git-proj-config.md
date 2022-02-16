<div>
    <hr/>
</div>

# NAME git proj config

# SYNOPSIS

    NOT IMPLEMENTED YET
    git proj config [common-options]

# DESCRIPTION

This is text menu driven tool for checking and tuning up your git-proj
configurations. There are are 4 configuration"levels" to consider:
Product, User, Project, and Local. The standard git support configs
at the User and Local levels, and the usual --global and --local options
can be used with "git config" to get and set variables. Or you can edit
the files directly. See the FILES section for files listed by their levels.

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

## Main Menu Options

- 1) Quit

    In lower levels #1 will change to "Back" and Quit will move to #3.

- 2) Help

    This will be available in all menus.

- 3) Run all health checks, only report problems. \[errors\]
- 4) Run all health checks, only report differences \[diff, warnings\]
- 5) Run all health checks, report status \[info\]
- 6) Select Global User Level Actions
- 7) Select Local Project Level Actions (offered only if in a PROJ)
- 8) Select pre-commit Hook Actions (offered only if in a PROJ)
- 9) Select Other Actions

## Global User Level Checks

These are done even if not in a git-proj managed workspace.

The Validate Checks, check ProductConfig and --global. If there
is a problem they can be fixed by: 1) manually setting, 2) select
default, or 3) if --global select from ProductConfig.

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

## Local and Project Level Checks

These are done if you are in a git-proj managed workspace.

The Validate Checks, check ProjectConfig and --local. If there is a
problem they can be fixed by: 1) manfully setting, 2) select from
\--global, 3) select default

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
- Check for big binary files, not in raw/ \[warning\]
- Report on all symlinks in PROJ \[info\]

### Global User Level Action Menus

- 1) Back
- 2) Help
- 3) Quit
- 4) ProductConfig -> --global Only update missing vars
- 5) ProductConfig -> --global Force update of all gitproj vars
- 6) ProductIgnore -> UserIgnore (only add missing)

    Note: this will result in a sorted list, with duplicates removed.

- 7) Install/update git-flow and corresponding configs
- \* TBD? Install/update git-flow and corresponding configs

## Local and Project Level Action Menus

- 4) --local -> ProjectConfig (Force core, git-flow, and gitproj.config sections only)
- 5) --global -> ProjectConfig and --local (Update missing vars in gitproj.config sections only)
- 6) --global -> ProjectConfig and --local (Force update of all gitproj.config section, vars in-common) (remotes are not changed)
- 7) UserIgnore -> ProjectIgnore (only adds missing items)

## pre-commit Hook Action Menus

- 4) ProductPreCommit -> UserPreCommit
- 5) --global -> ProjectConfig and --local (Force update of 'gitproj hooks' section)
- 6) UserPreCommit -> ProjectPreCommit and LocalPreCommit
- 7) --local -> ProjectConfig (Force update of 'gitproj.hooks' section)
- 8) LocalPreCommit -> ProjectPreCommit
- 9) --local -> --global (Force update of 'gitproj.hooks' section)
- 10) LocalPreCommit -> UserPreCommit

## Other Action Menus

- 4) Set remote-min-space

    Fix options: 1) set manually, 2) set from --global, 3) default

- 5) Set the max size for commits of binary files.

    Fix options: 1) set manually, 2) set from ~/.gitconfig, 3) default

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

- ProjectConfig is short for $gpProjName/.gitproj
- ProjectIgnore is short for $gpProjName/.gitignore
- ProjectPreCommit is short for $gpProjName/.pre-commit

## Local Level

- --local is short for $gpProjName/.git/config
- LocalPreCommit is short for $gpProjName/.git/hooks/pre-commit

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

# POD ERRORS

Hey! **The above document had some coding errors, which are explained below:**

- Around line 203:

    Expected text after =item, not a bullet

- Around line 219:

    You forgot a '=back' before '=head2'

- Around line 221:

    '=item' outside of any '=over'

- Around line 235:

    You forgot a '=back' before '=head2'
