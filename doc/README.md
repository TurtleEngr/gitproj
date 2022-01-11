# git-proj

## Documentation

git-proj is a git subsystem for managing large binary files.
Its setup and management is simpler than "git LFS".

The Usage help for git-proj sub-command can be found in the
doc/user-doc/*.md files. The git-proj.md file gives and overview of
how git proj works and it also has the usage help for all of the
subcommands. Or you can look at each sub command files individually.

### git proj sub-commands

* `git proj` - general help
* `git proj init` - initialize a local git-proj repo
* `git proj-remote` - initialize a new "remote repo" from the local one
* `git proj push` - push files in raw/ to the remote repo (and do a regular git push)
* `git proj pull` - pull raw/ files from the remote repo (and do a regular git pull)
* `git proj status` - git status and status of binary files in raw/ compared to remote raw/
* `git proj clone` - create a git workspace and raw/ workspace from a remote
* `git proj config` - TBD. This fixup or change the git-proj configuration

### Getting usage help

`man git-proj` - this will give you an overview of git-proj

`git proj -h | more` - output the overview and the usage help for all
of the subcommands

`git proj [CMD] -h | more` - only output the help for "git proj CMD".
For example: git proj init -h | more

html and markdown help files can also be found in:
/usr/share/doc/git-proj/user-doc/

### Config files in /usr/share/doc/git-proj/config

* gitconfig.default - copied to ~/.gitconfig, if none exists

* gitproj.config.global - copied to ~/.gitproj.config.global, if none
  exists. An include path will be added to ~/.gitconfig

* gitignore.default - copy to PROJ/.gitignore (version this file)

* gitproj.config.local - copy to PROJ/.gitproj.config.local and use
  PROJ/.gitproj.config.local to create any new
  PROJ/.gitproj.config.HOSTNAME file. (version these files)

* PROJ/.gitproj.config.HOSTNAME - include path in PROJ/.git/config will
  point to this file.

### pre-commit hooks

The file /usr/share/doc/git-proj/hook/pre-commit file will be copied
to PROJ/.git/hooks/ by `git proj init` or `git proj clone`

See the vars in the [gitproj "hook"] section in PROJ/.gitproj.config.HOSTNAME
for controlling the pre-commit hooks.

## Source

    https://github.com/TurtleEngr/gitproj

----------

