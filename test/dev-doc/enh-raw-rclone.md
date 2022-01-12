TODO item

* Use rclone for raw/ files.

This would be similar to `test/dev-doc/enh-ssh-remote.md`, but only
for raw/ files. (sftp would be used for ssh access.)

rclone support a large number of storage services. After configuring a
particular service, the rclone upload/download/sync commands are the
same for all services. (We would put the rclone name, that the user
configured, in a gitproj config var.)
