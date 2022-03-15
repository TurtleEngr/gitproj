# Raw Design

See also: [enh-raw Pros/Cons](https://github.com/TurtleEngr/gitproj/blob/develop/test/dev-doc/enhancements/raw-pros-cons.md)

## Supported Styles

Support multiple raw file management styles.

* `Local File System` - This includes mounted drives: local mounts, usb
mounts, and NFS mounts.

* `SSH Remotes` - user access to the remote server is needed with a user
id and key.

* `RClone Remotes` - user access to the remote service is needed with a
pre-defined rclone authentication.

* `CVS Remotes` - CVS is used (with SSH) to save files remotely.

## Config Vars

* `raw-style` [`local|ssh|rclone|cvs`]

* `raw-path`

    - if local: absolute file system path. `PROJ-PATH` This should be
      an absolute path.
    
    - if ssh: `USER@HOST:PROJ-PATH` USER default will be
      $USER. PROJ-PATH can be an absolute path or a path relative to
      the user's home dir. Also, raw-key config needs to be defined.
    
    - if rclone: `PATH:RCloneName`. PATH is the location of the
      rclone.conf file. Default: $HOME:.config/rclone/rclone.conf
      RCloneName is the service name. If SFTP is used, then keys need
      to be setup with sshagent. See raw-key
    
    - if cvs: CVSROOT - `:ext:USER@HOST:REPO-PATH|PROJ-PATH` REPO-PATH
      is an absolute path or a a path relative to the user's home dir.
      PROJ-PATH is the raw dir path directly under REPO-PATH.  Also,
      raw-key config needs to be defined.

* raw-key - path to ssh key. For services that need a ssh key, this
  key will be added to sshagent. Passwordless ssh keys will not be
  supported (i.e. the ssh key must be encrypted.)
