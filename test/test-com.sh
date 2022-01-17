#!/bin/bash

# ========================================
fUsage()
{
    fComUsage -s usage -f $cTestCurDir/test-com.sh

    # This is the start of the testing internal documentation. See:
    # fGitProjComInternalDoc()
    return

    cat <<\EOF >/dev/null
=pod

=for text ========================================

=for html <hr/>

=head1 test-com.inc

test-com.inc - Common functions used in the test scripts

=head1 SYNOPSIS

    ./test.com.sh [test,test,...]

=head1 DESCRIPTION

shunit2.1 is used to run the unit tests. If no test function names are
listed, then all of the test functions will be run.

=head1 RETURN VALUE

0 - if OK

=head1 ERRORS

=head1 EXAMPLES

=head1 ENVIRONMENT

=head1 FILES

=head1 SEE ALSO

shunit2.1

=head1 NOTES

=head1 CAVEATS

=head1 DIAGNOSTICS

=head1 BUGS

=head1 RESTRICTIONS

=head1 AUTHOR

=head1 HISTORY

=cut

EOF
}

# ========================================

# --------------------------------
NAoneTimeSetUp()
{
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-head2 Test gitproj-com.inc

=internal-head3 oneTimeSetuUp

Currently this records all of the script's expected initial global
variable settings, defined in fComSetGlobals. If different, adjust the
tests as needed.

Env Var

 HOME - this is set to the test user's home dir
 gpUnitDebug - this can be manually set to 1 in unit test functions.

Calls:

 $gpBin/gitproj-com.inc
 fComSetGlobals

=internal-cut
EOF
} # oneTimeSetUp

NAoneTimeTearDown()
{
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
} # oneTimeTearDown

# --------------------------------
setUp()
{
    # Restore default global values, before each test
    unset gpBin cPID gpCmdVer gErr gpFacility gpSysLog gpVerbose
    fTestSetupEnv
    fTestCreateEnv
    gpUnitDebug=0
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 setUp

Before each test runs, this restores all of the script's initial
global variable settings,

=internal-cut
EOF
} # setUp

tearDown()
{
    git config --global --remove-section gitproj.testit >/dev/null 2>&1
    gpUnitDebug=0
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
    return 0
} # tearDown

# ========================================

# --------------------------------
testSetup()
{
    assertTrue "$LINENO" "[ -x $gpBin/gitproj-com.inc ]"
    assertTrue "$LINENO" "[ -x $gpBin/git-proj ]"
    return 0

    assertTrue "$LINENO" "[ -r $cTestFiles ]"
    assertTrue "$LINENO" "[ -d $cTestSrcDirg ]"
    assertTrue "$LINENO" "[ -d $cTestDestDir ]"
    assertNotEquals "$LINENO" "$cHome" "$HOME"
    assertTrue "$LINENO" "[ -r $HOME/$cConfigGlobal ]"
    assertTrue "$LINENO" "[ -r $HOME/.gitproj-test.config ]"

    for i in $cDatMount1 $cDatMount2 $cDatMount3; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i##*/}" "[ -d $i ]"
    done
    for i in $cDatProj1 $cDatProj2; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -d $HOME/$i ]"
    done
    for i in $cDatProj1Big; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -r $HOME/$cDatProj1/$i ]"
    done
    for i in $cDatProj2Big $cDatProj2Big; do
        fTestDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -r $HOME/$cDatProj2/$i ]"
    done
}

