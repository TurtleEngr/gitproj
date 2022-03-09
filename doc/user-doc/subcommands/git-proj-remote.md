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

This example direcory is what you have after running **git init**:

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

You want to save these files to an external drive so that they are
backed up and so that you can clone them to another computer. Or to
restore them, if you remove the project from your computer.

For this example, assume the external drive is mounted at /mnt/usb-drive/
and that there is a top directory "video-proj" on it. This is the quick
quiet way of defining the remote location for the project files:

    cd ~/project/bigsur-video/
    git proj remote -aqd /mnt/usb-drive/video-proj

This is what will be created on the mounted drive:

    /mnt/usb-drive/video-proj/
        bigsur-video.raw/
            src/
                MVI_0224.MP4
                MVI_0225.MP4
        bigsur-video.git/
            config
            hooks/
                pre-config
            [other-dirs]/

    git config --get remote.origin.url
    outputs:
    /mnt/usb-drive/video-proj/bigsur-video.git

    git config --get gitproj.config.remote-raw-origin
    outputs:
    /mnt/usb-drive/video-proj/bigsur-video.raw

See **git proj push/pull** for how to push or pull files to/from the
external drive.

# SEE ALSO

    git proj init
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj status
    git proj add
    git proj config
    git flow
    

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
