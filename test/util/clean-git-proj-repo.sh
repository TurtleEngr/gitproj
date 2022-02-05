#!/bin/bash
# clean-git-proj-repo.sh

pMaxNum=${1:-10}

cRelPath=/rel/development/software/own/git-proj/deb

if [ $pMaxNum -le 1 ]; then
    exit 1
fi
if [ ! -d $cRelPath ]; then
    exit 1
fi
if ! cd $cRelPath; then
    exit 1
fi

tNumFile=$('ls' git-proj-* | wc -l)
if [ $tNumFile -le $pMaxNum ]; then
    exit 0
fi

(( tNumRm = tNumFile - pMaxNum ))
if [ ${tNumRm:-0} -le 0 ]; then
    exit 1
fi

rm -vf $('ls' -tr git-proj-* | head -n $tNumRm)
exit $?
