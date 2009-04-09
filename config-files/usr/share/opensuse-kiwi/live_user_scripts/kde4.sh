#! /bin/sh

if test -e /usr/share/applications/YaST2/live-installer.desktop ; then
   if [ ! -e "$HOME/Desktop/live-installer.desktop" -a -e "/usr/share/kde4/config/SuSE/default/live-installer.desktop" ]; then
        cp /usr/share/kde4/config/SuSE/default/live-installer.desktop $HOME/Desktop/
   fi
fi

mkdir -p $HOME/.kde4/share/config
cp -a /usr/share/kde4/config/SuSE/default/lowspacesuse.live $HOME/.kde4/share/config/lowspacesuse
cp -a /usr/share/kde4/config/SuSE/default/nepomukserverrc.live $HOME/.kde4/share/config/nepomukserverrc
cp -a /usr/share/kde4/config/SuSE/default/kdedrc.live $HOME/.kde4/share/config/kdedrc

mkdir -p $HOME/.config/autostart
cp -a /usr/share/kde4/config/SuSE/default/kupdateapplet-autostart.desktop.live $HOME/.config/autostart/kupdateapplet-autostart.desktop
cp -a /usr/share/kde4/config/SuSE/default/beagled-autostart.desktop.live $HOME/.config/autostart/beagled-autostart.desktop

/usr/bin/kbuildsycoca4
# the cache is hostname specific, so don't hardcode "build24". at least "linux"
mv $HOME/.kde4/cache-* $HOME/.kde4/cache-linux