# --------------------------------
testComInitialConfig()
{
    local tProg
    local tResult

    assertNotNull "$LINENO $gpBin" "$gpBin"
    assertTrue "$LINENO -d $gpBin" "[ -d $gpBin ]"
    assertTrue "$LINENO -x $gpBin/gitproj-com.inc" "[ -x $gpBin/gitproj-com.inc ]"

    assertNotNull "$LINENO $gpDoc" "$gpDoc"
    assertTrue "$LINENO -d $gpDoc" "[ -d $gpDoc ]"

    assertEquals "$LINENO" "0" "$gpDebug"
    assertEquals "$LINENO" "0" "$gpVerbose"
    assertEquals "$LINENO" "false" "$gpSysLog"
    assertEquals "$LINENO" "user" "$gpFacility"
    assertEquals "$LINENO" "0" "$gErr"
    assertNull "$LINENO" "$(echo $gpCmdVer | tr -d '.[:digit:]')"

    for tProg in logger pod2text pod2usage pod2html pod2man pod2markdown tidy awk tr rsync; do

        assertTrue "$LINENO missing: $tProg" "$(
            which $tProg >/dev/null 2>&1
            echo $?
        )"
    done
    return 0

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 testInitialConfig

Verify all of the global variables are correctly defined. Look for
"ADJUST" comment for tests that might need to be changed for your
script.

=internal-cut
EOF
} # testInitialConfig

# --------------------------------
testComFirstTimeSet()
{

    # source gitproj-init.inc calls fInitSetGlobals, which calls
    # fComFirstTimeSet, which creates these files.
    #assertFalse "$LINENO" "[ -f $HOME/.gitconfig ]"
    #assertFalse "$LINENO" "[ -f $HOME/.gitproj.config.global ]"

    fComFirstTimeSet
    assertTrue "$LINENO" "[ -f $HOME/.gitconfig ]"
    assertTrue "$LINENO" "[ -f $HOME/.gitproj.config.global ]"
    assertTrue "$LINENO" "$(
        grep -q path $HOME/.gitconfig
        echo $?
    )"
    return 0
} # testComFirstTimeSet

# --------------------------------
testComPreProjSetGlobals()
{
    assertTrue "$LINENO" "[ -d $HOME/$cDatProj1 ]"
    cd $HOME/$cDatProj1
    fComPreProjSetGlobals

    assertEquals "$LINENO" "$cGitProjVersion" "$gpVer"

    assertEquals "$LINENO" "false" "$gpSysLog"
    assertEquals "$LINENO" "user" "$gpFacility"
    assertEquals "$LINENO" "0" "$gpAuto"
    assertEquals "$LINENO" "${PWD##*/}" "$gpProjName"
    assertEquals "$LINENO" "${gpLocalTopDir}/raw" "$gpLocalRawDir"
    assertEquals "$LINENO" "10k" "$gpMaxSize"
    assertEquals "$LINENO" "false" "$gpGitFlow"
    assertNull "$LINENO" "$gpAction"

    cd - >/dev/null 2>&1
    return 0
} # testComPreProjSetGlobals

