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

$Revision: 1.2 $ $Date: 2021/09/08 01:39:35 $ GMT 

=cut

EOF
}

# ========================================

# --------------------------------
oneTimeSetUp()
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

 $cBin/gitproj-com.inc
 fComSetGlobals

=internal-cut
EOF
} # oneTimeSetUp

oneTimeTearDown()
{
    if [ -n "$cHome" ]; then
        HOME=$cHome
    fi
} # oneTimeTearDown

# --------------------------------
setUp()
{
    # Restore default global values, before each test
    unset cBin cCurDir cName cPID cVer gErr gpDebug gpFacility gpLog gpVerbose
    fComSetupTestEnv
    fCreateTestEnv
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
    git config --global --remove-section gitproj.test &>/dev/null
    if [ $gpDebug -ne 0 ]; then
        fRmTestEnv
    fi
    gpUnitDebug=0
    return 0
} # tearDown

# ========================================

# --------------------------------
testSetup()
{
    assertTrue "$LINENO" "[ -x $cBin/gitproj-com.inc ]"
    assertTrue "$LINENO" "[ -r $cTestFiles ]"
    assertTrue "$LINENO" "[ -d $cTestSrcDir ]"
    assertTrue "$LINENO" "[ -d $cTestDestDir ]"
    assertNotEquals "$LINENO" "$cHome" "$HOME"
    assertTrue "$LINENO" "[ -r $HOME/.gitproj.config ]"
    assertTrue "$LINENO" "[ -r $HOME/.gitproj-test.config ]"
    assertTrue "$LINENO" "[ -x $cBin/git-proj ]"

    for i in $cDatMount1 $cDatMount2 $cDatMount3; do
        fComUDebug "i=$i"
        assertTrue "$LINENO ${i##*/}" "[ -d $i ]"
    done
    for i in $cDatProj1 $cDatProj2; do
        fComUDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -d $HOME/$i ]"
    done
    for i in $cDatProj1Big; do
        fComUDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -r $HOME/$cDatProj1/$i ]"
    done
    for i in $cDatProj2Big $cDatProj2Big; do
        fComUDebug "i=$i"
        assertTrue "$LINENO ${i}" "[ -r $HOME/$cDatProj2/$i ]"
    done
}

