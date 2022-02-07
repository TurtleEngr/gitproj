# README for Developers

An outline of the directory structure and files in git-proj can be
found at: `test/dev-doc/outline.md`

This MindMap also show how the pieces relate to each other. Right
mouse click on the Mind Map image, and open itt in a new tab, get an
interactive version of this document. Double click on collapsed icons
to open/close sub-items.

[![mindmap](https://atlas.mindmup.com/bruceraf/git_proj_organization/thumb.png)](https://atlas.mindmup.com/bruceraf/git_proj_organization/index.html)

Rather than have lots of loose scripts, Makefiles are used to collect
together the major processes--not just the `build` process.

# Library functions in git-core/

* `gitproj-com.inc` - common functions used across many of the scripts

* `gitproj-[CMD].inc` - the main functions are put in these files, so
testing is easier

* `git-proj-[CMD]` - these files only get the arguments and call the
functions in the corresponding `gitproj-[CMD].inc` file. These files
are the main entry point for git sub-commands. For example:
`git-proj-init` is run, on the command line, with `git proj init`

# Test scripts in test/

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

## Test Environment files (test-env*.tgz)

These are symlinks to the directory above the git workspace.
The Makefile will copy any missing file from the release server.

See the Makefile for how the tar-env*.tgz files are created and rebuilt.

* test-env.tgz@ - is created manually.

# Documentation Files

The *.pod files are the master files for generating the *.html and
*.md files.

So if you see a pod file do not edit the corresponding html or md
files--they will be generated with "make gen-dev-doc"

Most of the open items in the TODO.md should be moved to
[issues](https://github.com/TurtleEngr/gitproj/issues)

----------

# Notes

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

# Design

## Move existing files to PROJ.raw/DIRS and make symlinks

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

