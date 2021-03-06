#!/bin/bash

echo "For accurate counts 'make dist-clean' should be run before this."

cd ..

tProjStart=2021-10-23
tProjEnd=$(date +%F)
tProjDuration=$(dateutils.ddiff $tProjStart $tProjEnd)

tNumTests=$(grep '^test' test/test-*.sh | grep '()' | wc -l)
tNumAsserts=$(grep assert test/*.sh | wc -l)

# Includes all files, even most generated files.
# Excludes: binary files, package built files, and files I didn't write.
tTotalLines=$(cat $(find * -type f | grep -Ev 'package/dist|package/pkg|test/.ran-*|*.tgz|*.gif|shunit2*') | wc -l)

tLinesInTest=$(cat $(find test -type f) | wc -l)

tLinesInDoc=$(cat $(find doc -type f) | wc -l)
tWordsInDoc=$(cat $(find doc -type f) | wc -w)

tDoc=$(cat $(find * -prune -type f) | wc -l)
let tLinesInDoc+=tDoc
tDoc=$(cat $(find * -prune -type f) | wc -w)
let tWordsInDoc+=tDoc

tLinesInCode=$(cat $(find git-core -type f) | wc -l)
tDoc=$(
    awk '
        /=pod/,/=cut/ {
            print $0
        }
        /=internal-pod/,/=internal-cut/ {
            print $0
        }
    ' $(find git-core/* -type f) | wc -l
)
#let tLinesInDoc+=tDoc
let tLinesInCode-=tDoc

tDoc=$(
    awk '
        /=pod/,/=cut/ {
            print $0
        }
        /=internal-pod/,/=internal-cut/ {
            print $0
        }
    ' $(find test/* git-core/* -type f) | wc -w
)
let tWordsInDoc+=tDoc

tNumFun=$(grep '()' git-core/* | grep -Ev '#' | wc -l)
tNumCmds=$('ls' git-core/git-proj-* | wc -l)

let tLinesPerWeek=tTotalLines*7/tProjDuration

let tPagesInDoc=tWordsInDoc/250

# --------------------
cat <<EOF
Tests
        Number of tests:    $tNumTests
        Number of asserts:  $tNumAsserts
        Lines in test/:     $tLinesInTest
Doc
        Lines in doc:       $tLinesInDoc
        Word  in doc:       $tWordsInDoc
	Pages of doc:	    $tPagesInDoc
Code
        Number of git SubCmds: $tNumCmds
        Number of Functions:   $tNumFun
        Lines of code:         $tLinesInCode

Total for all:   $tTotalLines

Time: $tProjDuration days
Lines/Week: $tLinesPerWeek
EOF
