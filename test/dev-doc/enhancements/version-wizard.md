TODO item:

* Version increment wizard script. Increment the
  major/minor/path/rc/build vars based on Q/A

Make a script to inc version tag "major.minor.fix-rc.N+build" parts
See: https://semver.org/ for a RegEx pattern matcher (so imp. with
Perl) This supports a proper sub-set of semver.

        sem-ver [-M] [-m] [-r] [-p] [-b] [-B] [-c] [-v] [-V] [-d pVer] FILE[:key]
            If FILE does not exist, create file with 0.1.0
            If no option, output the full version
            -M inc major, set minor, see -m (if rc, inc it, set all others to 0)
            -m inc minor, set patch to 0, see -p (if rc, inc it, set all others to 0)
            -p inc patch (if rc, inc it, clear all after +)
            -r inc the release number, if none, inc patch, then insert
               "-rc.1" after "patch", clear all after +
            -b inc build, if none, append "+1"
            -B datestamp build, clear all after + then add:
               date ++%Y.%m.%d.%H.%M
               +2021.12.31.15.06
            -c clear release and build parts (do this after a "release")
            -v output with no build part
            -V output just the major.minor.patch parts
            -d "ver" - only compare major, minor, and patch parts. Ignore the rest.
               Difference compare with expected "ver"
                -3 if ver < FILE if Major part is <
                -2 if ver < FILE if minor part is <
                -1 if ver < FILE if patch part is <
                 0 if ver = FILE
                 1 if ver > FILE if patch part is >
                 2 if ver > FILE if minor part is >
                 3 if ver > FILE if Major part is >

Rather than FILE, support reporting and updating the version number in
a git config variable. That means the git config file and variable key
needs to be defined. Use: FILE:KEY For example:
.gitproj.config.local:gitproj.config.proj-ver

Rather than FILE or git config var, update the variables in ver.sh

Note: epm uses "-BUILD" not "+BUILD". The mkver.pl ver.sh process
supports the "-rc.N.N" part (but the build # would not be added after
that.  The mkver.pl creates a -BuildTime (-test.YYYY.MM.DD.hh.mm),
which is useful for CI/CD builds.
