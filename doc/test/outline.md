# Outline of File Organization

## gitproj Source Repo - dev

    gitproj/
    |   |doc/
    |   |   |config/
    |   |   |   |gitproj.config.system
    |   |   |   |gitproj.config.global
    |   |   |   |getproj.config.local
    |   |   |   |gitproj-config.test-dev
    |   |   |   |gitproj-config.test-prod
    |   |   |hooks/
    |   |   |   |pre-commit*
    |   |   |html/ - generated user level docs
    |   |   |   |index.html
    |   |   |   |*.html
    |   |   |markdown/ - generated user level docs
    |   |   |   |*.md
    |   |   |development/ - generated docs for developers
    |   |   |   |index.html
    |   |   |   |*.html
    |   |   |   |*.md
    |   |   |test/
    |   |   |   |outline.md
    |   |   |   |shunit2 -> shunit2.1*
    |   |   |   |shunit2.1*
    |   |   |   |test.inc
    |   |   |   |test-com-inc*
    |   |   |   |test-env.tgz -> ../../../test-env.tgz
    |   |   |   |test-add.inc*
    |   |   |   |test-check.inc*
    |   |   |   |test-clone.inc*
    |   |   |   |test-config.inc*
    |   |   |   |test-init.inc*
    |   |   |   |test-move.inc*
    |   |   |   |test-pull.inc*
    |   |   |   |test-push.inc*
    |   |   |   |test-remote.inc*
    |   |   |   |test-rm.inc*
    |   |   |   |test-status.inc*
    |   |   |LICENSE
    |   |git-core/
    |   |   |gitproj-com.inc*
    |   |   |gitproj-init.inc*
    |   |   |gitproj-add.inc*
    |   |   |gitproj-check.inc*
    |   |   |gitproj-clone.inc*
    |   |   |gitproj-config.inc*
    |   |   |gitproj-move.inc*
    |   |   |gitproj-pull.inc*
    |   |   |gitproj-push.inc*
    |   |   |gitproj-remote.inc*
    |   |   |gitproj-rm.inc*
    |   |   |gitproj-status.inc*
    |   |   |git-proj*
    |   |   |git-proj-add*
    |   |   |git-proj-check*
    |   |   |git-proj-clone*
    |   |   |git-proj-config*
    |   |   |git-proj-init*
    |   |   |git-proj-move*
    |   |   |git-proj-pull*
    |   |   |git-proj-push*
    |   |   |git-proj-remote*
    |   |   |git-proj-rm*
    |   |   |git-proj-status*
    |   |.gitignore
    |   |Makefile
    |   |README.md
    |   |TODO.md

## gitproj Installed Files - prod

    /usr/
    |   |lib/
    |   |   |git-core/
    |   |   |   |git-proj
    |   |   |   |   |gitproj-com.inc*
    |   |   |   |   |gitproj-init.inc*
    |   |   |   |   |gitproj-add.inc*
    |   |   |   |   |gitproj-check.inc*
    |   |   |   |   |gitproj-clone.inc*
    |   |   |   |   |gitproj-config.inc*
    |   |   |   |   |gitproj-move.inc*
    |   |   |   |   |gitproj-pull.inc*
    |   |   |   |   |gitproj-push.inc*
    |   |   |   |   |gitproj-remote.inc*
    |   |   |   |   |gitproj-rm.inc*
    |   |   |   |   |gitproj-status.inc*
    |   |   |   |   |git-proj*
    |   |   |   |   |git-proj-add*
    |   |   |   |   |git-proj-check*
    |   |   |   |   |git-proj-clone*
    |   |   |   |   |git-proj-config*
    |   |   |   |   |git-proj-init*
    |   |   |   |   |git-proj-move*
    |   |   |   |   |git-proj-pull*
    |   |   |   |   |git-proj-push*
    |   |   |   |   |git-proj-remote*
    |   |   |   |   |git-proj-rm*
    |   |   |   |   |git-proj-status*
    |   |share/
    |   |   |doc/
    |   |   |   |git-proj/
    |   |   |   |   |LICENSE
    |   |   |   |   |README
    |   |   |   |   |config/
    |   |   |   |   |   |gitproj.config.system
    |   |   |   |   |   |gitproj.config.global
    |   |   |   |   |   |getproj.config.local
    |   |   |   |   |   |gitproj-config.test-dev
    |   |   |   |   |   |gitproj-config.test-prod
    |   |   |   |   |hooks/
    |   |   |   |   |   |pre-commit*
    |   |   |   |   |test/
    |   |   |   |   |   |outline.md
    |   |   |   |   |   |shunit2 -> shunit2.1*
    |   |   |   |   |   |shunit2.1*
    |   |   |   |   |   |test.inc
    |   |   |   |   |   |test-com-inc*
    |   |   |   |   |   |test-env.tgz
    |   |   |   |   |   |test-add.inc*
    |   |   |   |   |   |test-check.inc*
    |   |   |   |   |   |test-clone.inc*
    |   |   |   |   |   |test-config.inc*
    |   |   |   |   |   |test-init.inc*
    |   |   |   |   |   |test-move.inc*
    |   |   |   |   |   |test-pull.inc*
    |   |   |   |   |   |test-push.inc*
    |   |   |   |   |   |test-remote.inc*
    |   |   |   |   |   |test-rm.inc*
    |   |   |   |   |   |test-status.inc*
    |   |   |man/
    |   |   |   |man1/
    |   |   |   |   |gitproj.1.gz - generated, all user commands
    |   |   |   |man8/
    |   |   |   |   |gitproj.1.gz - generated, internal documentation
    
