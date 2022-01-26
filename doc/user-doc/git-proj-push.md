<div>
    <hr/>
</div>

# NAME git proj push

# SYNOPSIS

    git proj push [-g] [-d] [-a] [-y|-n] [common-options]

# DESCRIPTION

    rsync will be used top copy the '[ProjName]/raw/' files to
    [remote-raw-origin]/[ProjName].raw.

# OPTIONS

- **-g**

    If the -g option is given then run:

        "git push origin [current-branch]"

- **-d**

    If the -d option is used, then the remote raw/ will be made identical
    to the local raw/. I.e. files might be deleated from the remote raw/.

    See the EXAMPLES section for a "save" way to use this option.

- **-a**

    This turns on automated push for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option
    will be added. -d could be very distructive, so it needs to be
    run interactively.

- **-y|-n**

    If -y, then push files to the remote raw/

    If -n, then just show what would be pushed to the remote raw/

    Default: -n

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

The rsync (and rclone) updates of the files in raw/ are not versioned, so
be very careful with the -d option; files will be permanently deleted.

For a "safe" way, of updating the files in raw/, is to do a "push",
then "pull" with no -d. Then manually remove files you don't want in
the local raw/. Now run "push" with -d, to update the remote raw/

# SEE ALSO

    git proj
    git proj init
    git proj remote
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
