#!/bin/sh

desktop="`xdg-user-dir DESKTOP 2>/dev/null`"
if test -z "$desktop"; then
    desktop=$HOME/Desktop
fi

#
# do we run in a prelinked system ?
#
if test -f /etc/sysconfig/prelink; then
. /etc/sysconfig/prelink
  if test "$USE_PRELINK" = "yes" ; then
     KDE_IS_PRELINKED=1
     export KDE_IS_PRELINKED
  else
     unset KDE_IS_PRELINKED
  fi
fi

# 
# workaround SaX/SUSE bug that doesn't setup a proper keyboard map
#

if [ -f /usr/share/hotkey-setup/hotkey-setup.xmodmap ]; then
  xmodmap /usr/share/hotkey-setup/hotkey-setup.xmodmap
fi

#
# Do we have a special Gtk theming for our Qt widget theme ?
#
if [ ! -e $HOME/.no-qtrc-to-gtkrc-mapping ]; then

  SUSE_VERSION="`cat /etc/SuSE-release | sed -n 's/VERSION = //p'`"

    # Defaults for openSUSE 10.3
    GTK2_SYSCONFDIR=/etc
    GTK2_DATADIR=/usr/share/themes/

  # use general gtk-qt-engine
  if [ -e "$GTK2_DATADIR/Qt/gtk-2.0/gtkrc" ] ; then
    GTK2_THEME_RC="$GTK2_DATADIR/Qt/gtk-2.0/gtkrc"
  else
    # use QtCurve engine
    if [ -e "$GTK2_DATADIR/QtCurve/gtk-2.0/gtkrc" ] ; then
       GTK2_THEME_RC="$GTK2_DATADIR/QtCurve/gtk-2.0/gtkrc"
    fi
  fi

  # GTK2
  # NOTE: ~/.gtkrc-2.0-kde is added later (in latest KDE only)
  if [ "$GTK2_RC_FILES" ]; then
    export GTK2_RC_FILES="$GTK2_RC_FILES:$GTK2_THEME_RC:$HOME/.gtkrc-2.0-qtengine:$HOME/.gtkrc-2.0"
  else
    export GTK2_RC_FILES="$GTK2_SYSCONFDIR/gtk-2.0/gtkrc:$GTK2_THEME_RC:$HOME/.gtkrc-2.0-qtengine:$HOME/.gtkrc-2.0"
  fi

fi

if [ -r /etc/sysconfig/windowmanager ]; then
  # Do the user want the SuSE theme ?
  source /etc/sysconfig/windowmanager

  # Should we really enable FAM support for KDE ?
  export USE_FAM="$KDE_USE_FAM"

  # Disable IPv6 ?
  if [ "$KDE_USE_IPV6" = "no" ]; then
     export KDE_NO_IPV6=1
  fi
  # Disable IDN ?
  if [ "$KDE_USE_IDN" = "no" ]; then
     export KDE_NO_IDN=1
  fi

else
  if [ -r /etc/rc.config ]; then
    # Do the user want the SuSE theme ?
    INSTALL_DESKTOP_EXTENSIONS=`bash -c "source /etc/rc.config && echo \\$INSTALL_DESKTOP_EXTENSIONS"`

    # Should we really enable FAM support for KDE ?
    USE_FAM=`bash -c "source /etc/rc.config && echo \\$KDE_USE_FAM"`
    export USE_FAM
  fi
fi

