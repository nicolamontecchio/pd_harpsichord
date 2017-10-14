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


# make install  # TODO needed by portaudio?
# echo "building PORTAUDIO"
# cd /shared/armlibs
# wget http://www.portaudio.com/archives/pa_stable_v190600_20161030.tgz
# tar -zxvf pa_stable_v190600_20161030.tgz
# cd portaudio
# ./configure --without-oss --with-alsa






echo "building the listdevices script"
cd /shared
$CC -o listdevices -Iarmlibs/alsa-lib-1.1.4.1/include -lm -lpthread -ldl  listdevices.c armlibs/alsa-lib-1.1.4.1/src/.libs/libasound.a





echo "scp-ing stuff into chip"
scp listdevices chip@chip.local:listdevices

## TODO LATER
# alsa tools
# ftp://ftp.alsa-project.org/pub/tools/alsa-tools-1.1.3.tar.bz2
