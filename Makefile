all :
	cc -std=c99 -march=native -O3 -shared -undefined dynamic_lookup -o stoptrigger.pd_darwin stoptrigger.c
	cp stoptrigger.pd_darwin stoptrigger.so  #horrible hack
