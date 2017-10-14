# run this from docker using the cross-compiling image thing:
# docker run -it --rm -v `pwd`:/shared dockcross/linux-armv7 bash

cd /shared

# build alsa
mkdir alsalibs
cd alsalibs

wget ftp://ftp.alsa-project.org/pub/lib/alsa-lib-1.1.4.1.tar.bz2
tar -jxvf alsa-lib-1.1.4.1.tar.bz2
cd alsa-lib-1.1.4.1

echo "building ALSA"
./configure
make -j4

echo "building the listdevices script"
$CC -o listdevices -Iarmlibs/alsa-lib-1.1.4.1/include -lm -lpthread -ldl  listdevices.c armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a

echo "scp-ing stuff into chip"
scp listdevices chip@chip.local:listdevices

## TODO LATER
# alsa tools
# ftp://ftp.alsa-project.org/pub/tools/alsa-tools-1.1.3.tar.bz2
