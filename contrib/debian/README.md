
Debian
====================
This directory contains files used to package loonied/loonie-qt
for Debian-based Linux systems. If you compile loonied/loonie-qt yourself, there are some useful files here.

## loonie: URI support ##


loonie-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install loonie-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your loonieqt binary to `/usr/bin`
and the `../../share/pixmaps/loonie128.png` to `/usr/share/pixmaps`

loonie-qt.protocol (KDE)

