# run this from docker using the cross-compiling image thing:
# docker run -it --rm -v `pwd`:/shared dockcross/linux-armv7 bash


cd /shared

# was ALSA already compiled? if not, do it
if [ ! -f /shared/armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a ]; then
    rm -rf armlibs
    mkdir armlibs
    echo "building ALSA"
    cd armlibs
    wget ftp://ftp.alsa-project.org/pub/lib/alsa-lib-1.1.4.1.tar.bz2
    tar -jxvf alsa-lib-1.1.4.1.tar.bz2
    cd alsa-lib-1.1.4.1
    ./configure
    make -j4
fi



echo "building the listdevices script"
cd /shared
$CC -O3 -o listdevices -Iarmlibs/alsa-lib-1.1.4.1/include -lm -lpthread -ldl  listdevices.c armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a

echo "building the alsamidi script"
$CC -O3 -o alsamidi -Iarmlibs/alsa-lib-1.1.4.1/include -lm -lpthread -ldl  alsamidi.c armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a

# TODO could call strip ...

echo "scp-ing stuff into chip"
scp listdevices alsamidi chip@chip.local:
