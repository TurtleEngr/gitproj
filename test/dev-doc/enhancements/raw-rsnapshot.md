TODO item

* Future feature: manage different versions of binary files with a
  process similar to rsnapshot.

A snapshot of the current raw/ dir would be made and copied to the
remote location with a new directory being created, if there are any
file differences. Hard links will be made to files that are the same.
(The new directory could include a datestamp in its name.) With this
process deleted or changed files can be found based on time. To
cleanup, the older directories would be removed on the remote.

Use rsync's 3-way link feature, when there are differences.  Requires
a Linux formatted file-systems that supports hard-links.

This would probably require `enh-ssh-remote.md` for this to be useful.
However mounted discs would be easy to support.
