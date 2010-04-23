#!/bin/bash
# vim: sw=4 et

debug=0
set -e

unset CDPATH

SUSE_VERSION=1130
export EXTRACTRC=/usr/bin/extractrc
export XGETTEXT=/usr/bin/xgettext
EXTRACT_MESSAGES=$HOME/cvs/KDE/l10n-kde4/scripts/extract-messages.sh

test -f "$EXTRACTRC" || { echo "need $EXTRACTRC"; exit 1; }
test -f "$XGETTEXT" || { echo "need $XGETTEXT"; exit 1; }
test -f "$EXTRACT_MESSAGES" || { echo "need $EXTRACT_MESSAGES"; exit 1; }

# call this script from totranslate

extract_messages()
{
    test -n "$1" || { echo "need arg!"; exit 1; }

    export podir=$PWD/po
    mkdir $podir
    mkdir $podir/../kdelibs/

    $EXTRACT_MESSAGES

    shopt -q nullglob || resetnullglob=1
    shopt -s nullglob
    msgcat $podir/*.pot > $1
    test -z "$resetnullglob" || shopt -u nullglob

    test "$debug" = 0 && rm -rf $podir

    unset podir
}

pwd=$PWD

cd $pwd

spec=$1
test -f "$spec" || { echo "need full path to .spec as argument."; exit 1; }
echo "evaluating $spec"

rm -rf translation
mkdir -p translation/orig
mkdir -p translation/patched
source=`dirname $spec`
cp -a $source translation/source
source=$(cd translation/source && pwd)
spec=$source/`basename $spec`

export RPM_BUILD_DIR=$pwd/translation/orig
rpmbuild --eval "%define _builddir $pwd/translation/orig" -bp --nodeps --eval "%define _sourcedir $source" --eval "%define suse_version $SUSE_VERSION" --eval "%define __patch echo " $spec > $pwd/translation/orig.log 2>&1
export RPM_BUILD_DIR=$pwd/translation/patched
rpmbuild --eval "%define _builddir $pwd/translation/patched" -bp --nodeps --eval "%define _sourcedir $source" --eval "%define suse_version $SUSE_VERSION" $spec > $pwd/translation/patched.log 2>&1

# now see which files have changed

changedfiles="$(LC_ALL=C diff -Nurq translation/orig translation/patched | awk '{print $4}' | cut -d'/' -f3- )"

touch translation/orig/rc.cpp
touch translation/patched/rc.cpp
echo rc.cpp > translation/orig/changed_files
echo rc.cpp > translation/patched/changed_files
for file in $changedfiles ; do
    case $file in
        *.ui|*.rc)
            if test -f translation/orig/$file; then
                $EXTRACTRC translation/orig/$file | grep -v 'i18n("");' >> translation/orig/rc.cpp
            fi
            if test -f translation/patched/$file -a -s translation/patched/$file; then
                $EXTRACTRC translation/patched/$file | grep -v 'i18n("");' >> translation/patched/rc.cpp
            fi
        ;;
        *.cpp|*.cc|*.hh|*.h)
            if test -f translation/orig/$file; then
                echo $file >> translation/orig/changed_files
            fi
            if test -f translation/patched/$file; then
                echo $file >> translation/patched/changed_files
            fi
        ;;
    esac
done
cd $pwd/translation/orig
extract_messages ../orig.po
#/usr/bin/xgettext --force-po --from-code=UTF-8 -C  -ci18n -ki18n -ktr -ktr2i18n -kI18N_NOOP -kI18N_NOOP2 -kaliasLocale -o ../orig.po `sort -u changed_files`
sed -i -e 's,Content-Type: text/plain\; charset=CHARSET,Content-Type: text/plain; charset=UTF-8,' ../orig.po

cd $pwd/translation/patched
extract_messages ../patched.po
#/usr/bin/xgettext --force-po --from-code=UTF-8 -C  -ci18n -ki18n -ktr -ktr2i18n -kI18N_NOOP -kI18N_NOOP2 -kaliasLocale -o ../patched.po `sort -u changed_files`
sed -i -e 's,Content-Type: text/plain\; charset=CHARSET,Content-Type: text/plain; charset=UTF-8,' ../patched.po

cd $pwd/translation
msgcomm --force-po -o diff.po -u orig.po patched.po
potfile=$pwd/`basename $spec`.po
msgcomm --omit-header --no-wrap --more-than=1 diff.po patched.po > $potfile
if ! test -s $potfile; then
	rm $potfile
	echo "no strings"
else
      msgfmt --statistics $potfile
fi

test "$debug" = 0 && rm -rf $pwd/translation
exit 0
