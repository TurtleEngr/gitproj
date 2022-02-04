# GNU Rush Configuration

I want the package Action to save test packages to my server. Usually
I'll use ssh-agent to setup ssh keys for pushing files, then use scp or
rsync. But setting up ssh-agent on a github action server would be
really tricky, and require "secrets". So I chose to use a passwordless
ssh key that is is only in one user's authoried\_keys file, and that
user can only use the scp command, and the command will only work with
specific paths. "GNU rush" satisfies these requirements.

Once setup, scp can also replace "wget" for getting the tgz tar files.
That will be more direct and probably faster.

## Server Setup

- Create user "hamp"

        adduser --shell=/usr/sbin/rush hamp

- Add hamp to "AllowUsers" in /etc/ssh/sshd\_config
- Add hamp to "users" group in /etc/group
- Setup /home/hamp/.ssh with the passwordless ssh key.

        sudo -s
        mkdir /home/hamp/.ssh
        cd /home/hamp/.ssh
        ssh-keygen -t rsa -f id.hamp
        cat id.hamp.pub >authorized_keys
        chmod -R go= .
        chown -R hamp:hamp .

- This is the /etc/rush.rc configuration file:

        # Set verbosity level.
        debug 1

        # Default settings
        rule default
          acct on
          limits t5
          umask 002
          env - USER LOGNAME HOME PATH
          fall-through

        # Scp requests: limited

        rule scp-to
          command ^scp (-v )?-t( --)? /software/(own|ThirdParty)/?
          # User: hamp
          uid = 1002
          set[0] /usr/bin/scp
          match[$] ! /\.\.
          match[$] ! ^~/.*
          transform[$] s,^/software/,/rel/development/software/,

        rule scp-from
          command ^scp (-v )?-f( --)? /software/(own|ThirdParty)/?
          uid = 1002
          set[0] /usr/bin/scp
          match[$] ! /\.\.
          match[$] ! ^~/.*
          transform[$] s,^/software/,/rel/development/software/,

        fall-through

        rule gen
          exit You messed up!

## In this git repo

- Create a place for id.hamp key and ssh config at: test/ssh/
- test/ssh/config file

        Host moria.whyayh.com
            CheckHostIP no
            Compression yes
            ForwardAgent no
            ForwardX11 no
            IdentitiesOnly=yes
            IdentityFile ~/.ssh/id.hamp
            NoHostAuthenticationForLocalhost no
            Protocol 2,1
            ServerAliveInterval 60
            StrictHostKeyChecking no
            User hamp

- Create a Makefile target "mk-ssh" for creating the ~/.ssh dir

        -mkdir ~/.ssh
        cp -f ssh/id.hamp  ~/.ssh
        touch ~/.ssh/config; \
        if ! grep 'Host moria.whyayh.com' ~/.ssh/config; then \
            cat ssh/config >>~/.ssh/config; \
        fi
        chmod -R go= ~/.ssh

## Example Use

- Example commands

        spc -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software/own/foo.txt
        spc -i ~/.ssh/id.hamp hamp@moria.whyayh.com:/software/own/foo.txt .
        spc -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software/ThirdParty

- These commands will fail:

        spc -i ~/.ssh/id.hamp test.txt hamp@moria.whyayh.com:/software
        rsync test.txt hamp@moria.whyayh.com:/software/own
