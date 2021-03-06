# TODO

Move these to github issues.

--------------------

## bug (defect, fix, needs to be changed)

--------------------

## documentation

--------------------

## infrastructure (development, cleanup code, tests, build, package, etc)

--------------------

## enhancement 1.0

----

## Enhancement Major 2.0?

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
  add: find binary files that are greater than binary-file-size
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

* Remove the need for the cCurDir variable (has it ever been used?) - done

* in git-proj-init Testing Results: - Done

    In help: saved to ~/.gitproj.config and [project]/gitproj/config.$HOSTNAME.
    In usage: isn't -l required?
    The -s, -m, -f options seem to only be needed if using the -a option,
    otherwise they can be prompted for.
    The -l option assumes you have a dir with content already in it. Is fill path needed, or can it just...
    Project Path (-l) [/home/bruce/test/seal/quit]?
        huh, quit? s/b
        Project Path (-l) [/home/bruce/test/seal]
        Project Name [seal] Continue [y/n]?

* in git-proj-remote Testing Results: - done

    "git proj remote" lists all the files in /media/$USER/DRIVE/DIR1/DIR2
    maybe limit it to only one level (DIR1)?

* Remove the duplicate path, in .git/config (introduced by git-proj-clone)
  [include]
        path = ../.gitproj.config.testserver2
        path = ../.gitproj.config.testserver2
  NA - so done

* Rename remote-raw-dir to remote-raw-origin, and gpRemoteRawDir to
  gpRemoteRawOrigin - done

* Remove "gitproj.hook.source" and "gpHookSource" from configs, code,
  tests - done

* in git-proj-remote: check to see if a remove-raw-origin or git
  origin have already been setup. If no, continue. If yes, ask for
  override. - done

* Cleanup: remove gpMountDir. gpHookSource. - done

* Refactor: Move the [gitproj] sections in in .gitproj.config.global to
  .gitconfig so the include.path does not need to be maintained? Also
  removing the include path from PROJ/.git/config will make is easier
  to not forget to include the --include option to "git config". Also
  there will be fewer files for the user to manage.
  Only $gpDoc/gitconfig is needed. - done

* Refactor: Maybe copy [gitproj] sections from
  PROJ/.gitproj.config.local to PROJ/.git/config and update the values
  needed for the host? That way the include path can be removed. The
  file is usually only changed when it is crated by init or clone. The
  HOST option can be removed from the fComConfigGet/Set
  functions. .gitproj.config.local should remain as a template for new
  hosts.  Only PROJ/.gitproj is needed (i.e. .gitproj.config.local and
  .gitproj.config.HOST files are not needed. .gitproj.config.HOST vars
  are put in PROJ/.git/config. This works because git-proj-init or
  git-proj-clone will set the HOST relevant values. - done

* Cleanup the user docs. (bump the "patch" number for doc-only changes)

* Make and cleanup the internal docs. Describe more of the block-file
structure, and some coding style quirks (e.g. [ $x -ne 0 ] vs
[ $x -eq 1 ]

* Also gitproj.config.ver should be set to $gpDoc/VERSION

* Add "expand" to rm-trailing-sp script.

* Put rm-trailing-sp script in $gpDoc/contrib dir

* git-prog typo in readme - done

* tutorial
	fix headings, not bullets - done
	C<cmd> cannot be used in <pre> areas - done
	fix POD error - done

* Change "subsystem" to "subcommand" - done

* Write the tutorial: create_a_git-proj_repo - done

* Add check: on creation of remote raw, create a file with the git
origin in it. On clone, push, pull, the actual origin can be compared
to the origin in the remote raw file. Problem: the git origin can be
different on different systems, because of different mount points. Fix,
just compare the last dir name--it should match origin.git - done

* Why was git-flow pkg not found by init? Line: 264 - fixed

* ../.. is still showing up - fixed

* Run spell check on all the docs.

* Implement a command that will show the files in REMOTE-PATH/PROJ.raw
  `git proj status` does a diff, which is good, maybe just add the
  option to have the diff show the files that are the same. - done

* Copied the bug, infrastructure, and enhancements to "issues"
