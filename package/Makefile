
# TODO
# pre and post install scripts?  Check for expected dirs.

build : mkdist mkdocs
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
mkdist ../dist :
	-rm -rf ../dist
	./install-proj.sh -r ../dist

mkdocs : ../dist
