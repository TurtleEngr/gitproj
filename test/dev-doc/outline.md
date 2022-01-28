# Outline of Files

# gitproj Source Repo - dev

    gitproj/
    |   |Makefile
    |   |README.md
    |   |TODO.md
    |   |.gitignore
    |   |doc/
    |   |   |LICENSE
    |   |   |README.md - generated from ../../README.md, versioned
    |   |   |VERSION
    |   |   |config/
    |   |   |   |gitconfig.default
    |   |   |   |gitignore.default
    |   |   |   |gitproj.config.global
    |   |   |   |getproj.config.local
    |   |   |hooks/
    |   |   |   |pre-commit*
    |   |   |user-doc/
    |   |   |   |git-proj.html - generated, versioned
    |   |   |   |git-proj.md - generated, versioned
    |   |   |   |config.html - generated, versioned
    |   |   |   |config.md - generated, versioned
    |   |   |   |subcommands/
    |   |   |   |   |git-proj-CMD.html - gen from git-proj-CMD
    |   |   |   |   |git-proj-CMD.md - gen from git-proj-CM, versionedD
    |   |   |   |tutorials/
    |   |   |   |   |create_a_git-proj_repo.html - generated, not versioned
    |   |   |   |   |create_a_git-proj_repo.md -  generated, versioned
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
    |   |   |   |git-proj.html - gen: 'git-proj -H int-html', versioned
    |   |   |   |git-proj.md - gen: 'git-proj -H int-md', versioned
    |   |   |   |outline.md - this document
    |   |   |Makefile
    |   |   |shunit2 -&gt; shunit2.1*
    |   |   |shunit2.1*
    |   |   |test.inc
    |   |   |test-com.sh*
    |   |   |test-[SUBCMD].sh*
    |   |   |test-env.tgz -&gt; ../../test-env.tgz
    |   |   |test-env_FILE.tgz -&gt; ../../test-env_FILE.tgz
    |   |   |test-com.log - generated
    |   |   |test-[SUBCMD].log - generated
    |   |package/
    |   |   |Makefile
    |   |   |dist/ - all in dist is generated or copied
    |   |   |   |usr/
    |   |   |   |   |lib/
    |   |   |   |   |   |git-core/ - copy
    |   |   |   |   |   |   |git-proj
    |   |   |   |   |   |   |git-proj-CMD
    |   |   |   |   |   |   |gitproj-CMD.inc
    |   |   |   |   |share/
    |   |   |   |   |   |doc/
    |   |   |   |   |   |   |git-proj/ - copy doc/
    |   |   |   |   |   |   |   |config/ - system configs
    |   |   |   |   |   |   |   |contrib/
    |   |   |   |   |   |   |   |hooks/
    |   |   |   |   |   |   |   |user-doc/
    |   |   |   |   |   |   |   |   |index.html
    |   |   |   |   |   |   |   |   |README.html
    |   |   |   |   |   |   |   |   |subcommands/
    |   |   |   |   |   |   |   |   |tutorials/
    |   |   |   |   |   |man
    |   |   |   |   |   |   |man1
    |   |   |   |   |   |   |    |git-proj.1.gz - generated
    |   |   |   |   |   |   |    |gitproj.1.gz - generated
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

# gitproj Installed Files - prod

Note: The tests will only check for a valid installation. Unit test will
only be supported in a dev env. Running unit test on the installed files
would be tricky, and of low value.

See gitproj/package/dist/

# After running git-proj-init local, or git-proj-clone

    cd ~/ANY-DIR/PROJECT
    git proj init

    or

    cd ~/ANY-DIR
    git proj clone -d /MOUNT-DIR/ANY-DIR/PROJ.git

    home/
    |   |USER/
    |   |   |.gitconfig - see gitproj sections
    |   |   |.gitignore
    |   |   |ANY-DIR/
    |   |   |   |PROJECT/
    |   |   |   |   |.git/
    |   |   |   |   |   |config/ - see gitproj sections
    |   |   |   |   |   |hooks/
    |   |   |   |   |   |   |pre-commmit
    |   |   |   |   |.gitproj - version
    |   |   |   |   |.gitignore - version
    |   |   |   |   |.pre-commit - version
    |   |   |   |   |raw/ - never versioned in git
    |   |   |   |   |   |ANY-DIR/
    |   |   |   |   |   |  |LARGE-FILE.MP4
    |   |   |   |   |ANY-DIR/ - version
    |   |   |   |   |   |ANY-FILE - version
    |   |   |   |   |   |LARGE-FILE.MP4 -&gt; ../raw/ANY-DIR/LARGE-FILE.MP4
                         - version the symlink

# After running git-proj-remote

    cd ~/ANY-DIR/PROJECT
    git proj remote -a -d /MOUNT-DIR/ANY-DIR

    /MOUNT-DIR/
    |   |   |ANY-DIR/
    |   |   |   |PROJECT.raw/ - remote-raw-origin
    |   |   |   |PROJECT.git/ - remote-origin

# dev-test Environment

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

# Function Call Map

## git-proj

    fUsage()
        fComUsage
        fComSetGlobals

## git-proj-add

    fUsage()

## git-proj-check

## git-proj-clone

    fUsage()
            fComUsage
            fCloneFromRemoteDir

## git-proj-config

    fUsage()

    fCheckForGit()

    fFindRemote()

## git-proj-init

    fUsage()
            fComUsage
            fInitCreateLocalGit

## git-proj-pull

    fUsage()
        fComUsage
        fComGetProjGlobals
        fPullFromOrigin

## git-proj-push

    fUsage()
        fComUsage
        rsync
        fComGetProjGlobals
        fPushToOrigin

## git-proj-remote

    fUsage()
        fComUsage
        fRemoteSetGlobals
        fRemoteCreateRemoteGit

## git-proj-status

    fUsage()
        fComGetVer
        fStatusOutput

## gitproj-clone.inc

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

## gitproj-com.inc

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

## gitproj-init.inc

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

## gitproj-pull.inc

    fPullRawFiles()
        fComSelect

    fPullGit()
        fComGit

    fPullFromOrigin()
        fComGetProjGlobals
        fComIsRemoteMounted
        fPullRawFiles
        fPullGit

## gitproj-push.inc

    fPushRawFiles()
        fComSelect

    fPushGit()
        fComGit

    fPushToOrigin()
        fComGetProjGlobals
        fComIsRemoteMounted
        fPushRawFiles
        fPushGit

## gitproj-remote.inc

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

## gitproj-status.inc

    fStatusGit()

    fStatusRaw()

    fStatusOutput()
        fStatusGit
        fStatusRaw
        fComGetProjGlobals

## Get List

    #!/bin/bash

    cd git-core
    for i in git-proj* gitproj-*.inc; do
        echo '&lt;pre&gt;'
        echo
        echo "### $i"
        echo
        echo '&lt;pre&gt;'
        grep -E '\(\)| \$tTidy | awk | tr | rsync |fCom|fInit|fRemote|fPush|fPull|fStatus|fClone|fmt|pod2|git config ' $i | grep -Ev '\}|if|#|^=| local ' | awk '/\(\)/ {print "\n" $1; next} {print "\t" $1}'
    done &gt;t.txt
