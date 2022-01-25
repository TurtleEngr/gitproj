<div>
    <hr/>
</div>

    git proj config [-e pExternalPath] [-r pRawPath]

       Define or redefine the gitproj options.

       -e pExternalPath

          If the external repo is mounted at a different path than the
          original "origin", then use this command to set the origin to
          the new mount point.

       -r pRawPath

          Redefine the location of the local raw file dir. The "raw"
          symlink will be updated.

       -c on/off

          Set a commit hook to not allow commits for "large" binary files.

       -s pMaxSize

          Set the max size for commits of binary files.

Usage:
        proj-set-remote pPath

Description
        Use this script if the mount path is different from the origin
        remote. pPath is the path pointing the current repo's remote
        repo on an external drive.

        Setup a different host.

        Review and change the current settings interactively.

Example
        The origin is currently set to: /mnt/usb-video/video-2019-04-01/trip.git
        But the drive is now mounted at /mnt/usb-drive, use:

            proj-set-remote /mnt/usb-drive/video-2019-04-01

        will set the origin to: /mnt/usb-drive/video-2019-04-01/trip.git
        if trip.git is found at the specified path.
