#!/bin/bash
# vim: sw=4 et

SUBDIRS="kfiledialog kio_sysinfo krpmview SUSEgreeter suseplugger kde4-openSUSE"

rm -f CMakeLists.txt
echo "find_package(Gettext REQUIRED)" >> CMakeLists.txt

LANGS=`cd $MY_LCN_CHECKOUT && ls -1d ?? ??_??`
echo $LANGS
for lang in $LANGS; do
    PO_LIST=""
    for i in $SUBDIRS; do
        echo -n "copying $lang/po/$i.$lang.po... "
        if test -f  "$MY_LCN_CHECKOUT/$lang/po/$i.$lang.po"; then
            mkdir -p $lang
            cp -p $MY_LCN_CHECKOUT/$lang/po/$i.$lang.po $lang/$i.po
            PO_LIST="$PO_LIST $lang/$i.po"
            echo "done"
        else
            echo "missing"
        fi
    done
    echo "gettext_process_po_files($lang ALL INSTALL_DESTINATION \${LOCALE_INSTALL_DIR} $PO_LIST )" >> CMakeLists.txt
done
