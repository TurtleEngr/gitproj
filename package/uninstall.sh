#!/bin/bash

if [ $# -ne 1 ]; then
    cat <<EOF
Usage:
    ./uninstall TopDir

Example:
    ./uninstall /
    ./uninstall ../dist

Paths are relative to package/ dir.
EOF
    exit 1
fi
pRoot=$1

if [ "$pRoot" = "/" ]; then
    if [ "$(whoami)" != "root" ]; then
        echo "You need to be root to uninstall from '/'"
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

rm $pRoot/usr/lib/git-core/git-proj
rm $pRoot/usr/lib/git-core/git-proj-*
rm $pRoot/usr/lib/git-core/gitproj-*.inc

rm -rf $pRoot/usr/share/doc/git-proj
