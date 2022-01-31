# git-proj

# Description

`git-proj implements git sub-commands` for managing large binary files.
Its setup and management is simpler than "git LFS", because it doesn't
require a git server.

## git proj sub-commands

* `git proj` - general help.
* `git proj init` - initialize a local git-proj repo.
* `git proj-remote` - initialize a new "remote repo" from the local one.
* `git proj push` - push files in raw/ to the remote repo (and do a regular git push).
* `git proj pull` - pull raw/ files from the remote repo (and do a regular git pull).
* `git proj status` - git status and status of binary files in raw/ compared to the remote's raw/.
* `git proj clone` - create a git workspace and raw/ workspace from a remote.
* `git proj config` - TBD. Fix or change the git-proj configuration.
* `git proj add` - TBD. Add files and symlinks to the top raw/ directory.

# User Docs

The user docs can be browsed online at:
[user-doc](https://github.com/TurtleEngr/gitproj/tree/develop/doc/user-doc)

I recommend you start with:
[git-proj](https://github.com/TurtleEngr/gitproj/blob/develop/doc/user-doc/git-proj.md)

Then the
[Create_a_git-proj_Repo](https://github.com/TurtleEngr/gitproj/blob/develop/doc/user-doc/tutorial/create_a_git-proj_repo.md)
tutorial shows the git-proj commands in action.

# Installing

## System requirements:

* OS: Any debian based OS. For example: Ubuntu, Debian, Mint, MX.

* Packages: git (>=2.17), bash, coreutils (fmt, tr), gawk (awk), git-flow,
  libpod-markdown-perl (pod2markdown), openssh-client, openssh-server,
  perl (pod2html, pod2man, pod2usage), rsync, sed, tidy. (If you use
  the apt-get package manager to install, all of these packages will
  be installed when git-proj is installed.)

* The git subcommand directory /usr/lib/git-core exists.

## Download Location:

* [https://moria.whyayh.com/rel/released/software/own/git-proj/deb](https://moria.whyayh.com/rel/released/software/own/git-proj/deb) -
when prompted, use guest/guest for User/Password.

Select the latest version (any OS). (the ones with 'RC' or 'test' in
the names are not stable.

Install Example:

    apt-get install ./git-proj-0.4.1-1-mx-x86_64.deb

## Getting usage help

`man git-proj` - this will give you an overview of git-proj

`git proj -h` - output the overview and the usage help for ALL
of the sub-commands

`git proj [CMD] -h` - output the help for "git proj CMD".
For example: "git proj init -h"

`git proj [CMD] -H usage` - output the short usage help for "git proj CMD".
For example: "git proj init -H usage". If a command requires options,
you can just type the command for the usage help.

After installation, html and markdown help files can also be found at:
/usr/share/doc/git-proj/user-doc/

# Source

[https://github.com/TurtleEngr/gitproj](https://github.com/TurtleEngr/gitproj)

----------

# For Developers

The developer docs can be found at:
[test/dev-doc](https://github.com/TurtleEngr/gitproj/tree/develop/test/dev-doc)
