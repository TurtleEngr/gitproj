# Configuration Documentation

# gitproj

## Config Variables

        env.var, default, config.vars, options

## Precedence

# System

        /usr/share/doc/git-proj/config/gitconfig
        /usr/share/doc/git-proj/config/gitignore

# User (--global)

        ~/.gitconfig
        ~/.gitignore

# Project (--local)

        PROJ/.gitproj
        PROJ/.gitignore
        PROJ/.git/config

# pre-commit

        /usr/share/doc/git-proj/hooks/pre-commit
        PROJ/.git/hooks/pre-commit
