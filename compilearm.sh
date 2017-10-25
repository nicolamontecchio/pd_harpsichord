# run this from docker using the cross-compiling image thing:
# docker run -it --rm -v `pwd`:/shared dockcross/linux-armv7 bash

# was ALSA already compiled?
if [ ! -f /shared/armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a ]; then
    cd /shared
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

# was LIBPD already compiled?
if [ ! -f libpd.so ]; then
    echo "building libpd"
    cd /shared
    git clone https://github.com/libpd/libpd.git
    cd libpd
    git checkout 0.10.0
    git submodule init
    git submodule update
    make -j4
    cp libs/libpd.so ../
fi

cd /shared

echo "building the listdevices script"
$CC -O3 -o listdevices -Iarmlibs/alsa-lib-1.1.4.1/include -lm -lpthread -ldl  listdevices.c armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a

echo "building the alsamidi script"
$CC -O3 -o alsamidi -Iarmlibs/alsa-lib-1.1.4.1/include -lm -lpthread -ldl  alsamidi.c armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a


# TODO could call strip ...
echo "scp-ing stuff into chip"
# scp listdevices alsamidi chip@chip.local:
