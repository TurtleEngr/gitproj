<div>
    <hr/>
</div>

# NAME git proj init

# SYNOPSIS

    git proj init local [-a] [-l pLocalDir] [-s pMaxSize] [-m] [-f] [common-options]

    Defaults: [-l $PWD] [-s 10K]

# DESCRIPTION

This will create a local git repo with branches. If git-flow is
installed can will optionally be setup too. After this setup the
remote git repo with "git proj init remote"

If there is a -a option, "git proj init local" will be run with all
the default options, which can be overridden with the options.

If there is no -a option, you will be prompted for the settings.  See
the OPTION section for details.

When the local and remote git repos are setup, all the setings
will be saved to ~/.gitproj.config and
\[project\]/gitproj/config.$HOSTNAME. Includes are put in ~/.gitconfig
and \[project\].git/config to point to the gitproj config files.

# OPTIONS

- **-a**

    The -a option will automattically run the whole init process with
    default settings. The options can be defined to override the default
    settings.

- **-l pLocalDir**

    Define the existing project directory. The last directory will be used
    for the name of the project. Default: current directory

        Dir (-l) [$PWD]? $gpLocalTopDir
            So the project Name will be: ${gpLocalTopDir##*/}

- **-s pMaxSize**

    Define the "size" for large binary files. Default 10K

        Size (-s) [10K]?

- **-m**

    These binary files greater than \[pSize\]  were found in your project dir:

        [file list]

    The listed files can be moved to the project's "raw" directory. Dirs
    will be created in the raw directory that correspond to the project's
    directory. A symlink will replace the moved file. The symlink will
    point to \[raw\].

    The symlinks are only provided for backward compatability; it would be
    best to remove those links and modifiy your code and apps to access
    the file directly from the raw directories.

        Move the files (-m) [y/n]?

- **-f**

    \[If git-flow is installed\]

        Setup git-flow (-f) [y/n]?

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

    Run "git proj -h"  for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# SEE ALSO

    git proj
    git proj remote
    git proj clone
    git proj add
    git proj push
    git proj pull
    git proj set
    git proj status
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
