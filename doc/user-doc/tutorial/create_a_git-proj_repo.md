# Create a git-proj Repo

# Download and install git-proj.

    tUrl=https://moria.whyayh.com/rel/released/software/own/git-proj/deb
    tPkg=git-proj-0.4.1-1-mx-x86_64.deb
    tOpt="--user=guest --password=guest"
    wget $tOpt $tUrl/$tPkg

    sudo apt-get install ./$tPkg

# Runnig git proj commands

- Set up a project directory with some dummy files

        mkdir -p tmp/project/hello-world
        cd tmp/project/hello-world
        mkdir src doc
        touch src/prog1.sh doc/prog1.txt

        # Look at these files:
        if ls ~/.gitconfig; then more ~/.gitconfig; fi
        if ls ~/.gitignore; then more ~/.gitignore; fi
        tree -aF $PWD

- Run \`git proj init\`

        # You are in directory: tmp/project/hello-world

        git proj init -l $PWD

        # Look at these files:
        ls ~/.gitconfig ~/.gitconfig.bak ~/.gitignore
        more~/.gitconfig
        tree -aF $PWD

    If you didn't have a ~/.gitconfig file, you should edit it an update your
    user name and email.

    If you have a ~/.gitconfig file, it was backed up to ~/.gitconfig.bak

    In ~/.gitconfig the important sections are

        `[gitflow "branch"]`
        `[gitflow "prefix"]`
        `[gitproj "config"]`
        `[gitproj "hook"]``

    In the hello-world dir you have some new dirs and files. These are the
    interesting ones

        hello-world/
            .gitproj
            .gitignore
            raw/
                README.txt
            .git/
                config
                hooks/
                    pre-commit

    If you look in .gitproj it will have a copy of gitflow and getproj
    sections from ~/.gitconfig

    The raw/ directory is where you will put large binary files. If
    hello-world already had some large binary files then, before the git
    repo is created, those files would have been moved into raw/ and
    symlinks created to point to the files.

- Try some git commands

        git status
        git log
        touch foo.txt
        git add foo.txt
        git ci -am "Added foo.txt"
        git status -s --ignored

