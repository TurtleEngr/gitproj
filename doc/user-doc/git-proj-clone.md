<div>
    <hr/>
</div>

# NAME git proj clone

# SYNOPSIS

    git proj clone -d pRemoteDir [-y|-n] [common-options]

# DESCRIPTION

Clone a local git repo from the external pRemoteDir.

Use this script get a local copy of the remote git repo frome a
mounted drive.

# OPTIONS

- **-d pRemoteDir**

    TBD

- **-y|-n**

    If -y, then default to "yes" to all prompts to continue.

    If -n, then default to "no" to all prompts to continue.

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

    Run "git proj -h"  for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

The drive is mounted at: /mnt/usb-video and the the repo is
at: video-2019-04-01/trip.git So use this to get a local copy.

proj-get-local -d /mnt/usb-video/video-2019-04-01/trip.git

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