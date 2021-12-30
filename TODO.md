# TODO

* Code git-proj-status

* Code and test pre-commit checks.

* Move "auto-move" out of hooks config.  Use this in git-proj-add (or move)
  add: find binary files that are greater than binary-file-size-limit
  add: move and create symlink, or just move to raw/

* Create install.sh to install directly from development git
  dir. Implement it with a pseudo root var, so that the script can be
  used to create the dist/ dir that EPM will use.

* Make OS installers. Use EPM to make a portable installer and a native
  deb installer.

* Code git-proj-config

* Remove the duplicate path, in .git/config (introduced by git-proj-clone)
  [include]
        path = ../.gitproj.config.testserver2
        path = ../.gitproj.config.testserver2

* Code git-proj-add (support adding whole directory trees)

* Remove the need for cCurDir ?

* Refactor the test setup so it is simpler

* Copy the test-env tar files to a "public" place.

* Get travis-ci working. Run tests.

* Make a script to inc version tag "major.minor.fix-rc.N+build" parts
  See: https://semver.org/ for a RegEx pattern matcher (so imp. with Perl)
  This supports a proper sub-set of semver.

        sem-ver [-M] [-m] [-p] [-b] [-d ver] FILE[:key]
            If FILE does not exist, create file with 0.1.0
            If no option, output the full version
            -M inc major, set minor, see -m (if rc, inc it, set all others to 0)
            -m inc minor, set patch to 0, see -p (if rc, inc it, set all others to 0)
            -p inc patch (if rc, inc it, set build to 0)
            -r inc the release number, if none, inc patch, then insert
               "-rc.1" after "patch" and before +, if build set it to 0
            -b inc build, if none, append "+1"
            -c clear release and build parts (do this after a "release")
            -v output with no build part
            -V output just the major.minor.fix parts
            -d "ver" - only compare major, minor, and patch parts. Ignore the rest.
            Difference compare with expected "ver"
                -3 if ver < FILE if Major part is <
                -2 if ver < FILE if minor part is <
                -1 if ver < FILE if patch part is <
                 0 if ver = FILE
                 1 if ver > FILE if patch part is >
                 2 if ver > FILE if minor part is >
                 3 if ver > FILE if Major part is >
         
    Rather than FILE, support reporting and updating the version
    number in a git variable. That means the git config file and
    variable key needs to be defined. Use: FILE:KEY For example:
    .gitproj.config.local:gitproj.config.proj-ver

* Cleanup the user docs. (bump the "patch" number for doc-only changes)

* Make and cleanup the internal docs.

* Verify install with gitproj-com.test - NO!

* Fix-up gitproj-com.test so it can be run from /usr/share/doc/git-proj
  or from gitproj working dir. - NO!

* More travis-ci.
    * Run tests with changes to develop or main branches.
    * Build the package.
    * Install the package.
    * Verify the install.

----

## Future

* Future feature: manage different versions of binary files with a
  process similar to rsnapshot. Use rsync's 3-way link feature, when
  there are differences. A date-stamped dir can be used for the
  different version "snapshots". Requires Linux formatted file-systems
  that support hard-links. May only work well with "mounted" disks.

* Or manage different versions of large binary files with "cvs". cvs
  will track different versions of binary files as needed. If you want
  to reduce the size of a versioned binary file, cvs has a very easy
  way of removing the older versions. (A process that is very hard
  with git.) This script shows one way that I am already using:e
  https://github.com/TurtleEngr/my-utility-scripts/blob/develop/bin/vid-rm-ver

* Future feature: support remote large files across the network. Also
  support alternate remote sources, not just "origin".

----

## Done

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

