<div>
    <hr/>
</div>

# NAME git proj push

# SYNOPSIS

    git proj push [-b] [-y|-n] [common-options]

# DESCRIPTION

    rsync will be used top copy the '[ProjName]/raw/' files to
    [remote-raw-origin]/[ProjName].raw.

If the -b option is given then run:

    "git push origin [current-branch]"

# OPTIONS

- **-b**
- **-y|-n**

    If -y, then default to "yes" to all prompts to continue.

    If -n, then default to "no" to all prompts to continue.

- **\[common-options\]**

        -h
        -H pStyle
        -v, -vv
        -x, -xx

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
