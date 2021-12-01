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
    |   |   |LICENSE
    |   |git-core/
    |   |   |gitproj-com.inc*
    |   |   |gitproj-init.inc*
    |   |   |gitproj-add.inc*
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
    |   |   |git-proj-clone*
    |   |   |git-proj-config*
    |   |   |git-proj-init*
    |   |   |git-proj-move*
    |   |   |git-proj-pull*
    |   |   |git-proj-push*
    |   |   |git-proj-remote*
    |   |   |git-proj-rm*
    |   |   |git-proj-status*
    |   |test/
    |   |   |outline.md
    |   |   |shunit2 -> shunit2.1*
    |   |   |shunit2.1*
    |   |   |test-env.tgz -> ../../test-env.tgz
    |   |   |test-install.sh*
    |   |   |test.inc
    |   |   |test-com.sh*
    |   |   |test-add.sh*
    |   |   |test-clone.sh*
    |   |   |test-config.sh*
    |   |   |test-init.sh*
    |   |   |test-move.sh*
    |   |   |test-pull.sh*
    |   |   |test-push.sh*
    |   |   |test-remote.sh*
    |   |   |test-rm.sh*
    |   |   |test-status.sh*
    |   |.gitignore
    |   |Makefile
    |   |README.md
    |   |TODO.md

## gitproj Installed Files - prod

Note: The tests will only check for a valid installation. Unit test
will only be supported in a dev env. Running unit test on the
installed files would be tricky, and of low value.

    /usr/
    |   |lib/
    |   |   |git-core/
    |   |   |   |git-proj
    |   |   |   |   |gitproj-com.inc*
    |   |   |   |   |gitproj-init.inc*
    |   |   |   |   |gitproj-add.inc*
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

    cd ~/ANY-DIR/PROJECT
    git proj init

    or

    cd ~/ANY-DIR
    git proj clone -d /MOUNT-DIR/ANY-DIR/PROJ.git

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
    |   |   |   |   |   |config - see "include.path" ../.gitproj.config.global
    |   |   |   |   |.gitproj.config.local
    |   |   |   |   |raw -> ../PROJECT.raw
    |   |   |   |   |DIR1/    
    |   |   |   |   |   |DIR2/    
    |   |   |   |   |   |   |LARGE-FILE.MP4 -> ../raw/DIRS/LARGE-FILE.MP4 
    
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
5. GIT_DIR/.gitproj.config.local (include.path at beginning of GIT_DIR/.gitproj.config.$HOSTNAME)
6. GIT_DIR/.gitproj.config.$HOSTNAME (include.path at end of GIT_DIR/.git/config)
7. Env. var. will override corresponding .git config vars.
8. Command line options will override env. var. and corresponding .git config vars.

## Variable Naming Convention

* Globals that came from command line, config files, or external to
the scripts, begin with "gp" (global paramater). For example: gpVAR
(if they are not already defined before script (#7), set the initial
value to files #1 through #6. The command line option can always set
the value (#8)

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
the include file reads writes to.

* At the end of an include file, call a function that will define
defaults for the important globals used by the include file.

* For user callable scripts, do minimal setup, include files with
common functions and functions specific to the script. Put as much as
possible into functions in the commands include file, so that the
functions can be directly tested with unit test scripts found in
doc/test.

* Minimal vars: gpBin, cCurDir, gpDoc, gpTest if a test script.
All other vars can be defined from include files or from git config vars.

* Define gpCmdName at the top of each each script, that is a main script
called by a user.

* The -p option is not used with the "read" command, because this
prompt is not captured with the test scripts. So use "echo -n" for the
prompts before the read command.

* fLog and fError messages

    * Put this at the beginning or end of each file

        export tSrc=${BASH_SOURCE##*/}

    * Put this in each function that calls fError or fLog

        local tSrc=${BASH_SOURCE##*/}

    * in Error and Log pass this argument:

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
            |   |   |disk-2/		- $cDatMount1
            |   |   |usb-misc/ 		- $cDatMount2
            |   |   |   |files-2021-08-12/ 
            |   |   |usb-video/ 	- $cDatMount3
            |   |   |   |video-2020-04-02/
            |   |home/
            |   |   |john/		- $HOME
            |   |   |   |project/
            |   |   |   |   |beach/	- $cDatProj3
            |   |   |   |   |   |doc/
            |   |   |   |   |   |   |file1.txt
            |   |   |   |   |   |   |file2.txt
            |   |   |   |   |   |edit/
            |   |   |   |   |   |   |file.txt
            |   |   |   |   |   |.git/
            |   |   |   |   |george/	- $cDatProj1
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
            |   |   |   |   |paulb/		- $cDatProj2
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