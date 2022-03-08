# README for Developers

An outline of the directory structure and files in git-proj can be
found at: **test/dev-doc/outline.md**

The following MindMap also show how the pieces relate to each
other. To get an interactive outline, right mouse click on the MindMap
image, and open it in a new tab. Double click on collapsed icons to
open/close sub-items.

<div>
    <a target="_blank" href="https://atlas.mindmup.com/bruceraf/git_proj_organization/index.html"><img src="https://atlas.mindmup.com/bruceraf/git_proj_organization/thumb.png"/></A>
</div>

Rather than have lots of loose scripts, Makefiles are used to collect
together the major processes--not just the **build** process.

# Quick Start

See test.yml and package.yml in
[.github/workflows/](https://github.com/TurtleEngr/gitproj/tree/develop/.github/workflows)
for a quick list of commands for the testing, and packaging commands.

This is a list of the commands found there:

    git clone git@github.com:TurtleEngr/gitproj.git
    cd gitproj/test
    make install-deps mk-ssh
    make test-all
    # If all pass, the you can package
    cd ../package
    make first
    make clean build package

In package/pkg/ you should see the package(s). EPM supports a number
of "native" packages: bsd, deb, rpm, and macos.

# Dependencies for Development

To install all the dependencies: **cd test; make install-deps mk-ssh**

The "first" target in
[package/Makefile](https://github.com/TurtleEngr/gitproj/blob/develop/package/Makefile)
lists the required commands.

The main
[README.md](https://github.com/TurtleEngr/gitproj/blob/develop/README.md)
file for this repo lists the required packages for the gitproj script
(see section: Installing, System requirements, Packages).

## EPM

The package process requires the **epm** and **epm-helper**
packages.  These have the commands: epm, mkepmlist, patch-epm-list,
and mkver.pl.  The packages can be found at:
[/rel/released/software/ThirdParty/epm/](https://moria.whyayh.com/rel/released/software/ThirdParty/epm/)

- Download and install epm version 5.0.2 or larger from the mx19/ or
ubuntu18/ directories.
- Download and install epm-helper version 1.5.1 or larger.

The GitHub code for making the epm packages can be found at:
[github.com/TurtleEngr/epm-deb-pkg](https://github.com/TurtleEngr/epm-deb-pkg)
[github.com/TurtleEngr/epm-helper](https://github.com/TurtleEngr/epm-helper)

The actual EPM source code can be found at:
[https://github.com/jimjag/epm](https://github.com/jimjag/epm)

### Why EPM?

From the EPM intro:

"Software distribution under UNIX/Linux can be a challenge, especially
if you ship software for more than one operating system. Every
operating system provides its own software packaging tools and each
has unique requirements or implications for the software development
environment."

"The ESP Package Manager ("EPM") is one solution to this
problem. Besides its own "portable" distribution format, EPM also
supports the generation of several vendor-specific formats.  This
allows you to build software distribution files for almost any
operating system \*from the same sources.\*"

I have used EPM since 3/2000, to build packages for Red Hat, Suse,
OSX, and Debian based distributions. All with this one packaging tool!
Sure it doesn't support all of the "unique" features offered by each
different OS, but it has more than 90% of what is needed, and I didn't
have to learn (or code for) all of the nuances of different
packagers. If you really need a customization that is unique to a
particular OS, then use pre and post install scripts to manage the
differences. My style is to select the needed scripts when the package
is built--that keeps the scripts simple and tuned only for that OS.

# Library functions in git-core/

- **gitproj-com.inc** - has common functions that are used across many
of the scripts.
- **gitproj-\[CMD\].inc** - files have the main functions for the
commands. This is done so that testing is easier.
- **git-proj-\[CMD\]** - these files only get the arguments and call the
functions in the corresponding **gitproj-\[CMD\].inc** files. These files
are the main entry point for git sub-commands. For example:
**git-proj-init** is run, on the command line, with **git proj init**

# Test scripts in test/

Every function should have unit-tests that check the inputs and
outputs of functions. Valid inputs and error states should be checked.
If a function can only be tested "in production", then refactor the
code so that it can be tested! (In a QA/Release/Operations roles, I
have had developers say some of the code can only be tested in
production. That is B.S. What I hear? They don't want to do the work
to be a professional engineer.)

Some "mocking" might be done, but most tests are written so that the
need for mocking is reduced. Without mocking, the tests will be more
fragile. So? It is better to have fragile tests than fragile code.

- **test-com.sh**
- **test-com2.sh**
- **test-gitproj.sh**
- **test-\[CMD\].sh**
- **test-\[CMD\].log** - these files are used by the Makefile to collect the
output from a **test-\[CMD\].sh** script. If the log file is older than any
of its dependencies, then the corresponding **test-\[CMD\].sh** script will
be run. These files are NOT versioned.
- **test.inc** - this script has common function used by many tests to
setup the test's files and env. vars.

## Test Environment files (test-env\*.tgz)

These are symlinks to the directory above the git workspace. The
symlinks are versioned.  The Makefile will copy any missing files from
the release server. (There really is not need for these symlinks. The
tests could be refactored to just get the files from "../..")

See the Makefile for how the tar-env\*.tgz files are created and
rebuilt.

- **test-env.tgz** - was manually created.

# Documentation Files

The \*.pod files are the master files for generating the \*.html and
\*.md files.

So if you see a pod file \*do not edit the corresponding html or md
files\*--they will be generated with "make gen-dev-doc"

Most of the open items in the TODO.md should be moved to
[issues](https://metacpan.org/pod/<https:#github.com-TurtleEngr-gitproj-issues)

\----------

# Initial Notes

These are just some of the initial design notes that helped with
the implementation of this tool.

[https://coderwall.com/p/bt93ia/extend-git-with-custom-commands](https://coderwall.com/p/bt93ia/extend-git-with-custom-commands)

[https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html](https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html)

    source "$(git --exec-path)/git-sh-setup"
       require_work_tree_exists
       cd_to_toplevel
       require_clean_work_tree rebase "Please commit or stash them."

[https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html](https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html)

[https://github.com/nvie/gitflow](https://github.com/nvie/gitflow)

[https://github.com/nvie/gitflow/blob/develop/git-flow-init](https://github.com/nvie/gitflow/blob/develop/git-flow-init)

[https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)

[https://git-scm.com/docs/githooks](https://git-scm.com/docs/githooks)

TDD: [https://github.com/kward/shunit2](https://github.com/kward/shunit2)

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
location, then make your own symnlinks to point to the files in
raw/. Directories can be in raw/.

# POD ERRORS

Hey! **The above document had some coding errors, which are explained below:**

- Around line 115:

    You forgot a '=back' before '=head1'
