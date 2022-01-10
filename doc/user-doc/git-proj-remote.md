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

    Export the git repo to an external drive (or another local dir) This
    is usually the removable drive's "top" directory.  Ideally the top
    directory should be different across a set of external drives, so that the
    local "origin" can be used to make sure the proper git repo is found
    on the drive. "origin" will be set to $pMountDir/ProjName.git

    After adding and committing files, run this script to copy this git
    repo to a mounted drive (or to another local directory).

    A mounted drive should have top directory that is different from other
    drives so that the repo can be found with it's "origin" name.

    For example, with a mount point: /mnt/usb-video create the remote git
    at the top directory video-2019-04-01, with:

        git proj init -e /mnt/usb-video/video-2019-04-01

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
