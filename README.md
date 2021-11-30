# gitproj

## Version 0.1.0

This is still under development.

## Outline

The Usage help text in the main sub-command files have the most up-to-date
descriptions.

For a more complete outline, see: doc/test/outline.md

### Main sub-commands, in git-core/

* git-proj - general help - mostly implemented
* git-proj-init - initialize a local git-proj repo - mostly implemented
* git-proj-remote - initialize new remote repo from a local one - mostly implemented
* git-proj-push - git push and push proj.raw files (rsync or rclone)
* git-proj-pull - git pull and pull proj.raw files (rsync or rclone)
* git-proj-status - git status and status of binary files
* git-proj-clone - create a git workspace and raw workspace from a remote
* git-proj-add - add a binary file to raw/
* git-proj-config - mainly for managing new "mount-dirs" or different hosts
* git-proj-check - maybe move this to git-proj-status

### Library functions in git-core/

* gitproj-com.inc
* gitproj-init.inc
* gitproj-remote.inc

### Config files in doc/config/

* gitconfig.default - copied to ~/.gitconfig, if none exists
* gitproj.config.global - copied to ~/.gitproj.config.global, if none exists
* gitignore.default - copy to PROJ/.gitignore
* gitproj.config.local - copy to PROJ/.gitproj.config.local included
  by .gitproj.config.HOSTNAME
* gitproj.config.HOSTNAME - copy to PROJ/.gitproj.config.HOSTNAME
  included by .git/config

### Test scripts in doc/test/

test-com.sh
test-com2.sh
test-gitproj.sh
test-init.sh
test-remote.sh

### Test Environment files

Currently these are symlinks to the directory above the git workspace.
These will be moved to a public space when the tool is mostly done, or
when automated testing is setup.

See the Makefile for how these are built.

* test-env.tgz@ - manually built
* test-env_HomeAfterBMove.tgz@
* test-env_ProjAfterGInit.tgz@
* test-env_ProjLocalDefined.tgz@
* test-env_TestDestDirAfterMkRemote.tgz@

----

## Notes

These are just some of the initial design notes. The tool is very
different from this now.

https://coderwall.com/p/bt93ia/extend-git-with-custom-commands

https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html

    source "$(git --exec-path)/git-sh-setup"
       require_work_tree_exists
       cd_to_toplevel
       require_clean_work_tree rebase "Please commit or stash them."

https://mirrors.edge.kernel.org/pub/software/scm/git/docs/git-sh-setup.html

https://github.com/nvie/gitflow

https://github.com/nvie/gitflow/blob/develop/git-flow-init

https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks
https://git-scm.com/docs/githooks

## Design

### Move existing files to PROJ.raw/DIRS and make symlinks

    cd /home/video/seal
    mkdir ../seal.raw
    ln -s ../seal.raw raw
    cd edit/src/own/video
    mkdir -p  ../../../../../seal.raw/edit/src/own/video
    mv *.MP4 *.JPG *.jpg  ../../../../../seal.raw/edit/src/own/video
    ln -s ../../../../../seal.raw/edit/src/own/video/* .

### Best starting structure (only one symlink to manage)

    PROJ.raw/
            DIRS/
                    *.MP4
    PROJ/
            .git/
            PROJ.raw/ -> ../PROJ.raw/

### proj-new-local
        mkdir ../seal.raw
        ln -s ../seal.raw .
        git add *
        git ci -m Added

### proj-new-remote /mnt/usb-video/video-2021-10-22

    proj-new-remote /mnt/usb-video/video-2021-10-22

### git-push-raw

        mkdir /mnt/usb-video/video-2021-10-22/seal.raw
        rsync -rCP $cTop/../seal.raw/* /mnt/usb-video/video-2021-10-22/seal.raw

### pushes:

    git push origin develop
    git-push-raw
        mkdir /mnt/usb-video/video-2021-10-22/seal.raw
        rsync -rCP $cTop/../seal.raw/* /mnt/usb-video/video-2021-10-22/seal.raw

### Get local

    proj-get-local /mnt/usb-video/video-2021-10-22/seal.git
    cd seal
    mkdir ../seal.raw
    rsync -rCP /mnt/usb-video/video-2021-10-22/seal.raw/* $cTop/../seal.raw

### Pulls

    git pull origin develop
    git-pull-raw
        mkdir ../seal.raw
        rsync -rCP /mnt/usb-video/video-2021-10-22/seal.raw/* $cTop/../seal.raw
