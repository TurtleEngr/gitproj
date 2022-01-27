#!/bin/bash

echo "For accurate counts 'make dist-clean' should be run before this."

cd ..

tProjStart=2021-10-23
tProjEnd=$(date +%F)
tProjDuration=$(dateutils.ddiff $tProjStart $tProjEnd)

tNumTests=$(grep '^test' test/test-*.sh | grep '()' | wc -l)
tNumAsserts=$(grep assert test/*.sh | wc -l)

tTotalLines=$(cat $(find * -type f | grep -Ev 'dist|subcommands') | wc -l)

tLinesInTest=$(cat $(find test -type f) | wc -l)

tLinesInDoc=$(cat $(find doc -type f) | wc -l)
tDoc=$(cat $(find * -prune -type f) | wc -l)
let tLinesInDoc+=tDoc

tLinesInCode=$(cat $(find git-core -type f) | wc -l)
tDoc=$(
    awk '
        /=pod/,/=cut/ {
            print $0
        }
        /=internal-pod/,/=internal-cut/ {
            print $0
        }
    ' $(find test/* git-core/* -type f) | wc -l
)
let tLinesInDoc+=tDoc
let tLinesInCode-=tDoc

tNumFun=$(grep '()' git-core/* | grep -Ev '#' | wc -l)
tNumCmds=$('ls' git-core/git-proj-* | wc -l)

let tLinesPerWeek=tTotalLines*7/tProjDuration

# --------------------
cat <<EOF
Tests
        Number of tests:    $tNumTests
        Number of asserts:  $tNumAsserts
        Lines in test/:     $tLinesInTest
Doc
        Lines of doc:       $tLinesInDoc
Code
        Number of git SubCmds: $tNumCmds
        Number of Functions:   $tNumFun
        Lines of code:         $tLinesInCode

Total for all:   $tTotalLines

Time: $tProjDuration days
Lines/Week: $tLinesPerWeek
EOF
