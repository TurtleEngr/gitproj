# gitproj

## Notes

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
