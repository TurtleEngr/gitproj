# package/Makefile
# First: cd package

# --------------------
# Config

mTestDir=$(PWD)/../test
mBinDir=$(PWD)/../git-core
mDocDir=$(PWD)/../doc
mGenDir=$(PWD)/../generate
mDistDir=$(PWD)/../dist

# TODO
# pre and post install scripts?  Check for expected dirs.

# --------------------
check : $(mTestDir) $(mBinDir) $(mDocDir) $(mGenDir) $(mDistDir)

clean : check
	-find $(mGenDir) $(mDistDir) -type l -exec rm {} \;
	rm -rf $(mGenDir) $(mDistDir)

build : clean $(mGenDir) $(mDistDir)
	# Create epm list

install :
	if [ "$$(whoami)" != "root" ]; then exit 1; fi
	./install-proj.sh -r /

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

mkdocs $(mGenDir)/man/man1 : $(mGenDir)
	mkdir -p $(mGenDir)/man/man1
	$(mBin)/git-proj -H man >$(mGenDir)/man/man1/gitproj.1
	gzip $(mGenDir)/man/man1/gitproj.1
