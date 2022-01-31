# git-proj CHANGES

----------

## tag-0-5-4-1

Date: Mon 31 Jan 2022 09:56:47 AM PST

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

Date:   Tue Jan 11 12:03:36 2022 -0800

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

Date:   Mon Jan 10 17:12:39 2022 -0800

* Cleaned up the gen-doc.

* INT: Normalized gen-doc with tidy

* INT: Updated the tests

----------

## 0.1.1-2

Date:   Mon Jan 10 15:02:39 2022 -0800

* First package.
