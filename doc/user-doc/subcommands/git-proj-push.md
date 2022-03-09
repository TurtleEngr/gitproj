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
    to the local raw/. I.e. files might be deleted from the remote raw/.

    See the EXAMPLES section for a "safe" way to use this option.

- **-a**

    This turns on automated push for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option
    will be added. -d could be very destructive, so it needs to be
    run interactively.

- **-y|-n**

    If -y, then push files to the remote raw/

    If -n, then just show what would be pushed to the remote raw/

    Default: -n

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

# EXAMPLES

The rsync (or rclone) updates of the files in raw/ are not versioned, so
be very careful with the -d option; files will be permanently deleted.

For a "safe" way, of updating the files in raw/: do a "push", then
"pull" with no -d. Then manually remove files you don't want, in the
local raw/ dir. Now run "push" with -d, to update the remote raw/

For example:

    cd PROJ
    mount REMOTE-DRIVE

    # Update remote with all local raw/ files
    git proj push 

    # Get all remote raw/ files
    git proj pull

    # Review and remove fiies from local, preparing for permanent removal
    cd raw
    rm FILES

    # Remove raw/ files from remote, that are not in local raw/ dir
    git proj push -d

Note: Because is is so destructive, the -d option cannot be used with
the -a option. Also the -y option will be ignored--you must answer the
"delete?" prompt.

To quickly and quietly push files to raw and git you would run this

    git proj push -gay

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
