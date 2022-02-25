<div>
    <hr/>
</div>

# NAME git proj remote

# SYNOPSIS

    git proj remote [-a] [-d pMountDir] [common-options]

# DESCRIPTION

This will create a remote git repo on an external drive.

(Future versions may support repos on remote computers, where you have
rsync access.)

# OPTIONS

- **-d pMountDir**

    This command is run after "git proj init" to setup a remote location.
    After this is setup, "git proj push" and "git proj pull" can be used
    to push/pull git and raw file changes.

    Export the git repo to an external drive (or another local dir) This
    is usually the removable drive's "top" directory.  Ideally the top
    directory should be different across a set of external drives, so that
    the local "origin" can be used to make sure the proper git repo is
    found on the drive. The git "origin" will be set to
    pMountDir/ProjName.git And "remote-raw-origin" will be set to
    pMountDir/ProjName.raw

- **\[common-options\]**

        -h                     (-H text)
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)
        -y | -n                (only used with -a option)

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

# SEE ALSO

    git proj
    git proj init
    git proj clone
    git proj add
    git proj push
    git proj pull
    git proj config
    git proj status
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
