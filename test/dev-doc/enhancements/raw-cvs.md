TODO item:

* Or manage different versions of large binary files with "cvs".

cvs will track different versions of binary files as needed. If you
want to reduce the size of a versioned binary file, cvs has a very
easy way of removing the older versions. (A process that is very hard
with git.) This script shows one way that I am already using:
https://github.com/TurtleEngr/my-utility-scripts/blob/develop/bin/vid-rm-ver

  # Raw dir examples with CVS (non-root user should have write
  # permission to /[path]
  export CVS_RSH=ssh
  export CVS_SSH=ssh
  export CVSUMASK=0007
  export CVSROOT=:ext:[user]@example.com:/[path]/[proj].cvs

  # Create the top git-proj.cvs for all raw files, and setup raw/
  ssh [user]@example.com cvs -d /[path]/[proj].cvs init
  ssh [user]@example.com mkdir /[path]/[proj].cvs/[proj].raw
  export CVSROOT=:ext:[user]@example.com:/[path]/[proj].cvs
  cd raw/
  mkdir tmp
  cd tmp
  cvs checkout [proj].raw
  mv CVS ..
  cd ..
  rmdir tmp
  git config -f [HOST] gitproj.config.cvsroot $CVSROOT
  git config -f [HOST] gitproj.config.cvsrepo [proj].raw

...examples for removing all by HEAD version of raw files.