## After running git-proj-init local, or git-proj-clone

    /home/
    |   |USER/
    |   |   |.git.config - see "include.path" .gitproj.config.global
    |   |   |.gitproj.config.global
    |   |   |ANY-DIR/
    |   |   |   |PROJECT.raw/
    |   |   |   |   |DIR1/
    |   |   |   |   |   |DIR2/
    |   |   |   |   |   |  |LARGE-FILE.MP4
    |   |   |   |PROJECT/
    |   |   |   |   |.git/
    |   |   |   |   |   |config - see "include.path" ../../.gitproj.config.global
    |   |   |   |   |.gitproj.config.local
    |   |   |   |   |raw -> ../PROJECT.raw
    |   |   |   |   |DIR1/    
    |   |   |   |   |   |DIR2/    
    |   |   |   |   |   |   |LARGE-FILE.MP4 -> ../../raw/DIRS/LARGE-FILE.MP4 
    
## After running git-proj-init remote

    /MOUNT-DIR/
    |   |   |ANY-DIR/
    |   |   |   |PROJECT.raw/
    |   |   |   |PROJECT.git

## Git Config Definition Order

1. /etc/gitconfig and /etc/gitproj.config.system - optional
2. /home/USER/.gitconfig and .gitproj.config.global (include path location matters)
3. GIT_PROJECT/.git/config and GIT_PROJECT/.gitproj.config.local

## File include pattern - prod

    cBin=/usr/lib/git-core - set when a CMD is run
    cDoc=/usr/share/doc/gitproj

        $cBin/git-proj-CMD
                $cBin/gitproj-com.inc
                $cBin/gitproj-CMD.inc

## File include pattern - prod-test

    cTest=/usr/share/doc/gitproj/test - set when a test-*.inc is run
    cDoc=$cTest/.. or /usr/share/doc/gitproj
    cBin=/usr/lib/git-core

        $cTest/test-com-inc*
                $cBin/gitproj-com.inc
                $cTest/shunit2.1*

        $cTest/test-CMD.inc
                $cBin/gitproj-com.inc
                $cBin/gitproj-CMD.inc
                $cTest/test.inc
                $cTest/shunit2.1*

## File include pattern - dev

    cBin=DIR/gitproj/git-core - set when a CMD is run
    cDoc=$cBin/../doc or DIR/gitproj/doc

        $cBin/git-proj-CMD
                $cBin/gitproj-com.inc
                $cBin/gitproj-CMD.inc

## File include pattern - dev-test

    cTest=DIR/gitproj/doc/test - set when a test-*.inc is run
    cBin=$cTest/../../git-core or DIR/gitproj/git-core
    cDoc=$cTest/.. or DIR/gitproj/doc

        $cTest/test-com-inc*
                $cBin/gitproj-com.inc
                $cTest/shunit2.1*

        $cTest/test-CMD.inc
                $cBin/gitproj-com.inc
                $cBin/gitproj-CMD.inc
                $cTest/test.inc
                $cTest/shunit2.1*

## prod-test Environment

    cTest=/usr/share/doc/gitproj/test
    cTestEnv=~/test-gitproj.tmp
    HOME=$cTestEnv/root/home/john

Create with:

    mkdir $cTestEnv
    cd $cTestEnv
    tar -xzf $cTest/test-env.tgz

See dev-test Environment for more details
    
## dev-test Environment

    cTest=DIR/gitproj/doc/test
    cTestEnv=$cTest/../../..
    HOME=$cTestEnv/root/home/john

Create with:

    cd $cTestEnv
    tar -xzf $cTest/test-env.tgz

Creates:

    $cTestEnv/
    |   |test/
    |   |   |test-files.txt - this outline
    |   |   |root/
            |   |mnt/
            |   |   |disk-2/
            |   |   |usb-misc/
            |   |   |   |files-2021-08-12/
            |   |   |usb-video/
            |   |   |   |video-2020-04-02/
            |   |home/
            |   |   |john/ - $HOME
            |   |   |   |project/
            |   |   |   |   |beach/
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |file1.txt
            |   |   |   |   |   |   |file2.txt
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |file.txt
            |   |   |   |   |   |.git/
            |   |   |   |   |george/
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |notes.html
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |george.kdenlive
            |   |   |   |   |   |src/
            |   |   |   |   |   |   |final/
            |   |   |   |   |   |   |   |george.mp4
            |   |   |   |   |   |   |raw/
            |   |   |   |   |   |   |   |MOV001.MP3
            |   |   |   |   |   |   |   |MOV001.mp4
            |   |   |   |   |   |.gitignore
            |   |   |   |   |   |README.html
            |   |   |   |   |paulb/
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |notes.html
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |paulb.kdenlive
            |   |   |   |   |   |src/
            |   |   |   |   |   |   |final/
            |   |   |   |   |   |   |   |paulb.mp4
            |   |   |   |   |   |   |raw/
            |   |   |   |   |   |   |   |MOV001.MP3
            |   |   |   |   |   |   |   |MOV001.mp4
            |   |   |   |   |   |README.html
