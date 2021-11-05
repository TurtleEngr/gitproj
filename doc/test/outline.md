# Outline of File Organization

## gitproj Source Repo - dev

    gitproj/
    |   |doc/
    |   |   |config/
    |   |   |   |gitproj.config.system
    |   |   |   |gitproj.config.global
    |   |   |   |getproj.config.local
    |   |   |   |gitproj.config.test
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
    |   |   |   |test-env.tgz -> ../../../test-env.tgz
    |   |   |   |test-install.sh*
    |   |   |   |test.inc
    |   |   |   |test-com.sh*
    |   |   |   |test-add.sh*
    |   |   |   |test-check.sh*
    |   |   |   |test-clone.sh*
    |   |   |   |test-config.sh*
    |   |   |   |test-init.sh*
    |   |   |   |test-move.sh*
    |   |   |   |test-pull.sh*
    |   |   |   |test-push.sh*
    |   |   |   |test-remote.sh*
    |   |   |   |test-rm.sh*
    |   |   |   |test-status.sh*
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

Note: The tests will only check for a vailded installation. Unit test
will only be supported in a dev env. Running unit test on the
installed files would be tricky, and of low value.

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
    |   |   |   |   |hooks/
    |   |   |   |   |   |pre-commit*
    |   |   |   |   |test/
    |   |   |   |   |   |outline.md
    |   |   |   |   |   |shunit2 -> shunit2.1*
    |   |   |   |   |   |shunit2.1*
    |   |   |   |   |   |test-install.sh*
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

        $cTest/test-com.sh*
            . $cTest/test.inc
		fComSetup
            . $cBin/gitproj-com.inc
		fComSetGlobals
	    fComRunTests
                . $cTest/shunit2.1*

        $cTest/test-CMD.sh
            . $cTest/test.inc
		fComSetup
            . $cBin/gitproj-CMD.inc
                . $cBin/gitproj-com.inc
		    fComSetGlobals
		. fSetGlobals
	    fComRunTests
                . $cTest/shunit2.1*

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
            |   |   |disk-2/ - $cDatMount1
            |   |   |usb-misc/ - $cDatMount2
            |   |   |   |files-2021-08-12/ 
            |   |   |usb-video/ - $cDatMount3
            |   |   |   |video-2020-04-02/
            |   |home/
            |   |   |john/ - $HOME
            |   |   |   |project/
            |   |   |   |   |beach/ - $cDatProj3
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |file1.txt
            |   |   |   |   |   |   |file2.txt
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |file.txt
            |   |   |   |   |   |.git/
            |   |   |   |   |george/ - $cDatProj1
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
            |   |   |   |   |paulb/ - $cDatProj2
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |notes.html
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |paulb.kdenlive
            |   |   |   |   |   |src/
            |   |   |   |   |   |   |final/
            |   |   |   |   |   |   |   |paulb.mp4 - $cDatProj2Big
            |   |   |   |   |   |   |raw/
            |   |   |   |   |   |   |   |MOV001.MP3 - $cDatProj2Big
            |   |   |   |   |   |   |   |MOV001.mp4 - $cDatProj2Big
            |   |   |   |   |   |README.html
