
SUBDIRS = kfiledialog kio_sysinfo krpmview kryptomedia SUSEgreeter suseplugger susetranslations

update:
	for i in $(SUBDIRS); do \
		test -d $$i && sed "s:%PACKAGE%:$$i:" < Makefile.am.template > $$i/Makefile.am; \
	done

fill_pos:
	LANGS=`cd $$MY_LCN_CHECKOUT && ls -1d ?? ??_??` ;\
	echo $$LANGS; \
	for i in $(SUBDIRS); do \
		for lang in $$LANGS; do \
		  echo -n "copying $$lang/po/$$i.$$lang.po... " ;\
		  if test -f  "$$MY_LCN_CHECKOUT/$$lang/po/$$i.$$lang.po"; then \
			cp $$MY_LCN_CHECKOUT/$$lang/po/$$i.$$lang.po $$i/$$lang.po ;\
			echo "done" ; \
		  else \
			echo "missing" ;\
		  fi ;\
		done ;\
	done

