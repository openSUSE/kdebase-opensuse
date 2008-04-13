#!/bin/bash
# vim: sw=4 et

debug=1
set -e

# call this script from totranslate

extract_messages()
{
    extract_path=/suse/dmueller/src/release-4.1/clean/kde-l10n/scripts/extract-messages.sh


    bash -x $extract_path
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
rpmbuild --eval "%define _builddir $pwd/translation/orig" -bp --nodeps --eval "%define _sourcedir $source" --eval "%define suse_version 1020" --eval "%define ___build_pre patch() { echo \"$@\"; } " $spec > $pwd/translation/orig.log 2>&1
export RPM_BUILD_DIR=$pwd/translation/patched
rpmbuild --eval "%define _builddir $pwd/translation/patched" -bp --nodeps --eval "%define _sourcedir $source" --eval "%define suse_version 1020" $spec > $pwd/translation/patched.log 2>&1

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
			      /opt/kde-35/bin/extractrc translation/orig/$file | grep -v 'i18n("");' >> translation/orig/rc.cpp
			fi
			if test -f translation/patched/$file; then
                            /opt/kde-35/bin/extractrc translation/patched/$file | grep -v 'i18n("");' >> translation/patched/rc.cpp
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
podir=$PWD/.. extract_messages
#/usr/bin/xgettext --force-po --from-code=UTF-8 -C  -ci18n -ki18n -ktr -ktr2i18n -kI18N_NOOP -kI18N_NOOP2 -kaliasLocale -o ../orig.po `sort -u changed_files`
sed -e 's,Content-Type: text/plain\; charset=CHARSET,Content-Type: text/plain; charset=UTF-8,' ../orig.po > t && mv t ../orig.po

cd $pwd/translation/patched
podir=$PWD/.. extract_messages
#/usr/bin/xgettext --force-po --from-code=UTF-8 -C  -ci18n -ki18n -ktr -ktr2i18n -kI18N_NOOP -kI18N_NOOP2 -kaliasLocale -o ../patched.po `sort -u changed_files`
#sed -e 's,Content-Type: text/plain\; charset=CHARSET,Content-Type: text/plain; charset=UTF-8,' ../patched.po > t && mv t ../patched.po

cd $pwd/translation
msgcomm --force-po -o diff.po -u orig.po patched.po
cppfile=$pwd/`basename $spec`.cpp
msgcomm --omit-header --no-wrap --stringtable-output --more-than=1 diff.po patched.po | sed -e 's," = ".*$,",; s,^"\(.*\)$,i18n("\1);,' | grep -v '^/\* Flag:' | grep -v '^/\* Comment:' > $cppfile
if ! test -s $cppfile; then
	rm $cppfile
	echo "no strings"
else
      num=`grep i18n $cppfile | wc -l`
      echo "$num strings: $cppfile"
fi

test "$debug" = 0 && rm -rf $pwd/translation
