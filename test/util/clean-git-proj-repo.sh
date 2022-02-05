#!/bin/bash
# clean-git-proj-repo.sh

pMaxNum=${1:-10}

cRelPath=/rel/development/software/own/git-proj/deb

if [ $pMaxNum -le 2 ]; then
    return 1
fi
if [ ! -d $cRelPath ]; then
    return 1
fi

tNumFile=$('ls' $cRelPath | wc -l)
if [ ${tNumFile:-0} -le $cMaxNum ]; then
    return 0
fi

(( tNumRm = tNumFile - cMaxNum))
if [ ${tNumRm:0} -le 0 ]; then
    return 1
fi

rm -v $('ls' -tr | head -n $tNumRm)
return $?
