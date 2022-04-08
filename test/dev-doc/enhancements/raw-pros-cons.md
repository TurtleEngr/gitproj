# enh-raw Pros/Cons

[Decide on which network service to implement for remotes #18](https://github.com/TurtleEngr/gitproj/issues/18)

Pros and Cons for the different ways of managing remote raw/ files.

See also: [Raw Design](https://github.com/TurtleEngr/gitproj/blob/develop/test/dev-doc/enhancements/raw-design.md)

--------------------

## enh-raw-ssh

[For full remotes (over network), use ssh #21](https://github.com/TurtleEngr/gitproj/issues/21)

### Pros

* Can be configured to support remote git access and remote file access (rsync)

* If openssh-server is on a local computer, then localhost can be used
  to access the files, locally

* Files can be synced with rsync over ssh (--delete option)

* User authentication can be setup with login and/or ssh keys (keys is
  recommended)

* Files on the remote server can be directly managed in the
  filesystem, when signed into the remote server.  Then `git proj
  pull` can be used to sync the local copy.

* Since rsync is already being used for local copying, it would be
"easy" to extend it by just adding USER@HOST to the remote paths.

### Cons

* Requires openssh-server package on the remote computer

* Requires port 22 be open

* Status reports, i.e. diffs, will need to be done through ssh

* Only the latest version of files are kept on the remote server.

* The remote ssh server is not "distributed", so you would want to
  pick "one" remote ssh host for "main".

--------------------

## enh-raw-cvs

[Manage the different raw/ versions of large binary files with "cvs" #20](https://github.com/TurtleEngr/gitproj/issues/20)

### Pros

* Files in raw/ can be managed with all of the CVS commands,
  independent from the `git proj`, and `git proj` will still function.

* Reducing the size of files on the remote is easy--CVS supports easy
  total removal of any version of the files (unlike git).

* If openssh-server package is on a local computer, then localhost can
  be used to access the files, locally

### Cons

* Requires cvs package

* Requires ssh access

* cvs management can be mostly hidden with `git proj`, but some
  understanding of CVS would be useful.

* cvs is not "distributed", so you would want to pick "one" remote
  host for "main".

* raw file > 2GB will not transfer over the network. (They can be
transferred locally.) A workaround: split the file into smaller
pieces when pushing. Then reassemble them when pulling.

--------------------

## enh-raw-rclone

[Use rclone for remote raw/ files #22](https://github.com/TurtleEngr/gitproj/issues/22)

### Pros

* Very similar to rsync, so easy to setup for remote raw/ file access.

### Cons

* Not clear if rclone can be easily setup to work with remote git repos.

* This initial configuration of all the different rclone service
  options makes it hard to make this "invisible". Most likely the user
  will be required to define the initial configuration, then set the
  "name" of the service in a `gitproj-config` variable.

* Most storage services are not "distributed", so you would want
  to pick "one" remote service for "main".

--------------------

## enh-raw-rsnapshot

[Manage different versions of raw/ files with a backup style #19](https://github.com/TurtleEngr/gitproj/issues/19)

### Pros

* The files are managed with a simple directory structure that is a
  snapshot of the raw/ directory and files.

* Managing the files on the remote server, i.e. cleaning up, is
  simple--just remote whole directory snapshots, or single files
  across snapshots.

* Finding older files can be done by looking at the datestamps on the
  snapshot dirs.

### Cons

* Files are versioned as a set. Will the user do it frequently enough?

* Requires ssh/rsync access

* Requires a remote filesystem that supports hardlinks.

* Every commit of raw/ (where the files are different) would create a
  new directory on the remote server.

* The remote ssh server (used by rsnapshot) is not "distributed", so
  you would want to pick "one" remote ssh host for "main".

* Backing up the remote raw/ area would require preserving the hard
  links.  Or maybe only the latest directory would be backed up.

* This remote management style would be the hardest one to implement.

* This would likely lead to a lot of disk space being used. (And
removing "old" dirs is not fast.)

* This is more of a "backup" process, so if you want backups, then
  implement rsnapshot on the remote raw repository (i.e. the one using
  ssh).
  
