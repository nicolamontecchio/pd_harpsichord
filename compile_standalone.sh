#c++ -o test1 -lportaudio -I../soundtest/libpd/libpd_wrapper -I../soundtest/libpd/pure-data/src -L../soundtest/libpd/libs -lpd  test1.cc
c++ -o standalone  -lportaudio -lportmidi -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs  -lpd  standalone.c
cc -o listdevices -lportaudio -lportmidi -O2 listdevices.c
cc -o testmidi -lportmidi -O2 testmidi.c
