#!/bin/bash

if [ $# -ne 1 ]; then
    cat <<EOF
Usage:
    ./install TopDir

Example:
    ./install /
    ./install ../dist

Paths are relative to package/ dir.
EOF
    exit 1
fi
pRoot=$1

if [ "$pRoot" = "/" ]; then
    if [ "$(whoami)" != "root" ]; then
        echo "You need to be root to install to '/'"
        exit 1
    fi
    echo "Are you sure? Press enter to continue, ^C to quit. "
    read
else
    if [ ! -w $pRoot ]; then
        echo "You don't have write permission for $pRoot, or it is missing."
        exit 1
    fi
fi

mkdir -p $pRoot/usr/lib/git-core
rsync -a ../git-core/* $pRoot/usr/lib/git-core/git-proj/

mkdir $pRoot/usr/share/doc/git-proj
rsync -a ../doc/* $pRoot/usr/share/doc/git-proj