- Try \`git proj status\`

        git proj stats

    This outputs an error and the short usage help. All error and other
    log messages have these parts:

        `Cmd that ran:` git-proj-status
           `Log level:` crit: Error:
             `Message:` Unexpected: gitproj.config.remote-raw-origin
                      should not be set to: TBD
            `Location:` [gitproj-com.inc:1545](1)

    This is telling us a remote raw origin has not be defined. So let's
    define one.

- First we need to create a directory for the remote. We could mount
an external disk and create the directory there. But for this tutorial
we will create it under the tmp directory.

        cd tmp/project/hello-world
        cd ../..
        mkdir -p mounted-drive/repo

- Now we can run \`git proj remote\`

        cd tmp/project/hello-world
        git proj remote -d ../../mounted-drive/repo

        1) QUIT                      3) OTHER
        2) HELP                      4) ../../mounted-drive/repo
        Select by number, the location for the remote git and raw files? 4

        Use: ../../mounted-drive/repo [y/n]? y

    And a lot more output. A lot of it would be suppressed if the -q
    option is added.

    Scroll back and you can see what was done. Here are the important parts:

        git clone to ../../mounted-drive/repo
        Cloning into bare repository 'hello-world.git'

        rsync' -azC  .../tmp/project/hello-world/raw/
                     ../../mounted-drive/repo/hello-world.raw

        Running pre-commit
        [develop 755b166] git proj remote has been setup
        1 file changed, 2 insertions(+), 2 deletions(-)
        # the above a commit of a changed: .gitproj file

        # Now merge the changes to the `main` branch
        Switched to branch 'main'

        git remote origin is now: ../../mounted-drive/repo/hello-world.git
        raw remote origin is now: ../../mounted-drive/repo/hello-world.raw

- Now let's try \`git proj status\` again

        git proj status

    You should see:

        On branch develop
        Your branch is up to date with 'origin/develop'.

        nothing to commit, working tree clean

        REMOTE/ = ../../mounted-drive/repo/hello-world.raw/
        No differences.

- Let's put a file in raw/. It doesn't have to be a binary file and it
can be any size. Mainly the raw/ area is just "synced" to the
remote-raw-origin dir.

        cd tmp/project/hello-world
        echo "New file for raw" >raw/testing-raw
        echo "Another file" >raw/another-file.txt

        git proj status

    Now you should see:

        REMOTE/ = ../../mounted-drive/repo/hello-world.raw/
        Only in raw/: another-file.txt
        Only in raw/: testing-raw

- To "save" the raw/ files to remote-raw we run \`git proj push\`

        git proj push

    Now you'll see a diff summary and a "rsync" dry run, showing what
    would be done. And the question:

        Are the above differences OK [y/n]?

    If it looks OK, type 'y'

- New let's change a file in raw/ and remove a file, then push.

        rm raw/testing-raw
        echo "Add a line" >>raw/another-file.txt

        git proj push

    Now you'll see something like:

        raw/ push
        diff summary:
        Files tmp/project/hello-world/raw/another-file.txt and ../../mounted-drive/repo/hello-world.raw/another-file.txt differ
        Only in ../../mounted-drive/repo/hello-world.raw: testing-raw

    Enter 'y' to the question and do another status

        git proj status

        REMOTE/ = ../../mounted-drive/repo/hello-world.raw/
        Only in REMOTE/: testing-raw

    Huh? But we deleted raw/testing-raw. Let's check help for proj push.

        git proj push -h

    Oh we need the '-d' option to allow deletes.

        git proj push -d

        git proj status

    Now we see:

        REMOTE/a = ../../mounted-drive/repo/hello-world.raw/
        No differences.

    When you look at the proj push help, you might have noticed this
    command can also run "git push origin develop" if you also give the
    '-g' option.

- What about pull? If raw files in the remote are changed, added, or
deleted, then we could do a pull. Since we have access to the remote-raw
dir we an make a manual change.

        cd ../../mounted-drive/repo/hello-world.raw/
        echo "Simulate a change" >extra-file.txt
        echo "Add another line" >>another-file.txt
        rm README.txt

- Now for the pull. It also has a '-d' option.

        cd tmp/project/hello-world
        git proj pull -d

    Now you'll see something like:

        raw/ pull
        diff summary:
        Files ../../mounted-drive/repo/hello-world.raw/another-file.txt and
              tmp/project/hello-world/raw/another-file.txt differ
        Only in ../../mounted-drive/repo/hello-world.raw: extra-file.txt
        Only in tmp/project/hello-world/raw: README.txt

    Type 'y' and do another status:

        git proj status

        REMOTE/ = ../../mounted-drive/repo/hello-world.raw/
        No differences.

- That was a simulation of a remote-raw change. How about a
typical scenario? The disk was unmounted, moved to another computer.
For other computer to access remote with git and git-proj commands,
\`git proj clone\` will need to be run to setup the git workspace.
- We'll simulate this some by cloning into another place under the tmp
dir. We'll create a place where bob puts his project files.

        cd tmp/project/hello-world
        cd ../..
        mkdir -p bob/ver/proj

- Run \`git proj clone\`

        cd bob/ver/proj
        git proj clone -h

        git proj clone -d ../../../mounted-drive/repo/hello-world.git

    You should see:

        Be sure you are "cd" to the directory that the project will be cloned to.
        Clone git from: ../../../mounted-drive/repo/hello-world.git
        Clone raw from: ../../../mounted-drive/repo/hello-world.raw
        Project Name:   hello-world
        Project Dir:    /home/bruce/test/tmp/bob/ver/proj/hello-world

        Continue [y/n]?

    Type 'y' and you'll see another block of text describing what has been
    done so far.

        git-proj-clone err: Error: You must be in a git workspace for this command. [gitproj-com.inc:1800](1)
        git-proj-clone err: Error: You must be in a git workspace for this command. [gitproj-com.inc:1800](1)

    You are being asked this question because some changes could have been
    made to the .gitproj file. If you say 'n' here then the remote git
    won't be changed.

        Continue (commit the changes) [y/n]? y

- Now you can do a \`git proj status\`

           cd hello-world
           git proj status

        Defect:
        git-proj-status warning: ../../mounted-drive/repo/hello-world.raw was not found. Try again after mounting it or run 'git proj remote' to change the remote.raw.dir location. [gitproj-com.inc:1941]

         remote-raw-origin = ../../mounted-drive/repo/hello-world.raw

        The path is wrong. It should be:
            ../../../../mounted-drive/repo/hello-world.raw/
        or better, an absolute path!
            /home/bruce/test/tmp/mounted-drive/repo/hello-world.raw

    &#x3d;=back

# POD ERRORS

Hey! **The above document had some coding errors, which are explained below:**

- Around line 16:

    &#x3d;over without closing =back
