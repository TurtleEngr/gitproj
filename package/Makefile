# package/Makefile
# First: cd package
# make check	# verify dependencies
# make prepare1	# create make.env
# . ./make.env
# make build	# repeat if errors
# make install	# if you want to install from dist/ dir
# make package	# make the install package(s)
# make release 	# upload the package(s) to distribution location(s)
# make clean

# --------------------
# Config

mTestDir=$(PWD)/../test
mBinDir=$(PWD)/../git-core
mDocDir=$(PWD)/../doc
mGenDir=$(PWD)/../generate
mDistDir=$(PWD)/../dist

# TODO
# Create pre and post install scripts? i.e. Check for expected dirs.

# --------------------
check : $(mTestDir) $(mBinDir) $(mDocDir) $(mGenDir) $(mDistDir)
	which epm
	which mkepmlist
	which patch-epm-list
	which mkver.pl

clean : check
	-find $(mGenDir) $(mDistDir) -type l -exec rm {} \;
	rm -rf $(mGenDir) $(mDistDir)

dist-clean : clean
	rm ver.env ver.epm epm-list.txt

build : clean $(mDistDir)/usr/lib/git-core $(mDistDir)/usr/share/doc/git-proj $(mDistDir)/usr/share/man/man1
	find $(mDistDir) -type d -exec chmod a+rx {} \;
	find $(mDistDir) -type f -exec chmod a+r {} \;
	# TBD Create epm list

install : build
	if [ "$$(whoami)" != "root" ]; then exit 1; fi
	rsync -a $(mDistDir)/* /
	exit 0
	# Fixup perms?
	for tDir in \
		$(mDistDir)/usr/lib/git-core \
		$(mDistDir)/usr/share/doc/git-proj \
		$(mDistDir)/usr/share/man/man1 \
	    ; do \
		find $$tDir -exec chown root:root {} \; ; \
		find $$tDir -type d -exec chmod a+rx {} \; ; \
		find $$tDir -type f -exec chmod a+r {} \; ; \
		find $$tDir -type f -executable -exec chmod a+rx {} \; ; \
	done

uninstall :
	if [ "$$(whoami)" != "root" ]; then exit 1; fi
	rm $(mDistDir)/usr/lib/git-core/gitproj*
	rm $(mDistDir)/usr/lib/git-core/git-proj*
	rm -rf $(mDistDir)/usr/share/doc/git-proj
	rm $(mDistDir)/usr/share/man/man1/gitproj*
	rm $(mDistDir)/usr/share/man/man1/git-proj*

package :
	echo TBD
	# Create epm list
	# Create native pkg (deb)
	# Create general pkg

release :
	echo TBD

# ----------------------------------------
# Working targets

$(mGenDir) :
	mkdir $@

$(mDistDir) :
	mkdir $@

mkdocs $(mGenDir)/man/man1/gitproj.1.gz : $(mGenDir) $(mBin)/git-proj
	mkdir -p $(mGenDir)/man/man1
	$(mBin)/git-proj -H man >$(mGenDir)/man/man1/gitproj.1
	gzip $(mGenDir)/man/man1/gitproj.1
	cp $(mGenDir)/man/man1/gitproj.1.gz $(mGenDir)/man/man1/git-proj.1.gz

$(mDistDir)/usr/lib/git-core : $(mDistDir)
	mkdir -p $@
	rsync -a $(mBinDir)/* $@/

$(mDistDir)/usr/share/doc/git-proj : $(mDistDir)
	mkdir -p $@
	rsync -a $(mDocDir)/* $@/

$(mDistDir)/usr/share/man/man1 : $(mDistDir) $(mGenDir)/man/man1/gitproj.1.gz
	mkdir -p $@
	rsync $(mGenDir)/man/man1/* $@/

ver.env : ver.sh
	mkver.pl -d ver.sh -e env

# ----------------------------------------
prepare1 make.env : make.ver
	mkver.pl -d make.ver -e env
	chmod a+rx make.env
