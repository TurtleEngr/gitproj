# TODO

* Code test-com.sh, gitproj-com.inc - mostly done

* Code test-gitproj.sh, git-proj - mostly done

* Make a small test project for setup/teardown - done (see Makefile)

* Code test-init.sh, git-proj-init, gitproj-init.inc  - mostly done

* Update status configs:
  - proj-status = installed, install-errors (add)
  - local-status = not-defined, defined (rename proj-status)
  - remote-status = not-defined, defined, multiple-defined

* Setup defaults for for ~/.gitconfig, etc - mostly done

* Code git-proj-remote - split out from git-proj-init - done

* Refactor where the binary files live. Managing the raw symlink and
the file outside of the GIT_DIR is messy. Simplify: create "raw" dir
under GIT_DIR, add "raw" to .gitignore, and put all binary files in
"raw". Now it is easily found; no indirection or other area to manage.

The user is also free to make "raw" a symlink, to balance disk space
(the gitproj code just needs to "follow" symlinks when copying files
from raw to/from the "remote" location.)

Done

* Refactor how values are "returned". Rather than echoing the value to
be "caught", or setting the actual (external) variable, it would be
better to see a "temporary" external variable, then set the "actual"
variable to the temp variable. The advantage: testing is easier, and
independent of "special" variables.

* Code git-proj-clone

* Code git-proj-add (support adding whole directory trees)

* Code git-proj-push

* Code git-proj-pull

* Code git-proj-status

* Code git-proj-config

* Remove the need for cCurDir ?

* Refactor the test setup so it is simpler

* Get travis-ci working. Run tests.

* Build: make a script to inc version tag "major.minor.fix-rc.N+build" parts
  See: https://semver.org/ for a RegEx pattern matcher (so imp. with Perl)
  This supports a proper sub-set of semver.
    sem-ver [-M] [-m] [-p] [-b] FILE
      -M inc major, set minor, see -m (if rc, inc it)
      -m inc minor, set patch to 0, see -p (if rc, inc it)
      -p inc patch, if "rc", set it to 1, if build, set it to 0, (if
         rc, inc it)
      -r inc the release number, if none, inc with -m option, then insert
         "-rc.1" after "fix" and before +, if build set it to 1
      -b inc build, if none, append "+1"
      -c clear release and build parts (do this afer a "release")
      If file does not exist, create file with 0.1.0
      If no option, output the full version
      -v output with no build part
      -V output just the major.minor.fix parts
      Difference compare with expected "ver"
      -d "ver" - Build numbers are ignored. Return
         -2 if ver < FILE if Major part is <
         -1 if ver < FILE
         0 if ver = FILE
         1 if ver > FILE
         2 if ver > FILE if Major part is >

* Version management. If installed Major number is different from
  expected "ver" in config. then exit with error. Ask user to install
  the older version, or they should make changes to be compatable,
  then set the "ver" number in the config. Or better supply an option
  to automatically upgrade, and it will set "ver" in the configs.  If
  the Minor number is different, just warn the user(?) and continue.

* Cleanup the user docs. (bump the "patch" number for doc-only changes)

* Make and cleanup the internal docs.

* Make installer. Use epm to make portable installer and native deb installer.

* Verify install with gitproj-com.test

* Fixup gitproj-com.test so it can be run from /usr/share/doc/git-proj
or from gitproj working dir.

* More travis-ci. Run tests with changes to develop or main
branches. Build the package. Install the package. Verify the install.
