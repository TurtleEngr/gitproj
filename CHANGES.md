# git-proj CHANGES

[//]: # (The CHANGES.html is generated from this file. Usually pod is used for formatting.)

----------

## tag-0.7.6

Sat Mar 26 08:00:36 PDT 2022

* Removed -C from rsync copy of raw/ files

* INT: Cleaned up formatting

* Added links to docs

* INT: Describe possible designs for remore raw methods. Issue #18

* INT: Grammerly checks run against all doc and usage text

* INT: Fixed up product deps. Fixed up docs a bit.

* Added Quick Start

* INT: Fixed download path for tar files.

* INT: Remove symlinks to tar files. With gpTestEnv and mTestEnv, specify the directory for the tar files.

* INT: Added markdown package

* Added index files to docs

* INT: Cleaned up the doc files so they are all generated consistently.

* Updated overview image

* Added UserDFD image and link to docs

----------

## tag-0.7.3

Thu 03 Mar 2022 11:23:08 AM PST

* Removed vars: bin and doc. Fixed symlinks, for moved files, to be relative. See issue #51

* Make sure git config var name are always lower case. Issue #2

* Updated the documentation with more examples. See  issue #30

* INT: Regenerated the test-env tar files, and repaired tests

* INT: Increased sleep time, so the "test Action" will work

* INT: Fixed tests that use upper-case in key names. Issue #2

* INT: Adding more tests for git-proj-config. Issue #15

* INT: Fixed pre-commit check for binary files.

----------

## tag-0.7.1

Wed 23 Feb 2022 10:49:07 PM PST

* Updated docs, fixed typos

* Implemented git-proj-config. It is mostly ready, but still rough in spots. Issue #15

* Documented the menus in git-proj-config. Issue #15

* INT: Added git-proj-config to git-proj help list. Issue #15

* INT: Wrote fComMenu. It is more general than fComSelect.

* INT: Added some tests for git-proj-config. More are needed. See Issue #31

* INT: Fixed pre-commit check for binary files. Grep changed.

* INT: Fixed up fComConfigCopy, so that the "to" file doesn't need to exist.

* INT: Updated stats.sh. It is still not very accurate.

* INT: Add update of doc/VERSION to be in "first" target for package/Makefile

* INT: documented the "epm" dependency for development. I.e. where to get it.

* INT: Regenerated the tar files, and repaired the tests.

* INT: Remove try/catch functions. They don't work well enough to use.

* INT: Fixed the dev-doc generation.

----------

## tag-0-5-7-1

Wed 09 Feb 2022 07:37:27 PM PST

* INT: Created fix-rel-links.sh and fixed LINK{} tags. Issue: #5

* Implement the pre-commit gitproj.hook.check-for-tabs. See issue #4

* INT: The code has been converted to use the fComYesNo function. Issue #9

* Get pre-commit from more than one place. See "config" document, and issue #16

* INT: Added fComMkPreCommit to init and clone. Implements issue #16

* INT: Updated docs. Reorganized build steps in package.

* INT: Moved scripts to test/utils. Uses clean-git-proj-repo.sh and clean-git-proj-repo-cron on release server.

* Push test packages to moria. See: [/rel/development/software/own/git-proj/deb/](https://moria.whyayh.com/rel/development/software/own/git-proj/deb/)

* INT: Document the release-server setup. Added rush documentation.

* Added "ff-only" to "git proj pull"

* INT: Implemented GitHub Actions: test, package. See Issue #27

    * INT: Get epm package that does not include mkver.pl. Issue #27
    * INT: Fixed released path. Issue #27
    * INT: Changed package.yml so it only runs if test succeeds. Issue #27
    * INT: Added sudo to apt-get. Issue #27
    * INT: Added epm and epm-helper requirements. Issue #27
    * INT: Simplified install-deps
    * INT: Added github actions badge

----------

## tag-0-5-5-2

Thu 03 Feb 2022 11:42:47 AM PST

* Updated docs

* Added 'why' section to README. Spell corrections.

* Updated most github links, in docs, to point to the "main" branch.

* INT: Renamed release VERSION file to REL-VERSION

* INT: Issue #23 fixed in version 0.5.5

* INT: Got Travis-CI working

* INT: Fixed tar file deps for tests

* INT: Skipped syslog tests on Travis-CI server

----------

## tag-0-5-4-1

Mon 31 Jan 2022 09:56:47 AM PST

* Updated docs

* Created allcreate_a_git-proj_repo tutorial

* Added raw/.remote.proj

* Changed default remote-min-space to 2g

* Renamed allow-tabs to check-for-tabs

* Changed: binary-file-size-limit to binary-file-size

* Added rm-trailing-sp, bash-fmt, and bash-lint to
      /usr/share/doc/git-proj/contrib/

* Create CODE_OF_CONDUCT.md

* Created issue template: Bug Report

* Created issue template: Feature Request

* Added "requires" to package. See README "System requirements"
    section.

* Changed syslog default to `false`

* INT: Spell checked docs

* INT: Cleaned up mount-dir issues

* INT: Fixed POD error

* INT: Passed pRemoteGitDir. It was not validating enough.

* INT: Changed coding TBD to TODO, so it does not conflict with the
  TBD in variables.

* INT: Created: test/dev-doc/coding-convention.pod

* INT: Created: test/dev-doc/outline.pod

* INT: Moved TODO.md to test/dev-doc/

* INT: Created directory design docs: test/dev-doc/enhancements/

* INT: Added archive target

* INT: Cleaned up output and prompt/response from 'git proj remote'

* INT: Renamed gpRemoteRawDir to gpRemoteRawOrigin

* INT: Renamed remote-raw-dir to remote-raw-origin

* INT: Refactored all of the gitproj config files. Mainly removed the
need for HOST config files.

* INT: Fixed up the handling of fLog, fError, gpVerbose, and common
  options

* INT: Created: Function Map in outline.pod

* INT: Removed unused var cCurDir

* INT: Added pull request template

* INT: Moved LICENSE file.

* INT: Patch VERSION in config files, for test setup..

* INT: Make sure first option is not a flag, in git-pro

----------

## tag-0-2-0-1

Tue Jan 11 12:03:36 2022 -0800

* Updated documentation

* Fixed git-proj so it calls sub-commands.

* Formatting changes and spell checked.

* Bold markdown does nothing in github, so use back-ticks to highlight

* Added CHANGES.md.

* Updated README.md.

* Updated documentation, and added test/dev-doc

* Cleaned up the gen-doc. Normalized with tidy

* INT: added test/dev-doc

* INT: Updated package Makefile doc

* INT: docs are updated with 'make'

* INT: Fixed generation of doc/user-doc files

----------

## 0.1.2-2

Mon Jan 10 17:12:39 2022 -0800

* Cleaned up the gen-doc.

* INT: Normalized gen-doc with tidy

* INT: Updated the tests

----------

## 0.1.1-2

Mon Jan 10 15:02:39 2022 -0800

* First package.
