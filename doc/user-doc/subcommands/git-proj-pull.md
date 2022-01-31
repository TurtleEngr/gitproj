<div>
    <hr/>
</div>

# NAME git proj pull

# SYNOPSIS

    git proj pull [-g] [-d] [-a] [-y|-n] [common-options]

# DESCRIPTION

rsync will be used top copy the \[remote-raw-origin\]/\[ProjName\].raw/ files
to '\[ProjName\]/raw/'.

If the -g option is given then run:

    "git pull origin [current-branch]"

# OPTIONS

- **-g**

    If the -g option is given then run:

        "git push origin [current-branch]"

- **-d**

    If the -d option is used, then the local raw/ will be made identical
    to the remote raw/. I.e. files might be deleted from the local raw/.

    See the help EXAMPLES section, in "git proj push", for a "save" way to
    use this option.

- **-a**

    This turns on automated push for raw/ files. Use the -y or -n to
    select the action. -n will just display what would be done.

    If there is a -d option, that will be removed and the -n option
    will be added. -d could be very destructive, so it needs to be
    run interactively.

- **-y|-n**

    If -y, then pull files from the remote raw/

    If -n, then just show what would be pulled from the remote raw/

- **\[common-options\]**

        -h
        -H usage|text|html|md|man|int|int-html|int-md
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details.

# RETURN VALUE

    0 - if OK
    !0 - if errors

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
