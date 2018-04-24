
Debian
====================
This directory contains files used to package teled/tele-qt
for Debian-based Linux systems. If you compile teled/tele-qt yourself, there are some useful files here.

## tele: URI support ##


tele-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install tele-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your teleqt binary to `/usr/bin`
and the `../../share/pixmaps/tele128.png` to `/usr/share/pixmaps`

tele-qt.protocol (KDE)

