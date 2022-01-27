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

    Git status options. For example:

        git proj status -g "-s --ignored"

- **-r "pRawOpt"**

    Raw options. Currently these will be options passed to the diff
    command. (This will be added after the default options: -qr) For
    example:

        git proj status -r "-s"

- **\[common-options\]**

        -h
        -H pStyle
        -q, -v, -V N     (gpVerbose)
        -x, -xx..., -X N (gpDebug)

    Run "git proj -h" for details. Or "man git-proj" for help with all the
    subcommands.

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
    git proj add   TBD
    git proj config TBD
    git flow

# AUTHOR

TurtleEngr

# HISTORY

GPLv3 Copyright 2021 by TurtleEngr
