TODO item:

* To use a true remote (over network) with cvs and git, use ssh protocol.
  Requires: ssh-client and ssh-server

Locally mounted disk

    git clone ssh://$HSER@localhost:/[mount]/[path]/[proj].git

Remote server

    git clone ssh://$HSER@HOSTNAME:/[path]/[proj].git

Local example:

    git clone ssh://turtle@localhost:/media/turtle/usb-video/video-proj/seal.git

Remote example:

    git clone ssh://turtle@example.com:/repo/video-proj/seal.git

  # Raw dir pull example with rsync
  export RSYNC_RSH=ssh
  cd raw
  rsync -az --delete ssh://turtle@example.com:/repo/video-proj/seal.raw/ .
  # Raw dir push example with rsync
  cd raw
  rsync -az --delete ./ ssh://turtle@example.com:/repo/video-proj/seal.raw