# --------------------------------
testComInitialConfig()
{
    local tProg
    local tResult

    assertTrue "$LINENO -d $cCurDir" "[ -d $cCurDir ]"
    assertNotNull "$LINENO $cBin" "$cBin"
    assertTrue "$LINENO -d $cBin" "[ -d $cBin ]"
    assertTrue "$LINENO -f $cBin/$cName" "[ -f $cBin/$cName ]"
    assertTrue "$LINENO -x $cBin/$cName" "[ -x $cBin/$cName ]"
    assertTrue "$LINENO -x $cBin/gitproj-com.inc" "[ -x $cBin/gitproj-com.inc ]"

    assertEquals "$LINENO" "0" "$gpDebug"
    assertEquals "$LINENO" "2" "$gpVerbose"
    assertEquals "$LINENO" "0" "$gpLog"
    assertEquals "$LINENO" "user" "$gpFacility"
    assertEquals "$LINENO" "0" "$gErr"
    assertNull "$LINENO" "$(echo $cVer | tr -d '.[:digit:]')"

    for tProg in logger pod2text pod2usage pod2html pod2man pod2markdown tidy awk tr; do
        which $tProg &>/dev/null
        assertTrue "$LINENO missing: $tProg" "[ $? -eq 0 ]"
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
testComLog_MultiplePermutations()
{
    local tMsg
    local tLevel
    local tLine
    local tErr
    local tResult
    local tTestMsg

    # Check the format, for a number of settings
    gpLog=0
    gpVerbose=0
    gpDebug=0
    tMsg="Testing 123"
    tLine="458"
    tErr="42"

    for gpLog in 0 1; do
        for gpVerbose in 0 1; do
            for gpDebug in 0 1 2; do
                for tLevel in alert crit err warning notice info debug debug-1 debug-3; do
                    echo -n '.' 1>&2
                    tTestMsg="l-$gpLog.v-$gpVerbose.d-$gpDebug.$tLevel.fLog"
                    fComUDebug " "
                    fComUDebug "Call: fLog -p $tLevel -m \"$tMsg\" -l $tLine -e $tErr"
                    tResult=$(fLog -p $tLevel -m "$tMsg" -l $tLine -e $tErr 2>&1)
                    fComUDebug "tResult=$tResult"

                    if [ $gpVerbose -eq 0 ] && echo $tLevel | grep -Eq 'notice|info'; then
                        assertNull "$LINENO tcl1-$tTestMsg not notice,info" "$tResult"
                        continue
                    fi
                    if [ $gpVerbose -eq 1 ] && echo $tLevel | grep -Eq 'info'; then
                        assertNull "$LINENO tcl1-$tTestMsg not info" "$tResult"
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
                    assertContains "$LINENO tcl5-$tTestMsg.name" "$tResult" "$cName"
                    assertContains "$LINENO tcl6-$tTestMsg.level" "$tResult" "$tLevel:"
                    assertContains "$LINENO tcl7-$tTestMsg.msg" "$tResult" "$tMsg"
                    assertContains "$LINENO tcl8-$tTestMsg.line" "$tResult" '['$tLine']'
                    assertContains "$LINENO tcl9-$tTestMsg.$tLevel.err" "$tResult" '('$tErr')'
                done # tLevel
            done     # gpDebug
        done         # gpVerbose
    done             # gpLog

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
    export tSyslog=/var/log/user.log
    #export tSyslog=/var/log/messages.log
    #export tSyslog=/var/log/syslog

    # Check syslog
    gpLog=1
    gpVerbose=0
    tMsg="Testing 123"
    #for tLevel in emerg alert crit err warning; do
    for tLevel in alert crit err warning; do
        echo -n '.' 1>&2
        tTestMsg="$tLevel.fLog"
        fComUDebug " "
        fComUDebug "Call: fLog -p $tLevel -m \"$tMsg\""
        tResult=$(fLog -p $tLevel -m "$tMsg" 2>&1)
        fComUDebug "tResult=$tResult"
        assertContains "$LINENO tcl11-$tTestMsg" "$tResult" "$tLevel:"
        tResult=$(tail -n1 $tSyslog)
        fComUDebug "syslog tResult=$tResult"
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
    local tMsg
    local tLevel
    local tLine
    local tErr
    local tResult
    local tTestMsg

    gpUnitDebug=0
    gpLog=0
    gpVerbose=0
    local tMsg="Testing 123"
    local tLine="458"
    for gpLog in 0 1; do
        echo -n '.' 1>&2
        tTestMsg="l-$gpLog.fError"
        fComUDebug " "
        fComUDebug "Call: fError -m \"$tMsg\" -l $tLine"
        tResult=$(fError -m "$tMsg" -l $tLine 2>&1)
        fComUDebug "tResult=$tResult"
        assertContains "$LINENO $tTestMsg.name" "$tResult" "$cName"
        assertContains "$LINENO $tTestMsg.crit" "$tResult" "crit:"
        assertContains "$LINENO $tTestMsg.msg" "$tResult" "$tMsg"
        assertContains "$LINENO $tTestMsg.line" "$tResult" '['$tLine']'
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
    local tUsageScript=$cTest/test-com.sh
    local tInternalScript=$cBin/gitproj-com.inc

    gpUnitDebug=0

    #-----
    tResult=$(fComUsage -s usage -f $tUsageScript 2>&1)
    fComUDebug "tResult=$tResult"
    assertContains "$LINENO tcu-short" "$tResult" "Usage"

    #-----
    tResult=$(fComUsage -s foo -f $tUsageScript 2>&1)
    fComUDebug "tResult=$tResult"
    assertContains "$LINENO tcu-s-foo.1" "$tResult" "DESCRIPTION"
    assertContains "$LINENO tcu-s-foo.2" "$tResult" "HISTORY"

    #-----
    tResult=$(fComUsage -f $tUsageScript -s 2>&1)
    fComUDebug "tResult=$tResult"
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
    tResult=$(fComUsage -s html -f $tUsageScript -t "$cName Usage" 2>&1)
    assertContains "$LINENO tcu-html.1" "$tResult" '<li><a href="#DESCRIPTION">DESCRIPTION</a></li>'
    assertContains "$LINENO tcu-html.2" "$tResult" '<h1 id="HISTORY">HISTORY</h1>'
    assertContains "$LINENO tcu-html.3" "$tResult" "<title>$cName Usage</title>"

    #-----
    tResult=$(fComUsage -s md -f $tUsageScript 2>&1)
    assertContains "$LINENO tcu-md.1" "$tResult" '# DESCRIPTION'
    assertContains "$LINENO tcu-md.2" "$tResult" '# HISTORY'

    #-----
    tResult=$(fComUsage -i -s long -f $tUsageScript -f $tInternalScript 2>&1)
    fComUDebug "tResult=$tResult"
    assertContains "$LINENO tcu-internal.1" "$tResult" 'Template Use'
    assertContains "$LINENO tcu-internal.2" "$tResult" 'fComSetGlobals'

    #-----
    tResult=$(fComUsage -i -s html -t "Internal Doc" -f $tUsageScript -f $tInternalScript 2>&1)
    fComUDebug "tResult=$tResult"
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
    tResult=$(fComUsage -a -s long -f $tUsageScript -f $tInternalScript 2>&1)
    assertContains "$LINENO long" "$tResult" "DESCRIPTION"

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

    # Note: more complete "git config" is done when a test env is setup.

    grep -q '\[gitproj "testit"\]' $tGlobal
    assertFalse "$LINENO tearDown ran" "[ $? -eq 0 ]"

    fComSetConfig -g -k gitproj.testit.test-str -v "test a string"
    assertTrue "$LINENO -g" "[ -r $tGlobal ]"
    grep -q '\[gitproj "testit"\]' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"
    grep -q 'test-str = test a string' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"

    fComSetConfig -g -i -k gitproj.testit.test-int -v "2K"
    grep -q 'test-int = 2048' $tGlobal
    assertTrue "$LINENO" "[ $? -eq 0 ]"

    fComSetConfig -g -b -k gitproj.testit.test-bool -v "yes"
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
    fComSetConfig -g -k gitproj.testit.test-bool-get -v "yes"

    tResult=$(fComGetConfig -g -k gitproj.testit.test-str-get)
    assertEquals "$LINENO -g" "test a string" "$tResult"

    tResult=$(fComGetConfig -g -i -k gitproj.testit.test-int-get)
    assertEquals "$LINENO -g" "2048" "$tResult"

    tResult=$(fComGetConfig -g -b -k gitproj.testit.test-bool-get)
    assertEquals "$LINENO -g" "true" "$tResult"
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

# ====================
# This should be the last defined function
fComRunTests()
{
    if [ ! -x $cTest/shunit2.1 ]; then
        echo "Error: Missing: $cTest/shunit2.1"
        exit 1
    fi
    shift $#
    if [ -z "$gpTest" ]; then
        # shellcheck disable=SC1091
        . $cTest/shunit2.1
        exit $?
    fi

    # shellcheck disable=SC1091
    . $cTest/shunit2.1 -- $gpTest
    exit $?

    cat <<EOF >/dev/null
=internal-pod

=internal-head3 fComRunTests

Run unit tests for the common functions.

=internal-cut
EOF
} # fComRunTests

# ====================
# Main

export cTest cTestCurDir gpTest

# -------------------
# Set current directory location in PWD and cTestCurDir
if [ -z "$PWD" ]; then
    PWD=$(pwd)
fi
cTestCurDir=$PWD

# -------------------
# Define the location of this script
cTest=${0%/*}
if [ "$cTesBin" = "." ]; then
    cTest=$PWD
fi
cd $cTest
cTest=$PWD
cd $cTestCurDir

# -----
# Optional input: a comma separated list of test function names
gpTest="$*"

# -----
. $cTest/test.inc

fComRunTests $gpTest