<div>
    <hr/>
</div>

# NAME git proj init

# SYNOPSIS

    git proj init -l pLocalDir [-a] [-s pMaxSize] [-m] [-f]
                  [common-options]

# DESCRIPTION

This will create a local git repo with branches. If git-flow is
installed can be setup too. After "git proj init" is done, run
"git proj remote" to setup remote for git and raw files.

If there is a -a option, "git proj init" will be run with all the
default options, which can be overridden with other options.

If there is no -a option, you will be prompted for the settings.  See
the OPTION section for details.

When the local and remote git repos are setup, all the settings
will be saved to \[project\]/.git/config and \[project\]/.gitproj

# OPTIONS

- **-a**

    The -a option will automatically run the whole init process with
    default settings. The other options can be defined to override the
    default settings.

- **-l pLocalDir**

    Define the existing project directory. The last directory will be used
    for the name of the project. Required.

    Prompt:

        Dir (-l) [$PWD]? $gpLocalTopDir
            So the project Name will be: ${gpLocalTopDir##*/}

- **-s pMaxSize**

    Define the "size" for large binary files. Default 10K

    Prompt:

        Size (-s) [10K]?

- **-m**

    Prompt:

        Binary files greater than [pSize] were found in your project dir:
            [file list]

    The listed files can be moved to the project's "raw" directory. Dirs
    will be created in the raw directory that correspond to the project's
    directory. A symlink will replace the moved file. The symlink will
    point to \[raw\].

    The symlinks are only provided for backward compatibility; it would be
    best to remove those links and modify your code and apps to access
    the files directly from the raw directories.

    Prompt:

        Move the files (-m) [y/n]?

- **-f**

    If git-flow is installed.

    Prompt:

        Setup git-flow (-f) [y/n]?

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

    git proj init -l pLocalDir [-a] [-s pMaxSize] [-m] [-f]

For this example, you have a directory of files at:

    ~/project/bigsur-video/
        bigsur.kdenlive
        src/
            MVI_0224.MP4
            MVI_0225.MP4

You have started editing, when you realize you should version the
bigsur-vacation files. To do this automatically (-a, no prompts),
quietly (-q), move binary files greater than 10K (-m), and add
git-flow configs (-f).

    cd ~/project/bigsur-video
    git init -l $PWD -aqmf

This is what the bigsur-video/ will look like after:

    ~/project/bigsur-video/
        .gitproj
        .gitignore
        .pre-commit
        raw/
            src/
                MVI_0224.MP4
                MVI_0225.MP4
        .git/
            config (configs copied from .gitproj)
            hooks/
                pre-commit (copied from .pre-commit)
            [other-dirs]/
        bigsur.kdenlive
        src/
            MVI_0224.MP4 -> ../raw/MVI_0224.MP4
            MVI_0225.MP4 -> ../raw/MVI_0225.MP4

And if this is the first time you have run a git-proj command, these files
will be created (or merged with the files) in in your home dir:

    $HOME/
        .gitconfig (see the gitproj and gitflow sections)
        .gitignore
        .pre-commit

Or to be prompted, do this:

    cd ~/project/bigsur-video
    git init -l $PWD

Now you can used the usual git commands to save your changes for the
files that are not in raw/. To save all the files to an external
drive, see the **git proj remote** command (and the push/pull
commands).

# SEE ALSO

    git proj
    git proj init
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj status
    git proj config
    git proj add   TBD
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
