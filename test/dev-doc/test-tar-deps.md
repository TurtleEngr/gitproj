# Finding test tar file dependencies

Steps for inding tar file dependnecies and who creates the tar files:

    echo 'Deps' >t.tmp
    grep -E '\(\)|TarIn.*=' test-*.sh >>t.tmp
    echo '------------------' >>t.tmp
    echo 'Creates' >>t.tmp
    grep -E '\(\)|TarOut.*=' test-*.sh >>t.tmp

Edit t.tmp

Remove the tests that have no tar stms after them.

--------------------

## Dependencies

### test-clone.sh:

    test-clone.sh:    local tTarIn=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz
    test-clone.sh:    local tTarIn=$gpTest/test-env_Home3AfterCloneMkGit.tgz

### test-com2.sh:

    test-com2.sh:    local tTarIn=$gpTest/test-env_ProjLocalDefined.tgz

###test-init.sh:

    test-init.sh:    local tTarIn=$gpTest/test-env_HomeAfterBMove.tgz
    test-init.sh:    local tTarIn=$gpTest/test-env_ProjAfterGInit.tgz

### test-pre-commit.sh:

    test-pre-commit.sh:    local tTarIn=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz
    test-pre-commit.sh:    local tTarIn2=$gpTest/test-env_Home3AfterCloneSummary.tgz

### test-pull.sh:

    test-pull.sh:    local tTarIn=$gpTest/test-env_ProjLocalDefined.tgz
    test-pull.sh:    local tTarIn=$gpTest/test-env_Home2AfterPush.tgz

### test-push.sh:

    test-push.sh:    local tTarIn=$gpTest/test-env_ProjLocalDefined.tgz
    test-push.sh:    local tTarIn=$gpTest/test-env_TestDestDirAfterRemoteReport.tgz

### test-remote.sh:

    test-remote.sh:    local tTarIn=$gpTest/test-env_ProjLocalDefined.tgz
    test-remote.sh:    local tTarIn=$gpTest/test-env_TestDestDirAfterMkRemote.tgz

### test-status.sh:

    test-status.sh:    local tTarIn=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz
    test-status.sh:    local tTarIn2=$gpTest/test-env_Home3AfterCloneSummary.tgz

------------------

## Creates

### test-clone.sh:

    test-clone.sh:testCloneMkGitDirPass()
    test-clone.sh:    local tTarOut=$gpTest/test-env_Home3AfterCloneMkGit.tgz
    test-clone.sh:testCloneSummary()
    test-clone.sh:    local tTarOut=$gpTest/test-env_Home3AfterCloneSummary.tgz

### test-init.sh:

    test-init.sh:testInitMoveBinaryFiles()
    test-init.sh:    local tTarOut=$gpTest/test-env_HomeAfterBMove.tgz
    test-init.sh:testInitMkGitDir()
    test-init.sh:    local tTarOut=$gpTest/test-env_ProjAfterGInit.tgz
    test-init.sh:testInitCreateLocalGitAuto()
    test-init.sh:    local tTarOut=$gpTest/test-env_ProjLocalDefined.tgz

### test-push.sh:

    test-push.sh:testGitProjPushCLI()
    test-push.sh:    local tTarOut=$gpTest/test-env_Home2AfterPush.tgz

### test-remote.sh:

    test-remote.sh:testRemoteMkRemote()
    test-remote.sh:    local tTarOut=$gpTest/test-env_TestDestDirAfterMkRemote.tgz
    test-remote.sh:testRemoteReport()
    test-remote.sh:    local tTarOut=$gpTest/test-env_TestDestDirAfterRemoteReport.tgz
    test-remote.sh:testRemoteCreateRemoteGit()
    test-remote.sh:    local tTarOut=$gpTest/test-env_TestDestDirAfterCreateRemoteGit.tgz
