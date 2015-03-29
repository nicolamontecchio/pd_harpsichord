all :
	cc -march=native -O3 -shared -undefined=dynamic_lookup -o stoptrigger.pd_darwin stoptrigger.c
