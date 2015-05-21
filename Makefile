mac :
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o stoptrigger.pd_darwin stoptrigger.c
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o mraagpio.pd_darwin mraagpio.c
	cc -o listdevices -lportaudio -lportmidi -O2 listdevices.c
	cc -o standalone -lportaudio -lportmidi -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd standalone.c



edison :
	cc -D EDISON -lportaudio -lasound -o listdevices -O2 listdevices.c
	cc -std=c99  -o testpatch -D EDISON -O3 -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd testpatch.c
	cc -c -std=c99 -march=native -O3 -o stoptrigger.o stoptrigger.c
	cc -c -std=c99 -march=native -O3 -D EDISON -o mraagpio.o mraagpio.c
	cc -D EDISON -o standalone -lportaudio -lasound -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd -lmraa standalone.c mraagpio.o stoptrigger.o
