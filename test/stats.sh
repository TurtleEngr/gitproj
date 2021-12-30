#!/bin/bash

tProjStart=2021-10-23
tProjEnd=$(date +%F)
tProjDuration=$(dateutils.ddiff $tProjStart $tProjEnd)

tNumTests=$(grep '^test' test-*.sh | grep '()' | wc -l)
tNumAsserts=$(grep assert *.sh | wc -l)
tLinesTest=$(cat Makefile *.md *.sh *.inc | wc -l)
tLinesTestDoc=$(
    awk '
    	/=pod/,/=cut/ {
            print $0
        }
    	/=internal-pod/,/=internal-cut/ {
            print $0
    	}
    ' * | wc -l
)
let tOnlyTest=tLinesTest-tLinesTestDoc

tLinesCode=$(
    cat \
        ../doc/VERSION \
        ../doc/config/* ../doc/hooks/* \
        ../git-core/* |
        wc -l
)
tLinesCodeDoc=$(
    awk '
    	/=pod/,/=cut/ {
            print $0
        }
    	/=internal-pod/,/=internal-cut/ {
            print $0
        }
    ' ../git-core/* | wc -l
)
let tOnlyCode=tLinesCode-tLinesCodeDoc

tNumFun=$(grep '()' ../git-core/* | grep -Ev '#' | wc -l)
tNumCmds=$('ls' ../git-core/git-proj-* | wc -l)

tTotalLines=$(
    cat \
        Makefile *.md *.sh *.inc \
        ../doc/VERSION \
        ../doc/config/* ../doc/hooks/* \
        ../git-core/* |
        wc -l
)
tTotalLinesDoc=$(
    awk '
    	/=pod/,/=cut/ {
            print $0
        }
    	/=internal-pod/,/=internal-cut/ {
            print $0
        }
    ' * ../git-core/* | wc -l
)
let tNoDoc=tTotalLines-tTotalLinesDoc

let tLinesPerWeek=tTotalLines*7/tProjDuration

# --------------------
cat <<EOF
Tests
	Number of tests:    $tNumTests
	Number of asserts:  $tNumAsserts
	Lines of test code: $tOnlyTest
	Lines of doc. in test code: $tLinesTestDoc
	Total lines of test code:   $tLinesTest
Code
	Number of git SubCmds: $tNumCmds
	Number of Functions:   $tNumFun
	Lines of code:         $tOnlyCode
	Lines of doc. in code: $tLinesCodeDoc
	Total lines of code:   $tLinesCode

Total non-doc code: $tNoDoc
Total lines of doc: $tTotalLinesDoc
Total for all:      $tTotalLines

Time: $tProjDuration days
Lines/Week: $tLinesPerWeek
EOF
