# Outline of File Organization

## gitproj Source Repo - dev

    gitproj/
    |   |Makefile
    |   |README.md
    |   |TODO.md
    |   |.gitignore
    |   |doc/
    |   |   |LICENSE
    |   |   |README
    |   |   |VERSION
    |   |   |config/
    |   |   |   |gitconfig.default
    |   |   |   |gitignore.default
    |   |   |   |gitproj.config.global
    |   |   |   |getproj.config.local
    |   |   |hooks/
    |   |   |   |pre-commit*
    |   |   |user-doc/
    |   |   |   |README.md - generated from ../../README.md
    |   |   |   |git-proj.html - generated
    |   |   |   |git-proj.md - generated
    |   |   |   |config.html - generated
    |   |   |   |config.md - generated
    |   |   |   |subcommands/
    |   |   |   |   |git-proj-CMD.html - gen from git-proj-CMD
    |   |   |   |   |git-proj-CMD.md - gen from git-proj-CMD
    |   |   |   |tutorials/
    |   |   |   |   |create_a_git-proj_repo.html - gen from src-doc/create_a_git-proj_repo.pod
    |   |src-doc/ - src for generated docs
    |   |   |doc-org.md
    |   |   |config.pod
    |   |   |create_a_git-proj_repo.pod
    |   |git-core/
    |   |   |git-proj*
    |   |   |gitproj-com.inc*
    |   |   |gitproj-[SUBCMD].inc*
    |   |   |git-proj-[SUBCMD]*
    |   |test/
    |   |   |dev-doc/
    |   |   |   |enhancements/
    |   |   |   |   |raw-cvs.md
    |   |   |   |   |raw-pros-cons.md
    |   |   |   |   |raw-rclone.md
    |   |   |   |   |raw-rsnapshot.md
    |   |   |   |   |raw-ssh.md
    |   |   |   |   |version-wizard.md
    |   |   |   |git-proj.html - gen: 'git-proj -H int-html'
    |   |   |   |git-proj.md - gen: 'git-proj -H int-md'
    |   |   |   |outline.md - this document
    |   |   |Makefile
    |   |   |outline.md
    |   |   |shunit2 -> shunit2.1*
    |   |   |shunit2.1*
    |   |   |test-env.tgz -> ../../test-env.tgz
    |   |   |test-env_FILE.tgz -> ../../test-env_FILE.tgz
    |   |   |test.inc
    |   |   |test-com.sh*
    |   |   |test-[SUBCMD].sh*
    |   |package/
    |   |   |Makefile
    |   |   |dist/ - all in dist is generated or copied
    |   |   |   |usr/
    |   |   |   |   |lib/
    |   |   |   |   |   |git-core/
    |   |   |   |share/
    |   |   |   |doc/
    |   |   |   |   |   |git-proj/
    |   |   |   |   |   |   |config/
    |   |   |   |   |   |   |contrib/
    |   |   |   |   |   |   |hooks/
    |   |   |   |   |   |   |user-doc
    |   |   |   |   |   |   |   |subcommands/
    |   |   |   |   |   |   |   |   |tutorials/
    |   |   |   |   |   |man
    |   |   |   |   |   |   |man1
    |   |   |   |   |   |   |    |gitproj.1.gz
    |   |   |pkg/ - generated
    |   |   |Makefile
    |   |   |ver.sh*
    |   |   |mx.require
    |   |   |epm.patch*
    |   |   |epm.list - generated not versioned
    |   |   |ver.env* - generated not versioned
    |   |   |ver.epm - generated not versioned
    |   |   |ver.mak - generated not versioned
    |   |   |    |   |   |VERSION - versioned generated release info

## gitproj Installed Files - prod

Note: The tests will only check for a valid installation. Unit test
will only be supported in a dev env. Running unit test on the
installed files would be tricky, and of low value.

See gitproj/package/dist/

## After running git-proj-init local, or git-proj-clone

    cd ~/ANY-DIR/PROJECT
    git proj init

    or

    cd ~/ANY-DIR
    git proj clone -d /MOUNT-DIR/ANY-DIR/PROJ.git

    home/
    |   |USER/
    |   |   |.git.config - see "include.path" .gitproj.config.global
    |   |   |.gitproj.config.global
    |   |   |ANY-DIR/
    |   |   |   |PROJECT/
    |   |   |   |   |.git/
    |   |   |   |   |   |config -"include.path" ../.gitproj.config.$HOSTNAME
    |   |   |   |   |.gitproj.config.local
    |   |   |   |   |.gitproj.config.$HOSTNAME
    |   |   |   |   |raw/
    |   |   |   |   |   |ANY-DIR/
    |   |   |   |   |   |  |LARGE-FILE.MP4
    |   |   |   |   |ANY-DIR/
    |   |   |   |   |   |LARGE-FILE.MP4 -> ../raw/ANY-DIR/LARGE-FILE.MP4

