#!/bin/sh
# This is a comment!
make deletempq
cp -rf /mnt/d/games/sirus/World\ of\ Warcraft\ Sirus/Data/ruRU/patch-ruRU-4.mpq /mnt/c/Users/fxpw/source/repos/fxpw/Falaffell\ Helper/originalMPQ
make
./build/apps/FH
cp -rf /mnt/c/Users/fxpw/source/repos/fxpw/Falaffell\ Helper/newMPQ/patch-ruRU-x.mpq /mnt/d/games/sirus/World\ of\ Warcraft\ Sirus/Data/ruRU/
# cp -rf /mnt/c/Users/fxpw/source/repos/fxpw/Falaffell\ Helper/newMPQ/patch-ruRU-x.mpq /mnt/d/programms/repos/patch-ruRU-x/
# cd /mnt/d/programms/repos/patch-ruRU-x/
# git commit -a -m "Updated: `date +'%Y-%m-%d %H:%M:%S'`"
# git push
