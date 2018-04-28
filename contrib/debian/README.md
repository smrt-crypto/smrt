
Debian
====================
This directory contains files used to package smrtd/smrt-qt
for Debian-based Linux systems. If you compile smrtd/smrt-qt yourself, there are some useful files here.

## smrt: URI support ##


smrt-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install smrt-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your smrtqt binary to `/usr/bin`
and the `../../share/pixmaps/smrt128.png` to `/usr/share/pixmaps`

smrt-qt.protocol (KDE)