#
# create SuSE defaults
#
if [ ! -e "$HOME/.skel/kdebase4.120" ]; then
    mkdir -p "$desktop"

    if [ -e "/usr/bin/firefox" -a ! -e "$desktop/MozillaFirefox.desktop" -a -e "/usr/share/kde4/config/SuSE/default/MozillaFirefox.desktop" ]; then
          cp /usr/share/kde4/config/SuSE/default/MozillaFirefox.desktop "$desktop/"
    fi
    chmod u+x "$desktop/MozillaFirefox.desktop" 2>/dev/null

    if [ -e "/usr/bin/oofromtemplate" -a  ! -e "$desktop/Office.desktop" -a -e "/usr/share/kde4/config/SuSE/default/Office.desktop" ]; then
          cp /usr/share/kde4/config/SuSE/default/Office.desktop "$desktop/"
    fi
    chmod u+x "$desktop/Office.desktop" 2>/dev/null

    if [ ! -e "$desktop/SuSE.desktop" -a -e "/usr/share/kde4/config/SuSE/default/SuSE.desktop" ]; then
          cp /usr/share/kde4/config/SuSE/default/SuSE.desktop "$desktop/"
    fi
    chmod u+x "$desktop/SuSE.desktop" 2>/dev/null

    if [ ! -e "$desktop/Support.desktop" -a -e "/usr/share/kde4/config/SuSE/default/Support.desktop" ]; then
          cp /usr/share/kde4/config/SuSE/default/Support.desktop "$desktop/"
    fi
    sed -i 's/^Icon=susehelpcenter$/Icon=Support/' "$desktop/Support.desktop"
    chmod u+x "$desktop/Support.desktop" 2>/dev/null

    if [ ! -e "$desktop/myComputer.desktop" -a -e "/usr/share/kde4/config/SuSE/default/myComputer.desktop" ]; then
          cp /usr/share/kde4/config/SuSE/default/myComputer.desktop "$desktop/"
    fi
    chmod u+x "$desktop/myComputer.desktop" 2>/dev/null

    if [ ! -e $HOME/.kde4/share/config/kdeglobals -a -e /usr/share/kde4/config/SuSE/default/kdeglobals ]; then
          mkdir -p $HOME/.kde4/share/config
          cp /usr/share/kde4/config/SuSE/default/kdeglobals $HOME/.kde4/share/config/kdeglobals
          # add Language= and Country=
          local=`/usr/bin/locale 2>&1 | grep LC_MESSAGES | sed -e 's#^LC_MESSAGES="\(.*\)"$#\1#; s#\..*##'`
          lang=`echo $local | sed -e 's,_.*$,,'`
          countr=`echo $local | sed -e 's,^.*_,,' | tr 'A-Z' 'a-z'`
          if test -z "$countr"; then countr=$lang; fi
          cat >>$HOME/.kde4/share/config/kdeglobals <<EOF
[Locale]
Country=$countr
Language=$lang
EOF
    fi

    if [ ! -e $HOME/.kde4/share/config/kcminputrc -a -e /usr/share/kde4/config/SuSE/default/kcminputrc ]; then
          mkdir -p $HOME/.kde4/share/config
          cp /usr/share/kde4/config/SuSE/default/kcminputrc $HOME/.kde4/share/config/kcminputrc
    fi

    if [ ! -e $HOME/.kde4/share/config/nepomukserverrc -a -e /usr/share/kde4/config/SuSE/default/nepomukserverrc ]; then
          mkdir -p $HOME/.kde4/share/config
          cp /usr/share/kde4/config/SuSE/default/nepomukserverrc $HOME/.kde4/share/config/nepomukserverrc
    fi

    if [ ! -e "$HOME/.kde4/share/apps/konqueror/bookmarks.xml" -a -e "/usr/share/kde4/config/SuSE/default/bookmarks.xml" ]; then
          mkdir -p $HOME/.kde4/share/apps/konqueror
          cp /usr/share/kde4/config/SuSE/default/bookmarks.xml $HOME/.kde4/share/apps/konqueror/bookmarks.xml
    fi

    if [ ! -e "$HOME/.kde4/share/apps/akregator/data/feeds.opml" -a -e "/usr/share/kde4/config/SuSE/default/feeds.opml" ]; then
          mkdir -p $HOME/.kde4/share/apps/akregator/data
          cp /usr/share/kde4/config/SuSE/default/feeds.opml $HOME/.kde4/share/apps/akregator/data/feeds.opml
    fi

    documents="`xdg-user-dir DOCUMENTS 2>/dev/null`"
    if test -z "$documents"; then
        documents=$HOME/Documents
    fi
    mkdir -p "$documents"
    if [ ! -e "$documents/.directory" -a -e "/usr/share/kde4/config/SuSE/default/documents.directory" ]; then
          cp /usr/share/kde4/config/SuSE/default/documents.directory "$documents/.directory"
    fi

    mkdir -p $HOME/.skel/
    touch $HOME/.skel/kdebase4.120
fi

# check if any rpms have been (un)installed since ksycoca
# had been built, if yes, trigger ksycoca rebuild immediatelly
# instead of delayed

kdehome=$HOME/.kde4
test -n "$KDEHOME" && kdehome=`echo "$KDEHOME"|sed "s,^~/,$HOME/,"`
host=$HOSTNAME
test -n "$XAUTHLOCALHOSTNAME" && host=$XAUTHLOCALHOSTNAME
ksycoca="$kdehome/cache-$host/ksycoca"

if test -f "$ksycoca"; then
    if test -f /var/lib/rpm/Packages; then
	if test /var/lib/rpm/Packages -nt "$ksycoca"; then
	    rm -f "$ksycoca"
	fi
    fi
fi
