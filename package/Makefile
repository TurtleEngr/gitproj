
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
	mkdir -p ../dist/usr/share/man/man1
	../git-core/git-proj -H man >../dist/usr/share/man/man1/gitproj.1
	gzip ../dist/usr/share/man/man1/gitproj.1
	../git-core/git-proj -H text >../dist/usr/share/doc/git-proj/gitproj.html
