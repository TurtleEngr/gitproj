# Documentation Orgainizaton

* Try to structure the documentation in an order that users will most
  likely need.

* Make the documentation useful as a reference.

* Provide tutorials for typical scenarios, and advanced topics.

* Limit duplication - use cross references.

## User Documentation

File: github:/README.md

File: /usr/share/doc/git-proj/README.md - remove dev. doc. section
from top/README.md

File: /usr/share/doc/git-proj/README.html - gen: /usr/bin/markdown README.md

    Getting started
        download location
        install directions
    Usage help
        command line help (text)
            example
        local help files in html and markdown
            location
        remote help files - may have minor updates
            location
        tutorials
            location
            see create...
            see config...
    Support
        github issues

Dir: github:doc/user-doc/ - put the generated files in this dir, on a
version branch (release-M)

Dir: /usr/share/doc/git-proj/user-doc/

    File: git-proj.html - gen 'git-proj -H html, and git-proj-CMD
    File: git-proj.md - gen 'git-proj -H md', and git-proj-CMD
        NAME git proj
        SYNOPSIS - short usage help
        DESCRIPTION
        OPTIONS
            -OPT
            pSubCmd
            pSubCmdOpt
            head2 common-options

            head2 config - see File: config.*
                   ~/.gitconfig
                   top-dir/.gitproj
                   top-dir/.git/config
        RETURN VALUE
               return codes
               controlling output with -VN
        ERRORS
            describe format
        EXAMPLES
            see File: tutorial/create_a_git-proj_repo.*
        ENVIRONMENT
            see see File: config.* Config Variables section
        FILES
            see File: config.*
        SEE ALSO
        NOTES
        CAVEATS
        DIAGNOSTICS
        BUGS
            Support
        RESTRICTIONS
        AUTHOR
        HISTORY
            LICENSE
            CHANGES.md
    File: config.html - gen from src-doc/config.pod
    File: config.md - gen from src-doc/config.pod
        gitproj
            Config Variables
                env.var, default, config.vars, options
            Precedence
        System
            /usr/share/doc/git-proj/config/gitconfig
            /usr/share/doc/git-proj/config/gitignore
            /usr/share/doc/git-proj/hooks/pre-commit
        User (--global)
            ~/.gitconfig
            ~/.gitignore
        Project (--local)
            PROJ/.gitpro
            PROJ/.gitignore
            PROJ/.git/config
    Dir: subcommands/
        File: git-proj-CMD.html - gen from git-proj-CMD
        File: git-proj-CMD.md - gen from git-proj-CMD
    Dir: tutorials/
        File: create_a_git-proj_repo.html - gen from src-doc/create_a_git-proj_repo.pod
        File: create_a_git-proj_repo.md - gen from src-doc/create_a_git-proj_repo.pod

File: github:README.md - full version

Dir: github:test/dev-doc/

    File: git-proj.html - gen: 'git-proj -H int-html'
    File:git-proj.md - gen: 'git-proj -H int-md'

        Developer Documentation

    File: outline.md - annotated outline of all files

    Dir: enhancements/
        File: raw-cvs.md
        File: raw-pros-cons.md
        File: raw-rclone.md
        File: raw-rsnapshot.md
        File: raw-ssh.md
        File: version-wizard.md

