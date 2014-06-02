UNetbootin Source Revision 603
Copyright Geza Kovacs
Homepage at http://unetbootin.sourceforge.net
Licensed under the GNU GPL v2 and above, components from other projects are licensed under their respective licenses
Build generated on Mon Apr 21 18:05:08 PDT 2014

Download using git:
git clone https://github.com/gkovacs/unetbootin.git
cd unetbootin
git checkout 

Download using bzr:
bzr branch http://bazaar.launchpad.net/~vcs-imports/unetbootin/trunk -r603

Build instructions at http://sourceforge.net/apps/trac/unetbootin/wiki/compile

sed -i '/^RESOURCES/d' unetbootin.pro
lupdate-qt4 unetbootin.pro
lrelease-qt4 unetbootin.pro
qmake-qt4 "DEFINES += NOSTATIC" "RESOURCES -= unetbootin.qrc"
make
