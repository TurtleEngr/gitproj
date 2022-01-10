<div>
    <hr/>
</div>

# NAME git proj status

# SYNOPSIS

    git proj status [-s] [common-options]

# DESCRIPTION

    Do a "git status"
    Verify gitproj.config.remote-raw-dir is defined and mounted
    Verify origin is set to a path that exists (if mounted)
    Give a "diff" (-qr) of the raw files, local vs remote (if mounted)

Check install related health

    fComGetVer
    Compare .git/hooks/pre-commit to $gpDoc/hooks/pre-commit
    if -v, show all the proj related config settings.
