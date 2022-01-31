<div>
    <hr/>
</div>

# NAME git proj status

# SYNOPSIS

    git proj status [-g "pGitOpt"] [-r "pRawOpt"] [common-options]

# DESCRIPTION

    Do a "git status"
    Verify gitproj.config.remote-raw-origin is defined and mounted.

    Verify origin is set to a path that exists (if mounted).

    Give a "diff" (-qr) of the raw files, local vs remote (if mounted)

# OPTIONS

- **-g "pGitOpt"**

    Git status options. For example to give a "short" status and show
    ignored files, use::

        git proj status -g "-s --ignored"

- **-r "pRawOpt"**

    Raw options. Currently these options will be passed to the diff
    command. (This will be added after the default options: -qr) For
    example to show the files that are the same between local and remote:

        git proj status -r "-s"

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

# SEE ALSO

    git proj
    git proj init
    git proj remote
    git proj clone
    git proj push
    git proj pull
    git proj add   TODO
    git proj config TODO
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
