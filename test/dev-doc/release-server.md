# Release Server Configuration

I want the package Action to save test packages to my server. Usually
I'll use ssh-agent to setup ssh keys for pushing files, then use scp or
rsync. But setting up ssh-agent on a github action server would be
really tricky, and require "secrets". So I chose to use a passwordless
ssh key that is is only in one user's authorized\_keys file, and that
user can only use the scp command, and the command will only work with
specific paths. "GNU rush" satisfies these requirements.

## GNU Rush Setup

Once setup, scp can also replace "wget" for getting the tgz tar files.
That will be more direct and probably faster.

### Create User on Release Server

- Create user "hamp"

        adduser --shell=/usr/sbin/rush hamp

- Add hamp to "users" group in /etc/group
- Add hamp to "AllowUsers" in /etc/ssh/sshd\_config
- Setup /home/hamp/.ssh with the passwordless ssh key.

        sudo -s
        mkdir /home/hamp/.ssh
        cd /home/hamp/.ssh
        ssh-keygen -t rsa -f id.hamp
        cat id.hamp.pub >authorized_keys
        chmod -R go= .
        chown -R hamp:hamp .

### Configure rush

- Install rush: apt-get install rush
- Put test/util/rush.rc in /etc on the server

### Cron Job Cleanup Script

- Copy test/util/clean-git-proj-repo.sh to /usr/local/bin
- Copy test/util/clean-git-proj-repo-cron /etc/cron.d/ setting
the number of files to keep in the release directory.

## Configuration in the repo

- Create a place for id.hamp key and ssh config: mkdir test/util/ssh
- See: test/util/ssh/config file
- Create a Makefile target "mk-ssh" (in test/Makefile) for
creating the ~/.ssh dir

        mk-ssh : util/ssh/config util/ssh/id.hamp
                -mkdir ~/.ssh
                cp -f util/ssh/id.hamp  ~/.ssh
                touch ~/.ssh/config; \
                if ! grep 'Host moria.whyayh.com' ~/.ssh/config; then \
                    cat util/ssh/config >>~/.ssh/config; \
                fi
                chmod -R go= ~/.ssh

- Add commands like this to "release" targets in a Makefile

        mServer = moria.whyayh.com
        mRelPath = /software/own/git-proj/deb

        release : ...
                cd ../test; make mk-ssh
                scp pkg/git-proj*.deb hamp@$(mServer):$(mPath)
                scp pkg/git-proj*.tar.gz hamp@$(mServer):$(mPath)

    Note: the "git-proj/deb/" part of the path will need to be manually created.

## Example Tests and Use

Put the id.hamp key in your ~/.ssh directory. Added the
"test/ssh/config file" text to your ~/.ssh/config file, before any
last "Host \*" rules.

over 4

- Manual tests that should work

        scp -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software/own/foo.txt
        scp -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software/own/git-proj/deb/foo.txt
        scp -i ~/.ssh/id.hamp hamp@moria.whyayh.com:/software/own/foo.txt .
        scp -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software/ThirdParty

- Manual tests that should fail

    Paths must begin with "/software/" followed by "own" or "ThirdParty".

        scp -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/rel/develop/software/own
        scp -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software

    Only two arguments can be after "scp"

        scp -i ~/.ssh/id.hamp test.txt foo.txt hamp@moria.whyayh.com:/software/own

    Only scp can be run.

        rsync test.txt hamp@moria.whyayh.com:/software/own

        ssh -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com ls /software

- Makefile test

        make build
        make package
        make release

# POD ERRORS

Hey! **The above document had some coding errors, which are explained below:**

- Around line 105:

    '=item' outside of any '=over'
