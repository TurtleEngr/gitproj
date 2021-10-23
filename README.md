# gitproj

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
