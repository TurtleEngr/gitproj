#!/bin/bash

fUsage()
{
    cat <<EOF
Usage:
    fix-rel-links.sh FILE.[html|md]

Convert: LINK{TEXT|PATH.[html|md]} to

html: <a href="PATH.html">TEXT</a>

md: [TEXT](PATH.md)

EOF
    exit 1
} # fUsage

fFixLink()
{
    local pFile=$1
    local pType=$2
    local tTmp

    tTmp=file.tmp

    awk -v pType=$pType '
/LINK{.*\|.*}/ {
    match($0, /LINK{(.*)\|(.*)}/, tPart)
    if (tPart[1] == "" || tPart[2] == "") {
        print $0;
        next;
    }
    tText = tPart[1];
    tPath = tPart[2];
    tFile = tPath;
    tExt = "." pType;
    tFound = sub(/\.[^.]*$/,"",tFile)
    if (! tFound)
        tExt = "";
    if (pType == "html")
        sub(/LINK{.*\|.*}/,"<a href=\"" tFile tExt "\">" tText "</a>");
    else
        sub(/LINK{.*\|.*}/,"[" tText "](" tFile tExt ")");
    print $0;
    next;
}
{ print $0; }
    ' <$pFile >$tTmp

    if [ ! -s $tTmp ]; then
        echo "Error in fix-rel-links.sh [$LINENO]"
        exit 1
    fi
    mv $tTmp $pFile
} # fFixLink

# ========================================
if [[ $# -eq 0 || "x$1" = "x-h" ]]; then
    fUsage
fi

for tFile in $*; do
    if [ ! -f $tFile ]; then
        echo "Not a file, skipping $tFile [$LINENO]"
        continue
    fi
    if ! $(file $tFile | grep -q ' text'); then
        echo "Not a text file, skipping $tFile [$LINENO]"
        continue
    fi
    [[ $tFile =~ \.(html|md) ]]
    if [ -z "${BASH_REMATCH[0]}" ]; then
        echo "Not a html or md file, skipping $tFile [$LINENO]"
        continue
    fi
    tType=${BASH_REMATCH[0]}
    tType=${tType#.}
    if ! grep -q LINK $tFile; then
        # No LINK in file, so skip
        continue
    fi

    fFixLink $tFile $tType
done