# --------------------------------
testComLog_MultiplePermutations()
{
    local tMsg
    local tLevel
    local tLine
    local tErr
    local tResult
    local tTestMsg

    # Check the format, for a number of settings
    gpSysLog=false
    gpVerbose=0
    gpDebug=0
    tMsg="Testing 123"
    tLine="458"
    tErr="42"

    for gpSysLog in false true; do
        for gpVerbose in 0 1 2; do
            for gpDebug in 0 1 2; do
                for tLevel in alert crit err warning notice info debug debug-1 debug-3; do
                    echo -n '.' 1>&2
                    tTestMsg="l-$gpSysLog.v-$gpVerbose.d-$gpDebug.$tLevel.fLog"
                    fTestDebug " "
                    fTestDebug "Call: fLog -p $tLevel -m \"$tMsg\" -l $tLine -e $tErr"
                    tResult=$(fLog -p $tLevel -m "$tMsg" -l $tLine -e $tErr 2>&1)
                    fTestDebug "tResult=$tResult"

                    if [ $gpVerbose -eq 0 ] && echo $tLevel | grep -Eq 'notice|info'; then
                        assertNull "$LINENO tcl1-$tTestMsg not notice,info" "$tResult"
                        continue
                    fi
                    if [ $gpVerbose -eq 1 ] && echo $tLevel | grep -Eq 'info'; then
                        assertNull "$LINENO tcl1-$tTestMsg not info" "$tResult"
                        continue
                    fi
                    if [ $gpVerbose -eq 2 ] && echo $tLevel | grep -Eq 'info'; then
                        assertNotNull "$LINENO tcl1-$tTestMsg info" "$tResult"
                        continue
                    fi
                    if [ $gpDebug -eq 0 ] && [ "${tLevel%%-*}" = "debug" ]; then
                        assertNull "$LINENO tcl2-$tTestMsg not debug" "$tResult"
                        continue
                    fi
                    if [ $gpDebug -lt 2 ] && [ "$tLevel" = "debug-2" ]; then
                        assertNull "$LINENO tcl3-$tTestMsg not debug-2" "$tResult"
                        continue
                    fi
                    if [ $gpDebug -lt 3 ] && [ "$tLevel" = "debug-3" ]; then
                        assertNull "$LINENO tcl4-$tTestMsg not debug-3" "$tResult"
                        continue
                    fi
                    assertContains "$LINENO tcl5-$tTestMsg.name" "$tResult" "$gpCmdName"
                    assertContains "$LINENO tcl6-$tTestMsg.level" "$tResult" "$tLevel:"
                    assertContains "$LINENO tcl7-$tTestMsg.msg" "$tResult" "$tMsg"
                    assertContains "$LINENO tcl8-$tTestMsg.line" "$tResult" '['$tLine']'
                    assertContains "$LINENO tcl9-$tTestMsg.$tLevel.err" "$tResult" '('$tErr')'
                done # tLevel
            done     # gpDebug
        done         # gpVerbose
    done             # gpSysLog

    echo 1>&2
    return

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 testLog

Test fLog and fLog.

=internal-cut
EOF
} # testLog

# --------------------------------
testComSysLog()
{
    local tErr
    local tLevel
    local tLine
    local tMsg
    local tResult
    local tTestMsg

    # ADJUST? This is dependent on your syslog configuration.
    export tSysLog=/var/log/user.log
    #export tSysLog=/var/log/messages.log
    #export tSysLog=/var/log/syslog

    # Check syslog
    gpSysLog=true
    gpVerbose=0
    tMsg="Testing 123"
    #for tLevel in emerg alert crit err warning; do
    for tLevel in alert crit err warning; do
        echo -n '.' 1>&2
        tTestMsg="$tLevel.fLog"
        fTestDebug " "
        fTestDebug "Call: fLog -p $tLevel -m \"$tMsg\""
        tResult=$(fLog -p $tLevel -m "$tMsg" 2>&1)
        fTestDebug "tResult=$tResult"
        assertContains "$LINENO tcl11-$tTestMsg" "$tResult" "$tLevel:"
        tResult=$(tail -n1 $tSysLog)
        fTestDebug "syslog tResult=$tResult"
        assertContains "$LINENO tcl12-$tTestMsg" "$tResult" "$tLevel:"
        assertContains "$LINENO tcl13-$tTestMsg" "$tResult" "$tMsg"
    done
    echo 1>&2
    return

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 testSysLog

Test fLog and fLog, and verify messages are in a syslog file.

=internal-cut
EOF
} # testSysLog

