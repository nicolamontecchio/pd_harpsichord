mac :
	# cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o stoptrigger.pd_darwin stoptrigger.c
	# cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o mraagpio.pd_darwin mraagpio.c
	# cc -o listdevices -lportaudio -lportmidi -O2 listdevices.c
	# cc -o standalone -lportaudio -lportmidi -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd standalone.c
	cc -o testpatch -D EDISON -O3 -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd testpatch.c


edison :
	cc -std=c99 -march=native -O3 -shared -undefined=dynamic_lookup -o stoptrigger.so stoptrigger.c
	cc -std=c99 -march=native -O3 -shared -undefined=dynamic_lookup -D EDISON -lmraa -o mraagpio.so mraagpio.c
	cc -D EDISON -lportaudio -lasound -o listdevices -O2 listdevices.c
	cc -D EDISON -o standalone -lportaudio -lasound -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd standalone.c
