
CFLAGS = -Wall -g -O3

fitsio_test: fitsio_test.c
	gcc $(CFLAGS) fitsio_test.c -o fitsio_test -lcfitsio -lm -lnsl -lsocket -L./../cfitsio_latest/cfitsio-4.1.0  


clean:
	rm -f fitsio_test