# --------------------------------
testComErrorLog()
{
    local tSrc=${BASH_SOURCE##*/}
    local tMsg
    local tLevel
    local tLine
    local tErr
    local tResult
    local tTestMsg

    gpUnitDebug=0
    gpSysLog=false
    gpVerbose=0
    local tMsg="Testing 123"
    local tLine="458"
    for gpSysLog in false true; do
        echo -n '.' 1>&2
        tTestMsg="l-$gpSysLog.fError"
        fTestDebug " "
        fTestDebug "Call: fError -m \"$tMsg\" -l $tSrc:$tLine"
        tResult=$(fError -m "$tMsg" -l $tSrc:$tLine 2>&1)
        fTestDebug "tResult=$tResult"
        assertContains "$LINENO $tTestMsg.name" "$tResult" "$gpCmdName"
        assertContains "$LINENO $tTestMsg.crit" "$tResult" "crit:"
        assertContains "$LINENO $tTestMsg.msg" "$tResult" "$tMsg"
        assertContains "$LINENO $tTestMsg.line" "$tResult" '['$tSrc:$tLine']'
        assertContains "$LINENO $tTestMsg.err" "$tResult" '(1)'
        assertContains "$LINENO $tTestMsg.usage" "$tResult" "Usage"
        assertNotContains "$LINENO" "$tResult" "Internal:"
    done
    gpUnitDebug=0
    return

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 testErrorLog

Test fError and fError.

=internal-cut
EOF
} # testErrorLog

# --------------------------------
testComUsage()
{
    local tResult
    local tUsageScript=$gpTest/test-com.sh
    local tInternalScript=$gpBin/gitproj-com.inc

    gpUnitDebug=0

    #-----
    tResult=$(fComUsage -s usage -f $tUsageScript 2>&1)
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO tcu-short" "$tResult" "Usage"

    #-----
    tResult=$(fComUsage -s foo -f $tUsageScript 2>&1)
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO tcu-s-foo.1" "$tResult" "DESCRIPTION"
    assertContains "$LINENO tcu-s-foo.2" "$tResult" "HISTORY"

    #-----
    tResult=$(fComUsage -f $tUsageScript -s 2>&1)
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO tcu-s-null.1" "$tResult" "crit: Internal: fComUsage: Value required"

    #-----
    tResult=$(fComUsage -s long -f $tUsageScript 2>&1)
    assertContains "$LINENO tcu-long.1" "$tResult" "DESCRIPTION"
    assertContains "$LINENO tcu-long.2" "$tResult" "HISTORY"

    #-----
    tResult=$(fComUsage -s man -f $tUsageScript 2>&1)
    assertContains "$LINENO tcu-man.1" "$tResult" '.IX Header "DESCRIPTION"'
    assertContains "$LINENO tcu-man.2" "$tResult" '.IX Header "HISTORY"'

    #-----
    tResult=$(fComUsage -s html -f $tUsageScript -t "$gpCmdName Usage" 2>&1)
    assertContains "$LINENO tcu-html.1" "$tResult" '<li><a href="#DESCRIPTION">DESCRIPTION</a></li>'
    assertContains "$LINENO tcu-html.2" "$tResult" '<h1 id="HISTORY">HISTORY</h1>'
    assertContains "$LINENO tcu-html.3" "$tResult" "<title>$gpCmdName Usage</title>"
    #assertContains "$LINENO $tResult" "$tResult" "Show tResult"

    #-----
    tResult=$(fComUsage -s md -f $tUsageScript 2>&1)
    assertContains "$LINENO tcu-md.1" "$tResult" '# DESCRIPTION'
    assertContains "$LINENO tcu-md.2" "$tResult" '# HISTORY'

    #-----
    tResult=$(fComUsage -i -s long -f $tUsageScript -f $tInternalScript 2>&1)
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO tcu-internal.1" "$tResult" 'Template Use'
    assertContains "$LINENO tcu-internal.2" "$tResult" 'fComSetGlobals'

    #-----
    tResult=$(fComUsage -i -s html -t "Internal Doc" -f $tUsageScript -f $tInternalScript 2>&1)
    fTestDebug "tResult=$tResult"
    assertContains "$LINENO tcu-int-html.1" "$tResult" '<a href="#Template-Use">Template Use</a>'
    assertContains "$LINENO tcu-int-html.2" "$tResult" '<h3 id="fComSetGlobals">fComSetGlobals</h3>'
    assertContains "$LINENO tcu-int-html.3" "$tResult" '<title>Internal Doc</title>'
    assertContains "$LINENO tcu-int-html.4" "$tResult" '<h3 id="testComUsage">testComUsage</h3>'

    #-----
    tResult=$(fComUsage -i -s md -f $tUsageScript -f $tInternalScript 2>&1)
    assertContains "$LINENO tcu-int-md.1" "$tResult" '## Template Use'
    assertContains "$LINENO tcu-int-md.2" "$tResult" '### fComSetGlobals'
    assertContains "$LINENO tcu-int-md.3" "$tResult" '### testComUsage'

    #-----
    tResult=$(fComUsage -s long -f $tUsageScript -f $tInternalScript 2>&1)
    assertContains "$LINENO $tResult" "$tResult" "DESCRIPTION"

    #-----
    tResult=$(fComUsage -i -s long -f $tInternalScript 2>&1)
    assertContains "$LINENO $tResult" "$tResult" "Internal Documentation"

    #-----
    gpUnitDebug=0
    return

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 testComUsage

Test fComUsage. Verify the different output styles work.

=internal-cut
EOF
} # testComUsage

# --------------------------------
testComFunctions()
{
    local tResult

    tResult=$(fComCheckDeps 2>&1)
    assertTrue "$LINENO tcf-fComCheckDep tResult=$tResult" "[ $? -eq 0 ]"

    tResult=$(fComSetGlobals 2>&1)
    assertTrue "$LINENO tcf-fComSetGlobals tResult=$tResult" "[ $? -eq 0 ]"
    return

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 testComFunctions

Just verify these functions exist and run.

Calls:

 fComCheckDeps
 fComSetGlobals

=internal-cut
EOF
} # testComFunctions

# --------------------------------
testComSetConfigGlobal()
{
    local tResult
    local tGlobal=$HOME/.gitconfig

    # Note: a more complete "git config" is done when a test env is setup.

    grep -q '\[gitproj "testit"\]' $tGlobal >/dev/null 2>&1
    assertFalse "$LINENO did tearDown run?" "$?"

    fComSetConfig -g -k gitproj.testit.test-str -v "test a string"
    assertTrue "$LINENO -g" "[ -r $tGlobal ]"
    grep -q '\[gitproj "testit"\]' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"
    grep -q 'test-str = test a string' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"

    fComSetConfig -g -i -k gitproj.testit.test-int -v "2K"
    grep -q 'test-int = 2048' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"

    fComSetConfig -g -b -k gitproj.testit.test-bool -v "true"
    grep -q 'test-bool = true' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"
} # testComSetConfigGlobal

# --------------------------------

testComGetConfigGlobal()
{
    local tResult
    local tGlobal=$HOME/.gitconfig

    fComSetConfig -g -k gitproj.testit.test-str-get -v "test a string"
    fComSetConfig -g -k gitproj.testit.test-int-get -v "2K"
    fComSetConfig -g -k gitproj.testit.test-bool-get -v "true"
    fComSetConfig -g -k gitproj.testit.test-tbd-get -v "TBD"
    fComSetConfig -g -k gitproj.testit.test-not-get -v "not-defined"

    tResult=$(fComGetConfig -g -k gitproj.testit.test-str-get)
    assertEquals "$LINENO -g" "test a string" "$tResult"

    tResult=$(fComGetConfig -g -i -k gitproj.testit.test-int-get)
    assertEquals "$LINENO -g" "2048" "$tResult"

    tResult=$(fComGetConfig -g -b -k gitproj.testit.test-bool-get)
    assertEquals "$LINENO -g" "true" "$tResult"

    fComGetConfig -g -k gitproj.testit.foobar >/dev/null 2>&1
    assertTrue "$LINENO" "$?"
    assertEquals "$LINENO" "not-found" "$gGetConfigOrigin"

    tResult=$(fComGetConfig -g -k gitproj.testit.foobar -d TBD -e 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "gitproj.testit.foobar is not defined"

    tResult=$(fComGetConfig -g -k gitproj.testit.foobar -e 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "gitproj.testit.foobar is not defined"

    tResult=$(fComGetConfig -g -k gitproj.testit.test-tbd-get -d TBD -e 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "should not be set to"

    tResult=$(fComGetConfig -g -k gitproj.testit.test-tbd-get -e 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "gitproj.testit.test-tbd-get should not be set to: TBD"

    tResult=$(fComGetConfig -g -k gitproj.testit.test-not-get -d not-defined -e 2>&1)
    assertFalse "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "should not be set to"

    return 0
} # testComGetConfigGlobal

# --------------------------------
testComUnsetConfigGlobal()
{
    local tResult
    local tGlobal=$HOME/.gitconfig

    fComSetConfig -g -k gitproj.testit.test-str-unset -v "test unset"
    tResult=$(fComGetConfig -g -k gitproj.testit.test-str-unset)
    assertEquals "$LINENO $tResult" "test unset" "$tResult"
    grep -q "test unset" $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"

    fComUnsetConfig -g -k gitproj.testit.test-str-unset
    assertTrue "$LINENO" "[ $? -eq 0 ]"
    grep -q "test unset" $tGlobal
    assertFalse "$LINENO" "[ $? -eq 0 ]"
} # testComUnsetConfigGlobal

# --------------------------------
testCheckPkg()
{
    local tResult

    tResult=$(fComCheckPkg less 2>&1)
    assertTrue "$LINENO $tResult" "$?"

    tResult=$(fComCheckPkg foobar 2>&1)
    assertFalse "$LINENO $tResult" "$?"
} # testCheckPkg

# --------------------------------
testComStackTrace()
{
    local tResult

    gpSysLog=true
    tResult=$(fComStackTrace 2>&1)
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Stack trace at: gitproj-com.inc"
    assertContains "$LINENO $tResult" "$tResult" "testComStackTrace"
    assertContains "$LINENO $tResult" "$tResult" "fTestRun"

    tResult=$(tail -n 5 /var/log/user.log)
    assertTrue "$LINENO $tResult" "$?"
    assertContains "$LINENO $tResult" "$tResult" "Stack trace at: gitproj-com.inc"
    assertContains "$LINENO $tResult" "$tResult" "testComStackTrace"
    assertContains "$LINENO $tResult" "$tResult" "fTestRun"
    assertContains "$LINENO $tResult" "$tResult" "test-com.sh:"

} # testComStackTrace

# --------------------------------
testComFmt()
{
    local tText
    local tResult
    local tExpect

    tText=$(for i in $(seq 1 300); do echo -n "x "; done)

    COLUMNS=50
    tExpect=45
    tResult=$(echo "$tText" | fComFmt | head -n 1)
    assertTrue "$LINENO $tExpect got ${#tResult}" "[ ${#tResult} -le $tExpect ]"

    COLUMNS=20
    tExpect=30
    tResult=$(echo "$tText" | fComFmt | head -n 1)
    assertTrue "$LINENO $tExpect got ${#tResult}" "[ ${#tResult} -le $tExpect ]"

    COLUMNS=35
    tExpect=30
    tResult=$(echo "$tText" | fComFmt | head -n 1)
    assertTrue "$LINENO $tExpect got ${#tResult}" "[ ${#tResult} -le $tExpect ]"

    COLUMNS=120
    tExpect=115
    tResult=$(echo "$tText" | fComFmt | head -n 1)
    assertTrue "$LINENO $tExpect got ${#tResult}" "[ ${#tResult} -le $tExpect ]"

    COLUMNS=145
    tExpect=130
    tResult=$(echo "$tText" | fComFmt | head -n 1)
    assertTrue "$LINENO $tExpect got ${#tResult}" "[ ${#tResult} -le $tExpect ]"

    COLUMNS=""
    tExpect=75
    tResult=$(echo "$tText" | fComFmt | head -n 1)
    assertTrue "$LINENO $tExpect got ${#tResult}" "[ ${#tResult} -le $tExpect ]"
} # testComFmt

# --------------------------------
testComSetConfigMore()
{
    startSkipping
    fail "TBD"
    # untar a git env., test in and out of git dir
} # testComSetConfigMore

# --------------------------------
testComGetConfigMore()
{
    startSkipping
    fail "TBD"
    # untar a git env., test in and out of git dir
} # testComGetConfigMore

# --------------------------------
testComUnsetConfigMore()
{
    startSkipping
    fail "TBD"
    # untar a git env., test in and out of git dir
} #testComUnsetConfigMore

# --------------------------------
testComSelect()
{
    local tResult
    local tDirList
    local tPrompt
    local tHelp

    gpAuto=0

    tDirList=$(find $cDatMount3 $cDatMount3/* $cDatMount3/*/* -prune -type d 2>/dev/null | grep -v ' ' | sort -uf)
    tPrompt="Select a mount point: "
    tHelp="Just select by number."
    tResult=$(fComSelect "$pPrompt" "$tDirList" "$tHelp" 2>&1 < <(echo -e "1\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "QUIT"
    assertContains "$LINENO $tResult" "$tResult" "warning"
    assertContains "$LINENO $tResult" "$tResult" "Quitting"

    tResult=$(fComSelect "$pPrompt" "$tDirList" "$tHelp" 2>&1 < <(echo -e "2\n1\n"))
    assertFalse "$LINENO" "$?"
    assertContains "$LINENO $tResult" "$tResult" "QUIT"
    assertContains "$LINENO $tResult" "$tResult" "Just select by number"
    ##assertContains "$LINENO $tResult" "$tResult" "Uncoment to show tResult"

    fComSelect "$pPrompt" "$tDirList" "$tHelp" 2>&1 < <(echo -e "5\n") >/dev/null 2>&1
    assertTrue "$LINENO" "$?"
    assertContains "$LINENO $gResponse" "$gResponse" "/mnt/usb-video/video-2019-11-26/dev"

    return 0
} # testComSelect

# --------------------------------
testComYesNo()
{
    local tResult

    gpYesNo="Yes"
    fComYesNo "Continue" 2>&1
    assertTrue $LINENO $?
    assertEquals $LINENO "Yes" "$gResponse"

    gpYesNo="No"
    fComYesNo "Continue" 2>&1
    assertFalse $LINENO $?
    assertEquals $LINENO "No" "$gResponse"

    gpYesNo=""

    fComYesNo "Continue" >/dev/null 2>&1 < <(echo yes)
    assertTrue $LINENO $?
    assertEquals $LINENO "Yes" $gResponse

    tResult=$(fComYesNo "Continue" 2>&1 < <(echo Yes))
    assertTrue $LINENO $?
    assertContains "$LINENO $tResult" "$tResult" "Continue [y/n]?"

    fComYesNo "Continue" >/dev/null 2>&1 < <(echo no)
    assertFalse $LINENO $?
    assertEquals $LINENO "No" $gResponse

    fComYesNo "Continue" >/dev/null 2>&1 < <(echo xx)
    assertFalse $LINENO $?
    assertEquals $LINENO "No" $gResponse

    return 0
} # testComYesNo

# --------------------------------
NAtestComGit()
{
    local tResult

    gpVerbose=2
    tResult=$(fComGit ????)

} # testComGit

# ========================================
# This should be the last defined function
fTestRun()
{
    if [ ! -x $gpTest/shunit2.1 ]; then
        echo "Error: Missing: $gpTest/shunit2.1"
        exit 1
    fi
    shift $#
    if [ -z "$gpTestList" ]; then
        # shellcheck disable=SC1091
        . $gpTest/shunit2.1
        exit $?
    fi

    # shellcheck disable=SC1091
    . $gpTest/shunit2.1 -- $gpTestList
    exit $?

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 fTestRun

Run unit tests for the common functions.

=internal-cut
EOF
} # fTestRun

# ====================
# Main

export gpTest cTestCurDir gpTestList gpCmdName

gpCmdName=${BASH_SOURCE##*/}

# -------------------
# Set current directory location in PWD and cTestCurDir
if [ -z "$PWD" ]; then
    PWD=$(pwd)
fi
cTestCurDir=$PWD

# -------------------
# Define the location of this script
gpTest=${0%/*}
if [ "$gpTest" = "." ]; then
    gpTest=$PWD
fi
cd $gpTest
gpTest=$PWD
cd $cTestCurDir

# -----
# Optional input: a comma separated list of test function names
gpTestList="$*"

# -----
. $gpTest/test.inc

fTestRun $gpTestList
