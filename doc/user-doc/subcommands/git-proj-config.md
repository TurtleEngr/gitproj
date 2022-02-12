<div>
    <hr/>
</div>

# NAME git proj config

# SYNOPSIS

    NOT IMPLEMENTED YET
    git proj config [common-options]

# DESCRIPTION

## Main Menu Options

- Run all health checks, only report problems. \[error\]
- Run all health checks, only report differences \[diff, warnings\]
- Run all health checks, report status \[info\]
- Select Global Actions
- Select Local Actions (offered only if in a PROJ)
- Select Other Actions

## Global Checks

These are done even if not in a git-proj managed workspace.

The Validate Checks, check $gpDoc/gitconfig and ~/.gitconfig. If there
is a problem they can be fixed by: 1) manually setting, 2) select
default, or 3) if ~/.gpconfig select from $gpDoc/gitconfig.

- Validate "facility" name \[error\]
- Validate "bin" and "doc" locations \[error\]
- Validate all bool vars are only true/false \[error\]
- Validate all int vars only have numbers \[error\]
- Compare ~/.gitconfig with $gpDoc/config/gitconfig \[diff\]
- Compare ~/.gitignore with $gpDoc/config/gitignore \[diff\]
- Compare ~/.pre-commit with $gpDoc/hooks/pre-commit \[diff\]
- if git-flow=true
    - is git-flow installed? \[warning\]
    - are git-flow section vars defined? \[warning\]

## Local Checks

These are done if you are in a git-proj managed workspace.

The Validate Checks, check PROJ/gitproj and --local. If
there is a problem they can be fixed by: 1) manfully setting, 2) select
from ~/.gitconfig, 3) select default

- If defined, validate "facility" name \[error\]
- If defined, validate "bin" and "doc" locations \[error\]
- Validate all gitproj bool vars are only true/false \[error\]
- Validate all gitproj int vars only have numbers \[error\]
- Compare PROJ/.gitproj with ~/.gitconfig (only list the diffs
from all gitproj sections and the diffs that are in .gitproj and also
in ~/.gitconfig \[diff\]
- Compare PROJ/.gitproj with PROJ/.git/config (only list the
diffs from all gitproj sections and the diffs that are in .gitproj and
also in ~/.gitconfig \[diff\]
- Compare PROJ/.gitignore with ~/.gitignore \[diff\]
- Compare PROJ/.pre-commit with ~/.pre-commit \[diff\]
- Compare PROJ/.git/hooks/pre-commit with PROJ/.pre-commit \[diff\]
- Note disabled pre-commit checks \[info\]
- prog-name != top dir name \[warning\]
- gitproj.config.local-status - is TBD \[error\]
- remote.origin.url - not defined
(git-proj-init needs to be run) \[error\]
- gitproj.config.remote-status - is TBD
(git-proj-remote needs to be run) \[error\]
- gitproj.config.remote-raw-origin - is TBD
(git-proj-remote needs to be run) \[warning\]
- warn if gitproj.config.remote.min-space is < 2x existing space \[warning\]
- if gitproj.config.remote-raw-origin, report if not found.
(suggest mounting it or running git-proj-remote) \[warning\]

## Additional Checks

- List symlinks with problems \[error\]
- Check for big binary files, not in raw/ \[warning\]
- Report on all symlinks in PROJ \[info\]

### Global Action Menus

- Update missing vars in ~/.gitconfig from $gpDoc/config/gitconfig?
- Force update of ALL gitproj sections from $gpDoc/config/gitconfig?
- Force update of "gitproj config" section from $gpDoc/config/gitconfig?
- Force update of "gitproj hooks" section from $gpDoc/config/gitconfig?
- Update ~/.gitignore from $gpDoc/config/gitignore?  (note this
will result in a sorted list, with duplicates removed) (only adds
missing from $gpDoc/config/.gitignore)
- Replace ~/.pre-commit from $gpDoc/hooks/pre-commit?
- Install/update git-flow and corresponding configs?

## Local Action Menus

- Fix remote.origin.url
- Change: gitproj.config.remote-raw-origin? (Best way: run
git-proj-remote)
- Force update of PROJ/.gitproj from --local
(core, git-flow, and gitproj sections only)
- Force update of --local from PROJ/.gitproj
(core, git-flow, and gitproj sections only)
- Update missing vars in PROJ/.gitproj and --local from
~/.gitconfig (gitproj sections only)
- Force update of ALL vars in PROJ/.gitproj and --local from
~/.gitconfig (remotes are not changed) (gitproj sections only) (and
vars in-common)
- Force update of ALL gitproj sections in PROJ/.gitproj and
--local from ~/.gitconfig (remotes are not changed)
- Force update of "gitproj config" section in PROJ/.gitproj and
--local from ~/.gitconfig (remotes are not changed)
- Force update of "gitproj hooks" section in PROJ/.gitproj and
--local from ~/.gitconfig
- Update ~/.gitignore from ~/.gitignore? (note this will result
in a sorted list, with duplicates removed) (only adds missing from
~/.gitignore)
- Replace PROJ/.pre-commit and PROJ/.git/hooks/pre-commit from
~/.pre-commit

## Additional Action Menus

- Set remote-min-space (1) set manually, 2) set from
~/.gitconfig, 3) default)
- Set the max size for commits of binary files.  (1) set
manually, 2) set from ~/.gitconfig, 3) default)

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

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr

# POD ERRORS

Hey! **The above document had some coding errors, which are explained below:**

- Around line 208:

    You forgot a '=back' before '=head2'
