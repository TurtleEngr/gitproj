# git-proj

# Description

git-proj implements git sub-commands` for managing large binary files.
Its setup and management is simpler than "git LFS", because it doesn't
require a git server.

[![test](https://github.com/TurtleEngr/gitproj/actions/workflows/test.yml/badge.svg)](https://github.com/TurtleEngr/gitproj/actions/workflows/test.yml)
[![package](https://github.com/TurtleEngr/gitproj/actions/workflows/package.yml/badge.svg)](https://github.com/TurtleEngr/gitproj/actions/workflows/package.yml)

## Why git-proj?

Have you ever had a developer commit their whole home directory into
your project's git repository (on purpose or accidentally)? I have
seen this. That includes their .ssh/ dir with their ssh keys. They
better have passwords on their private keys! Someone making this
mistake will likely have password-less keys. Yikes, what a security
mess--all their keys, and shared keys, will need to be changed on
all the systems they access.

Sure you can "delete" the mistake, but if it was pushed to your main
git server, it will be in the repo forever, unless you do the work
needed to rip it out. Yeah, there are tools that will do it, but they
are not trivial, and your repo should be "locked" for a few hours
until it is repaired. Maybe a faster solution: create a new repo, from
a cleaned up HEAD on all active branches, and delete the compromised
repo. Yeah, faster, but you've lost all your change history.

That is one scenario. This tool won't help with removing large binary
files, but it can help with a more common scenario. Beginning
developers don't realize they shouldn't be committing object files,
exe files, log files, video files, or generated PDFs to the
repository. Your snappy repo will start to become huge and
*slow*. There are some good reasons for wanting to save and track
those kind of files, but a git repo is not the place. So git-lfs was
created. If you are OK with that solution and don't mind paying for
the storage, you can skip this tool.  I wanted something simpler to
setup and with a more distributed approach. This tool is a start down
that path.

The main inspiration for this git-proj tool came from my desire to use
git to version video projects. But video files are HUGE, yet they
don't change much. Since most video files are rendered from video
editing tools, it is only important to version the video editor files,
so that you can recreate a particular video file. Since rendering
takes time you will want to save some of the rendered files. But there
is usually no need to save every rendered version.

Currently this tool only supports git and large files on a local
system. The "remotes" can be setup on mounted drives, and they can be
used to clone the files to other systems.

See the
[issues](https://github.com/TurtleEngr/gitproj/issues?q=is%3Aopen+is%3Aissue+milestone%3A%222.0+Release%22),
milestone 2.0 tagged issues for future enhancements that will support
remotes across a network.

# git proj sub-commands

* `git proj` - general help.
* `git proj init` - initialize a local git-proj repo.
* `git proj-remote` - initialize a new "remote repo" from the local one.
* `git proj push` - push files in raw/ to the remote repo (and do a regular git push).
* `git proj pull` - pull raw/ files from the remote repo (and do a regular git pull).
* `git proj status` - git status and status of binary files in raw/ compared to the remote's raw/.
* `git proj clone` - create a git workspace and raw/ workspace from a remote.
* `git proj config` - Fix or change the git-proj configuration files.
* `git proj add` - TBD. Add files and symlinks to the top raw/ directory.

[<img src="image/git-proj_User_DFD.png" width="600"/>](https://docs.google.com/presentation/d/15JqVxkBczpMKwtJI64FNU-2ixbXCh2ijwS3vfIBt4ko/edit#slide=id.p)

## User Docs

The user docs can be browsed online at:
[user-doc](https://github.com/TurtleEngr/gitproj/tree/main/doc/user-doc)

I recommend you start with:
[git-proj](https://github.com/TurtleEngr/gitproj/blob/main/doc/user-doc/git-proj.md)

Then the
[Create_a_git-proj_Repo](https://github.com/TurtleEngr/gitproj/blob/main/doc/user-doc/tutorial/create_a_git-proj_repo.md)
tutorial shows how to get started with the git-proj commands.

For a full understanding of all the configuration variable and their
"levels", see
[gitproj Configuration Documentation](https://github.com/TurtleEngr/gitproj/blob/main/doc/user-doc/config.md).

# Installing

## System requirements:

* OS: Any debian based OS. For example: Ubuntu, Debian, Mint, MX.

* Packages: git (>=2.17), bash, coreutils (fmt, tr), gawk (awk), git-flow,
  libpod-markdown-perl (pod2markdown), openssh-client, openssh-server,
  perl (pod2html, pod2man, pod2usage), rsync, sed, tidy. (If you use
  the apt-get package manager to install, all of these packages will
  be installed when git-proj is installed.)

* The git sub-command directory /usr/lib/git-core exists.

## User requirements:

* Have used more than just `git clone`

* You have used the git CLI a lot (not just git in an IDE)

## Download Locations:

* The stable versions can be found at:
[https://moria.whyayh.com/rel/released/software/own/git-proj/deb](https://moria.whyayh.com/rel/released/software/own/git-proj/deb) -
when prompted, use guest/guest for User/Password.

* The test packages (ones that have at least passed all tests) can be found at:
[https://moria.whyayh.com/rel/development/software/own/git-proj/deb](https://moria.whyayh.com/rel/development/software/own/git-proj/deb)

[![test](https://github.com/TurtleEngr/gitproj/actions/workflows/test.yml/badge.svg)](https://github.com/TurtleEngr/gitproj/actions/workflows/test.yml)
[![package](https://github.com/TurtleEngr/gitproj/actions/workflows/package.yml/badge.svg)](https://github.com/TurtleEngr/gitproj/actions/workflows/package.yml)

Select the latest version (any OS). The ones with 'RC' or 'test' (and
timestamps) in the names are not "stable".

### Download example:

    tVer=0.7.1-1
    tPkg=git-proj-$tVer-mx-x86_64.deb
    tUrl=https://moria.whyayh.com/rel/released/software/own/git-proj/deb
    tOpt="--user=guest --password=guest"
    wget $tOpt $tUrl/$tPkg

### Install Example:

    sudo apt-get install ./$tPkg

# Getting more usage help

The user docs can be browsed online at:
[user-doc](https://github.com/TurtleEngr/gitproj/tree/main/doc/user-doc)

After installation, html and markdown help files and tutorials can
also be found at: /usr/share/doc/git-proj/user-doc/

Also you can get help with these commands:

`man git-proj` - this will give you an overview of git-proj

`git proj -h` - output the overview AND usage help for ALL of the
git-proj sub-commands.

`git proj [CMD] -h` - output the help for "git proj CMD".
For example: "git proj init -h"

`git proj [CMD] -H usage` - this outputs just the short usage help for
"git proj CMD".  For example: "git proj init -H usage". If a command
requires options, you can just type the command for the usage help.

# Source

[https://github.com/TurtleEngr/gitproj](https://github.com/TurtleEngr/gitproj)

----------

# For Developers

The developer docs can be found at:
[test/dev-doc](https://github.com/TurtleEngr/gitproj/tree/develop/test/dev-doc)
