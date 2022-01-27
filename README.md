# git-proj

## Documentation

`git-proj is a git subsystem` for managing large binary files.  Its
setup and management is simpler than "git LFS", because it doesn't
require a git server setup.

### git proj sub-commands

* `git proj` - general help.
* `git proj init` - initialize a local git-proj repo.
* `git proj-remote` - initialize a new "remote repo" from the local one.
* `git proj push` - push files in raw/ to the remote repo (and do a regular git push).
* `git proj pull` - pull raw/ files from the remote repo (and do a regular git pull).
* `git proj status` - git status and status of binary files in raw/ compared to the remote's raw/.
* `git proj clone` - create a git workspace and raw/ workspace from a remote.
* `git proj config` - TBD. Fix or change the git-proj configuration.
* `git proj add` - TBD. Add files and symlinks to the top raw/ directory.

### Installing

System requirements:

* OS: Any debian based OS. For example: Ubuntu, Debian, Mint, MX.

* Packages: git (>=2.17), bash, coreutils (fmt, tr), gawk (awk), git-flow,
  libpod-markdown-perl (pod2markdown), openssh-client, openssh-server,
  perl (pod2html, pod2man, pod2usage), rsync, sed, tidy. (If you use
  the apt-get package manager to install, all of these packages will
  be installed when git-proj is installed.)

* git subcommand directory /usr/lib/git-core exists.

Download Location:

* [https://moria.whyayh.com/rel/released/software/own/git-proj/deb](https://moria.whyayh.com/rel/released/software/own/git-proj/deb) -
when prompted, use guest/guest for User/Password.

Select the latest version (any OS). (the ones with 'RC' or 'test' in
the names are not stable.

Install Example

* apt-get install ./git-proj-0.4.1-1-mx-x86_64.deb

### Getting usage help

`man git-proj` - this will give you an overview of git-proj

`git proj -h` - output the overview and the usage help for all
of the sub-commands

`git proj [CMD] -h` - only output the help for "git proj CMD".
For example: git proj init -h

html and markdown help files can also be found in:
/usr/share/doc/git-proj/user-doc/

The Usage help for git-proj sub-command can be found in the
doc/user-doc/*.md files. The git-proj.md file gives and overview of
how git proj works and it also has the usage help for all of the
sub-commands. Or you can look at each sub command's file individually.

### Config files in /usr/share/doc/git-proj/config

* `gitconfig.default` - copied to `~/.gitconfig`, if none exists

* `gitproj.config.global` - copied to `~/.gitproj.config.global`, if none
  exists. An include path will be added to `~/.gitconfig`

* `gitignore.default` - copy to `PROJ/.gitignore` (version this file)

* `gitproj.config.local` - copy to `PROJ/.gitproj.config.local` and use
  PROJ/.gitproj.config.local to create any new
  `PROJ/.gitproj.config.HOSTNAME` file. (version these files)

* `PROJ/.gitproj.config.HOSTNAME` - include path in `PROJ/.git/config` will
  point to this file.

### pre-commit hooks

The file `/usr/share/doc/git-proj/hook/pre-commit` file will be copied
to `PROJ/.git/hooks/` by `git proj init` or `git proj clone`

See the vars in the `[gitproj "hook"]` section in
`PROJ/.gitproj.config.HOSTNAME` for controlling the pre-commit hooks.

## Source

[https://github.com/TurtleEngr/gitproj](https://github.com/TurtleEngr/gitproj)

----------

## For Developers

An outline of the directory structure and files in git-proj can be
found at: `test/dev-doc/outline.md`

Rather than have lots of loose scripts, Makefiles are used to collect
together the major processes in the code's maintenance--not just the
`build` process.

### Library functions in git-core/

* `gitproj-com.inc` - common functions used across many of the scripts

* `gitproj-[CMD].inc` - the main function are put in these files, so
testing is easier

* `git-proj-[CMD]` - these files only get the arguments and call the
functions in the corresponding `gitproj-[CMD].inc` file. These files
are the main entry point for git sub-commands. For example:
`git-proj-init` is run, on the command line, with `git proj init`

### Test scripts in test/

Every function should have unit-tests that check the inputs and
outputs of functions. Valid inputs and error states should be checked.

Some "mocking" might be done, but most tests are written so that need
for mocking is reduced.

* `test-com.sh`

* `test-com2.sh`

* `test-gitproj.sh`

* `test-[CMD].sh`

* `test-[CMD].log` - these file are used by the Makefile to collect the
output from a `test-[CMD].sh` script. If the log file is older than any
of its dependencies, then the corresponding `test-[CMD].sh` script will
be run.

* `test.inc` - this script has common function used by many tests to
setup the test's files and env. vars.

### Test Environment files (test-env*.tgz)

Currently these are symlinks to the directory above the git workspace.
These will be moved to a public space when the tool is mostly done, or
when automated testing is setup. An rsync or rclone step will be added when
the space is setup.

See the Makefile for how the tar-env*.tgz files are created and rebuilt.

* test-env.tgz@ - is manually built

## Style

The `fmt` target uses `shfmt` to normalize the indenting to 4 char.
The `rm-trailing-sp` is used to remove any trailing spaces in text
files. The pre-commit hook can be added (with the corresponding config
hooks) to look for trailing spaces, invalid file names, and large
binary files.

The `then` and `do` are on the same line as the opening statement.

For more details on the coding style for this product, see
`test/dev-doc/git-proj.md`

----------

## Notes

These are just some of the initial design notes that helped with
the implementation of this tool.

https://coderwall.com/p/bt93ia/extend-git-with-custom-commands

https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html

    source "$(git --exec-path)/git-sh-setup"
       require_work_tree_exists
       cd_to_toplevel
       require_clean_work_tree rebase "Please commit or stash them."

https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html

https://github.com/nvie/gitflow

https://github.com/nvie/gitflow/blob/develop/git-flow-init

https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks

https://git-scm.com/docs/githooks

TDD: https://github.com/kward/shunit2

## Design

### Move existing files to PROJ.raw/DIRS and make symlinks

This is an example of moving files in an existing structure and
creating symlinks so it looks like the files are still there.

    cd /home/video/seal
    mkdir ../seal.raw
    ln -s ../seal.raw raw
    cd edit/src/own/video
    mkdir -p  ../../../../../seal.raw/edit/src/own/video
    mv *.MP4 *.JPG *.jpg  ../../../../../seal.raw/edit/src/own/video
    ln -s ../../../../../seal.raw/edit/src/own/video/* .

Yuck. Just put all large binary files in PROJ/raw/ and fix the
references to them. If you really need to have files in some other
location, then make your own symnlinks to point the files in
raw/. Directories can be in raw/.
