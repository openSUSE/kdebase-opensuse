#!/bin/bash
# vim: sw=4 et

set -e

for s in $(<spec_list); do

    if ! test -f /work/SRC/all/KDE/$s ; then
        echo "$s does not exist.. "
        exit 1
    fi
    ./extract_strings.sh /work/SRC/all/KDE/$s
done
msgcat *.po > kde4-openSUSE.pot
