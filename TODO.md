# TODO

After 1.0, move these to github issues.

--------------------

## bug (defect, fix, needs to be changed)

* in git-proj-init Testing Results:

    In help: saved to ~/.gitproj.config and [project]/gitproj/config.$HOSTNAME.
    In usage: isn't -l required?
    The -s, -m, -f options seem to only be needed if using the -a option,
    otherwise they can be prompted for.
    The -l option assumes you have a dir with content already in it. Is fill path needed, or can it just...
    Project Path (-l) [/home/bruce/test/seal/quit]?
    	huh, quit? s/b
	Project Path (-l) [/home/bruce/test/seal]
	Project Name [seal] Continue [y/n]?

* Move the [gitproj] sections in in .gitproj.config.global to
  .gitconfig so the include.path does not need to be maintained? Also
  removing the include path from PROJ/.git/config will make is easer
  to not forget to include the --include option to "git config". Also
  there will be fewer files for the user to manage.

* Maybe copy [gitproj] sections from PROJ/.gitproj.config.local to
  PROJ/.git/config and update the values needed for the host? That way
  the include path can be removed. The file is usually only changed when
  it is crated by init or clone. The HOST option can be removed from
  the fComConfigGet/Set functions. .gitproj.config.local should remain
  as a template for new hosts.

* in git-proj-remote Testing Results:

    "git proj remote" lists all the files in /media/$USER/DRIVE/DIR1/DIR2
    maybe limit it to only one level (DIR1)?

* Remove the duplicate path, in .git/config (introduced by git-proj-clone)
  [include]
        path = ../.gitproj.config.testserver2
        path = ../.gitproj.config.testserver2


--------------------

## documentation

* Cleanup the user docs. (bump the "patch" number for doc-only changes)

* Make and cleanup the internal docs.

--------------------

## infrastructure (development, cleanup code, tests, build, package, etc)

* Remove the need for the cCurDir variable (has it ever been used?)

* Refactor the test setup so it is simpler.

* Copy the test-env tar files to a "public" place. And implement the
Makefile targets for managing them.

* Get travis-ci working. Run tests.

* More travis-ci.
    * Run tests with changes to develop or main branches.
    * Build the package.
    * Install the package.
    * Verify the install.

--------------------

## enhancement

* Imp a command that with show the files in REMOTE-PATH/PROJ.raw
  git proj status does a diff, which is good, maybe just add the
  option to have th diff show the files that are the same.

* Code git-proj-config.

* Code git-proj-add (support adding whole directory trees of raw files)

----

## Enhancement Major

* Version increment wizard script. Increment the
  major/minor/path/rc/build vars based on Q/A. See
  `test/dev-doc/enh-ver-wizard.md`

* Decided on which remote network service to implement. Do all, only
  do one or two? Some methods are not compatable.

* Future feature: manage different versions of binary files with a
  process similar to rsnapshot. See `test/dev-doc/enh-raw-rsnapshot.md`
  and `test/dev-doc/enh-raw-ssh.md`

* Manage the different versions of large binary files with "cvs".
  See `test/dev-doc/enh-raw-cvs.md`

* To use a true remote (over network) with cvs and git, use ssh protocol.
  Requires: ssh-client and ssh-server. See `test/dev-doc/enh-raw-ssh.md`

* Use rclone for raw/ files. See `test/dev-doc/enh-raw-rclone.md`

----

## done (closed)

* Code test-com.sh, gitproj-com.inc - mostly done

* Code test-gitproj.sh, git-proj - mostly done

* Make a small test project for setup/teardown - done (see Makefile)

* Code test-init.sh, git-proj-init, gitproj-init.inc  - mostly done

* Setup defaults for for ~/.gitconfig, etc - mostly done

* Code git-proj-remote - split out from git-proj-init - done

* Refactor where the binary files live. Managing the raw symlink and
  the file outside of the GIT_DIR is messy. Simplify: create "raw" dir
  under GIT_DIR, add "raw" to .gitignore, and put all binary files in
  "raw". Now it is easily found; no indirection or other area to
  manage.
  The user is also free to make "raw" a symlink, to balance disk space
  (the gitproj code just needs to "follow" symlinks when copying files
  from raw to/from the "remote" location.)  Done

* Refactor how values are "returned". Rather than echoing the value to
  be "caught", or setting the actual (external) variable, it would be
  better to see a "temporary" external variable, then set the "actual"
  variable to the temp variable. The advantage: testing is easier, and
  independent of "special" variables.

* Version management. If installed Major number is different from
  expected "ver" in config. then exit with error. Ask user to install
  the older version, or they should make changes to be compatible,
  then set the "ver" number in the config. Or better supply an option
  to automatically upgrade, and it will set "ver" in the configs.  If
  the Minor number is different, just warn the user(?) and continue.
  Mostly done

* Code git-proj-remote - done

* Code git-proj-push - done

* Code git-proj-pull - done

* Update status configs: - Done
    * proj-status = installed, install-errors (add)
    * local-status = not-defined, defined (rename proj-status)
    * remote-status = not-defined, defined, multiple-defined

* Refactor .gitproj.config.local, .gitproj.config.HOSTNAME mgmt - Done
  * .gitproj.config.local will only be used as a default to create
    .gitproj.config.HOSTNAME, i.e. it will not be used by "git config --get"
  * Remove doc/config/.gitproj.config.HOSTNAME
  * git-proj-init may save changes to .gitproj.config.local, i.e. when
    a project it first created.
  * If in a gitproj git dir, at start of cmd, look for .gitproj.config.HOSTNAME
    If not found the cp .gitproj.config.local .gitproj.config.HOSTNAME,
    and set .git/config include too .gitproj.config.HOSTNAME
    Tell user to review .gitproj.config.HOSTNAME and .gitproj.config.local

* Code git-proj-clone - done

* Make a fComYesNo() function with fComSelect - done

* Code git-proj-status - done

* Code and test pre-commit checks. - done

* Move "auto-move" out of hooks config.  Use this in git-proj-add (or move)
  add: find binary files that are greater than binary-file-size-limit
  add: move and create symlink, or just move to raw/ - done

* Create install.sh to install directly from development git
  dir. Implement it with a pseudo root var, so that the script can be
  used to create the dist/ dir that EPM will use.
  NO - instead created a make install target. - done

* Make OS installers. Use EPM to make a portable installer and a native
  deb installer. - done

* Add a CHANGES.md file - done

* Fix -h so it works with "git proj CMD" CLI
Defect in the design 'git proj CMD ARGS' calls: git-proj CMD ARGS
Fix: look for first arg so see if it is a CMD. - done

* Fixup the github README.md so that it can be used in the dist package too. - done

* Change the logging messages so that they don't output the
  timestamp. Maybe don't log to syslog? Change default syslog to false. - done
