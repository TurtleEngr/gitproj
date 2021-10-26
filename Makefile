
sh-files = \
	git-proj \
	git-proj-add \
	git-proj-clone \
	gitproj-com.inc \
	gitproj-com.test \
	git-proj-init \
	git-proj-pull \
	git-proj-push \
	git-proj-set \
	git-proj-status

fmt :
	which shfmt
	for i in $(sh-files); do \
		echo $$i; \
		'shfmt' -i 4 -ci -fn - <$$i >$$i.tmp; \
		mv -f $$i.tmp $$i; \
		chmod a+rx $$i; \
	done