## After running git-proj-remote

    cd ~/ANY-DIR/PROJECT
    git proj remote -a -d /MOUNT-DIR/ANY-DIR

    /MOUNT-DIR/
    |   |   |ANY-DIR/
    |   |   |   |PROJECT.raw/
    |   |   |   |PROJECT.git

## Git Config Definition Order

The last definition 'wins".

1. /etc/gitconfig - optional
2. /home/USER/.gitconfig
3. /home/USER/.gitproj.config.global (include.path at end of .gitconfig)
4. GIT_DIR/.git/config
5. GIT_DIR/.gitproj.config.$HOSTNAME (include.path at end of GIT_DIR/.git/config)
6. Env. var. will override corresponding .git config vars.
7. Command line options will override env. var. and corresponding .git config vars.

## Variable Naming Convention

* Globals that came from command line, config files, or external to
the scripts, begin with "gp" (global parameter). For example: gpVAR
(if they are not already defined before script (#7), set the initial
value to files #1 through #6. The command line option can always set
the value (#7)

* Global variables should begin with a "g"

* Global constants should begin with a "c"

* Local variables should begin with a "t" (temporary)

* git config vars are usually all lower case with words separated by
hyphens (-).

* Bash variables usually use CamelCase, with each word beginning with
an upper case letter (no hyphens or underscores).

## Function Naming Convention

* All gitproj functions begin with a "f"

* Functions in the include files, begin with command base. For example:
functions in gitproj-init.inc begin with "fInit".

## Coding patterns

* At the top of include files define exports for all the globals that
the include file reads/writes to.

* At the end of an include file, call a function that will define
defaults for the important globals used by the include file. [optional]

* For the user callable scripts, do minimal setup--mainly collect the
and validate the options.  Include files with common functions and
functions specific to the script. Put as much as possible into
functions in the include file, so that the functions can be directly
tested with unit test scripts found in doc/test.

* Minimal vars: gpBin, cCurDir, gpDoc, gpTest if a test script.
All other vars can be defined from include files or from git config vars.

* Define gpCmdName at the top of each each script that is called by a
user.

* Clean-Coding style (well I try).

    * Ifs

        * Avoid if/then/else. Replace with: if problem, then exit

        * Avoid long contents in ifs. Make functions.

        * Don't nest ifs more than 2 levels. One level is preferred.

        * Don't clutter the code with "pass-through" error-handling
          ifs. (see below)

        * Rather than using debug ifs, use fLog with its debug level
          support. Using TDD, the need for internal debug output should
          be reduced.

    * Identify problems or defaults early in a function so it can exit
      early.

    * Most of the code is structured so that if a function returns,
      you can assume it executed OK, or it took an acceptable default
      action. There should be no need to continually check exit codes
      from functions.

    * In other words, if there is a "fatal" error, cleanup, and exit
      with an error msg. Don't pass around exit codes, through
      multiple levels (one level is OK for the "utility"
      functions). Exit codes are mainly for the immediate calling
      function--if there was and error, exit, don't pass the error up
      to another level.

* Use TDD: Every function should have tests to exercise all the input
boundary conditions, and all of the error-handling states that are
unique to the function.

* shunit2 is used for the TDD framework. See test/Makefile

* TDD is done with the git development environment structure. It is
not done with "installed" code.

* The -p option is not used with the "read" command, because the
prompt is not "captured" with the test scripts. So use "echo -n" for
the prompts before the read command.

* fLog and fError messages

    * Put this at the beginning or end of each file

        export tSrc=${BASH_SOURCE##*/}

    * Put this in each function that calls fError or fLog

        local tSrc=${BASH_SOURCE##*/}

    * in Error and Log always pass this argument:

        -l $tSrc:$LINENO

## File include pattern - prod

    gpBin=/usr/lib/git-core - set when a CMD is run
    gpDoc=/usr/share/doc/gitproj

        $gpBin/git-proj-CMD
            . $gpBin/gitproj-com.inc
                fComSetGlobals
            . $gpBin/gitproj-CMD.inc
                fCMDSetGlobals

## File include pattern - dev

    gpBin=DIR/gitproj/git-core - set when a CMD is run
    gpDoc=$gpBin/../doc or DIR/gitproj/doc

        $gpBin/git-proj-CMD
            . $gpBin/gitproj-com.inc
                fComSetGlobals
            . $gpBin/gitproj-CMD.inc
                fCMDSetGlobals

## File include pattern - dev-test

    gpTest=DIR/gitproj/test - set when a test-*.inc is run
    gpBin=$gpTest/../git-core or DIR/gitproj/git-core
    gpDoc=$gpTest/../doc or DIR/gitproj/doc

        $gpTest/test-com.sh*
            . $gpTest/test.inc
                fTestSetupEnv
                . $gpBin/gitproj-com.inc
                    fComSetGlobals
            fComRunTests
                . $gpTest/shunit2.1*

        $gpTest/test-CMD.sh
            . $gpTest/test.inc
                fTestSetupEnv
                . $gpBin/gitproj-com.inc
                    fComSetGlobals
            fTestCreateEnv
            . $gpBin/gitproj-CMD.inc
                fCMDSetGlobals
            fTestConfigSetup

## dev-test Environment

    gpTest=DIR/gitproj/test
    cTestEnv=$gpTest/../..
    HOME=$cTestEnv/root/home/john

Create with:

    cd $cTestEnv
    tar -xzf $gpTest/test-env.tgz

Creates:

    $cTestEnv/
    |   |test/
    |   |   |test-files.txt - this outline
    |   |   |root/
            |   |mnt/
            |   |   |disk-2/            - $cDatMount1
            |   |   |usb-misc/          - $cDatMount2
            |   |   |   |files-2021-08-12/
            |   |   |usb-video/         - $cDatMount3
            |   |   |   |video-2020-04-02/
            |   |home/
            |   |   |john/              - $HOME
            |   |   |   |project/
            |   |   |   |   |beach/     - $cDatProj3
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |file1.txt
            |   |   |   |   |   |   |file2.txt
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |file.txt
            |   |   |   |   |   |.git/
            |   |   |   |   |george/    - $cDatProj1
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |notes.html
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |george.kdenlive
            |   |   |   |   |   |src/
            |   |   |   |   |   |   |final/
            |   |   |   |   |   |   |   |george.mp4 - $cDatProj1Big
            |   |   |   |   |   |   |raw/
            |   |   |   |   |   |   |   |MOV001.MP3 - $cDatProj1Big
            |   |   |   |   |   |   |   |MOV001.mp4 - $cDatProj1Big
            |   |   |   |   |   |.gitignore
            |   |   |   |   |   |README.html
            |   |   |   |   |paulb/             - $cDatProj2
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |notes.html
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |paulb.kdenlive
            |   |   |   |   |   |src/
            |   |   |   |   |   |   |final/
            |   |   |   |   |   |   |   |paulb.mp4  - $cDatProj2Big
            |   |   |   |   |   |   |raw/
            |   |   |   |   |   |   |   |MOV001.MP3 - $cDatProj2Big
            |   |   |   |   |   |   |   |MOV001.mp4 - $cDatProj2Big
            |   |   |   |   |   |README.html

## Function Call Map

### git-proj

```
fUsage()
	fComUsage
	fComSetGlobals
```

### git-proj-add

```
fUsage()
```

### git-proj-check

### git-proj-clone

```
fUsage()
	fComUsage
	fCloneFromRemoteDir
```

### git-proj-config

```
fUsage()

fCheckForGit()

fFindRemote()
```

### git-proj-init

```

fUsage()
	fComUsage
	fInitCreateLocalGit
```

### git-proj-pull

```
fUsage()
	fComUsage
	fComGetProjGlobals
	fPullFromOrigin
```

### git-proj-push

```
fUsage()
	fComUsage
	rsync
	fComGetProjGlobals
	fPushToOrigin
```

### git-proj-remote

```
fUsage()
	fComUsage
	fRemoteSetGlobals
	fRemoteCreateRemoteGit
```

### git-proj-status

```
fUsage()
	fComGetVer
	fStatusOutput
```

### gitproj-clone.inc

```
fCloneGettingStarted()

fCloneValidRemoteDir()

fCloneCheckLocalConfig()
	fComConfigCopy
	fComSetConfig

fCloneCheckHostConfig()
	tHost=$(fComGetConfig
	fComConfigCopy
	fComSetConfig
	fComSetConfig

fCloneCheckProjConfig()
	tHost=$(fComGetConfig
	fComConfigCopy
	fComSetConfig

fCloneMkGitDir()
	fCloneCheckProjConfig
	fCloneCheckHostConfig
	fCloneCheckLocalConfig

fCloneMkRawDir()

fCloneUpdateHostConfig()
	fComSetConfig

fCloneSummary()

fCloneFromRemoteDir()
	fCloneGettingStarted
	fCloneValidRemoteDir
	fCloneMkGitDir
	fCloneMkRawDir
	fCloneUpdateHostConfig
	fCloneSummary
	fComIntroText
	fComPreProjSetGlobals
```

### gitproj-com.inc

```

fComGitProjInternalDoc()

fComIntroText()

fComConfigCopy()
	fError
	fComConfigCopy

fComFirstTimeSet()
	fComGetConfig
	fComConfigCopy

fComSetGlobals()

fComPreProjSetGlobals()
	fComFirstTimeSet
	fComGetConfig

fComCheckDeps()

fComInternalDoc()
	awk

fComUsage()
	fComInternalDoc
	fError
	pod2html
	pod2man
	pod2markdown
	pod2text
	pod2usage
	tidy

fComStackTrace()

fComSelect()

fComYesNo()

fComFmtLog()

fLog()
	fComFmtLog

fError()
	fComStackTrace

fComGit()

fComSetConfig()
	fError
	fComSetConfig

fComGetConfig()
	fError
	fComGetConfig

fComUnsetConfig()
	fError

fComSaveVar2Config()

fComConfigSetupGlobal()
	fComConfigCopy

fComConfigSetupLocal()
	fComMustBeInGitRepo
	fComConfigCopy

fComCheckPkg()
	fLog

fComMustBeInGitRepo()

fComMustNotBeInGit()

fComMustBeInProjRepo()
	fComStackTrace

fComAllMustBeReadable()

fComIsRemoteMounted()
	fComGetConfig

fComGetProjGlobals()
	fComGetConfig
	fComGetVer

fComGetVer()
	fComGetConfig
	fComSetConfig

fComFmt()
	fmt
	fComSetGlobals
```

### gitproj-init.inc

```

fInitGettingStarted()
	fLog

fInitValidLocalPath()

fInitGetLocalPath()

fInitValidSize()

fInitGetSize()

fInitGetBinaryFiles()

fInitGetMoveFiles()

fInitGetGitFlow()
	fComCheckPkg

fInitSummary()

fInitMkRaw()

fInitMoveBinaryFiles()
	fInitGetBinaryFiles

fInitMkGitFlow()
	fComSetConfig

fInitMkGitDir()
	fInitMkGitFlow

fInitMkLocalConfig()
	fComConfigCopy

fInitSaveVarsToConfigs()
	fComSaveVar2Config

fInitCreateLocalGit()
	fInitGettingStarted
	fInitGetLocalPath
	fInitGetSize
	fInitGetMoveFiles
	fInitGetGitFlow
	fInitSummary
	fInitMkRaw
	fInitMoveBinaryFiles
	fInitMkGitDir
	fInitMkLocalConfig
	fInitSaveVarsToConfigs
	fComIntroText
	fComPreProjSetGlobals
```

### gitproj-pull.inc

```

fPullRawFiles()
	fComSelect

fPullGit()
	fComGit

fPullFromOrigin()
	fComGetProjGlobals
	fComIsRemoteMounted
	fPullRawFiles
	fPullGit
```

### gitproj-push.inc

```

fPushRawFiles()
	fComSelect

fPushGit()
	fComGit

fPushToOrigin()
	fComGetProjGlobals
	fComIsRemoteMounted
	fPushRawFiles
	fPushGit
```

### gitproj-remote.inc

```

fRemoteSetGlobals()
	fComGetConfig
	fComGetVer

fRemoteCheckDir()

fRemoteCheckDirSpace()

fRemoteGetDirList()

fRemoteGetAnotherMountDir()

fRemoteGetMountDir()
	fRemoteGetDirList
	fComSelect
	fRemoteGetAnotherMountDir
	fRemoteGetDirList

fRemoteGetRemoteRawDir()

fRemoteMkRemote()
	fComSetConfig

fRemoteReport()
	fComGetConfig
	fComSetConfig

fRemoteCommit()
	fComGit

fRemoteCreateRemoteGit()
	fRemoteGetMountDir
	fRemoteGetRemoteRawDir
	fRemoteMkRemote
	fRemoteReport
	fRemoteCommit
```

### gitproj-status.inc

```

fStatusGit()

fStatusRaw()

fStatusOutput()
	fStatusGit
	fStatusRaw
	fComGetProjGlobals

### Get List

```bash
cd git-core
for i in git-proj* gitproj-*.inc; do
    echo '```'
    echo
    echo "### $i"
    echo
    echo '```'
    grep -E '\(\)| \$tTidy | awk | tr | rsync |fCom|fInit|fRemote|fPush|fPull|fStatus|fClone|fmt|pod2|git config ' $i | grep -Ev '\}|if|#|^=| local ' | awk '/\(\)/ {print "\n" $1; next} {print "\t" $1}'
done >t.txt
```
