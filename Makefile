mac :
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o stoptrigger.pd_darwin stoptrigger.c
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o mraagpio.pd_darwin mraagpio.c

edison :
	cc -std=c99 -march=native -O3 -shared -undefined=dynamic_lookup -o stoptrigger.so stoptrigger.c
	cc -std=c99 -march=native -O3 -shared -undefined=dynamic_lookup -o mraagpio.so mraagpio.c
