# Makefile

#----------
# Config

mHtmlOpt = --cachedir=/tmp --index --backlink --podpath=.

mTidy = tidy -m -q -i -w 78 -asxhtml --break-before-br yes --indent-attributes yes --indent-spaces 2 --tidy-mark no --vertical-space no

#----------
gen-doc : config-doc tutorial-doc index-doc

#----------
config-doc : ../doc/user-doc/config.html

../doc/user-doc/config.html : config.pod
	-pod2html --title='Configuration Documentation' $(mHtmlOpt) <$? >$@
	-test/util/fix-rel-links.sh ../doc/user-doc/config.html
	-$(mTidy) ../doc/user-doc/config.html
	-pod2markdown <config.pod >../doc/user-doc/config.md
	-../test/util/fix-rel-links.sh ../doc/user-doc/config.md

#----------
tutorial-doc : ../doc/user-doc/tutorial/create_a_git-proj_repo.html

../doc/user-doc/tutorial/create_a_git-proj_repo.html : create_a_git-proj_repo.pod
	-mkdir -p ../doc/user-doc/tutorial
	-pod2html --title='Create a git-proj Repo' $(mHtmlOpt) <$? >$@
	-$(mTidy) ../doc/user-doc/config.html
	-pod2markdown create_a_git-proj_repo.pod >../doc/user-doc/tutorial/create_a_git-proj_repo.md

#----------
index-doc : ../doc/index.html

../doc/index.html : index.pod
	-pod2html --title='git-proj TOC' $(mHtmlOpt) <$? >$@
	-../test/util/fix-rel-links.sh ../doc/index.html
	-$(mTidy) ../doc/index.html
	-pod2markdown <index.pod >../doc/index.md
	-../test/util/fix-rel-links.sh ../doc/index.md
