
build :
	-rm -rf ../dist
	./install-proj.sh ../dist

install :
	if [ "$$(whoami)" != "root" ]; then exit 1; fi
	./install-proj.sh /

uninstall :
	if [ "$$(whoami)" != "root" ]; then exit 1; fi
	./uninstall-proj.sh /

package :
	echo TBD

release :
	echo TBD
