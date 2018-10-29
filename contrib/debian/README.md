
Debian
====================
This directory contains files used to package aibd/aib-qt
for Debian-based Linux systems. If you compile aibd/aib-qt yourself, there are some useful files here.

## aib: URI support ##


aib-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install aib-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your aib-qt binary to `/usr/bin`
and the `../../share/pixmaps/aib128.png` to `/usr/share/pixmaps`

aib-qt.protocol (KDE)

