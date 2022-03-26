# git-proj CHANGES

[//]: # (The CHANGES.html is generated from this file. Usually pod is used for formatting.)

----------

## tag-0.7.6

Sat Mar 26 08:00:36 PDT 2022

* Removed -C from rsync copy of raw/ files

* Added links to docs

* Added Quick Start

* Added index files to docs

* Updated overview image

* Added UserDFD image and link to docs

----------

## tag-0.7.3

Thu 03 Mar 2022 11:23:08 AM PST

* Removed vars: bin and doc. Fixed symlinks, for moved files, to be relative. See issue #51

* Make sure git config var name are always lower case. Issue #2

* Updated the documentation with more examples. See  issue #30

----------

## tag-0.7.1

Wed 23 Feb 2022 10:49:07 PM PST

* Updated docs, fixed typos

* Implemented git-proj-config. It is mostly ready, but still rough in spots. Issue #15

* Documented the menus in git-proj-config. Issue #15

----------

## tag-0-5-7-1

Wed 09 Feb 2022 07:37:27 PM PST

* Implement the pre-commit gitproj.hook.check-for-tabs. See issue #4

* Get pre-commit from more than one place. See "config" document, and issue #16

* Push test packages to moria. See: [/rel/development/software/own/git-proj/deb/](https://moria.whyayh.com/rel/development/software/own/git-proj/deb/)

* Added "ff-only" to "git proj pull"

----------

## tag-0-5-5-2

Thu 03 Feb 2022 11:42:47 AM PST

* Updated docs

* Added 'why' section to README. Spell corrections.

* Updated most github links, in docs, to point to the "main" branch.

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

  TBD in variables.

need for HOST config files.

  options

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

----------

## 0.1.2-2

Mon Jan 10 17:12:39 2022 -0800

* Cleaned up the gen-doc.

----------

## 0.1.1-2

Mon Jan 10 15:02:39 2022 -0800

* First package.
