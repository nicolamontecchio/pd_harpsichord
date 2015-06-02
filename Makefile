mac_standalone:
	cc -c -std=c99 -march=native -O3 -o stoptrigger.o stoptrigger.c
	cc -c -std=c99 -march=native -O3 -o mraagpio.o mraagpio.c
	cc -o listdevices -lportaudio -lportmidi -O2 listdevices.c
	cd pd_sampleplayer; make pdexternal_static
	c++ -o standalone -lportaudio -lportmidi -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd -lsndfile standalone.c pd_sampleplayer/sampleplayer.o pd_sampleplayer/sampleplayer_c_interface.o pd_sampleplayer/sampleplayer_pd.o stoptrigger.o mraagpio.o


mac :
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o stoptrigger.pd_darwin stoptrigger.c
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o mraagpio.pd_darwin mraagpio.c
	cd pd_sampleplayer; make pdexternal; cp sampleplayer~.pd_darwin ..

edison :
	cc -D EDISON -lportaudio -lasound -o listdevices -O2 listdevices.c
	# cc -g -c -std=c99 -march=native -O3 -D EDISON -o mraagpio.o mraagpio.c
	cc -g -D EDISON -o standalone -lportaudio -lasound -Ilibpd/libpd_wrapper -Ilibpd/pure-data/src -Llibpd/libs -lpd -lsndfile -lmraa standalone.c mraagpio.c
