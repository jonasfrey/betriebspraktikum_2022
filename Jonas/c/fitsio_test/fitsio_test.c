
// c file 
#include <stdio.h>

// how to build fitsio.h 
// download http://heasarc.gsfc.nasa.gov/FTP/software/fitsio/c/cfitsio_latest.tar.gz from https://heasarc.gsfc.nasa.gov/docs/software/fitsio/fitsio.html
//  >  ./configure [--prefix=/target/installation/path]
//  >  make          (or  'make shared')
//  >  make install  (this step is optional)

// important !!! link library in makefile
// gcc $(CFLAGS) fitsio_test.c -o fitsio_test -L./../cfitsio_latest/cfitsio-4.1.0  -lcfitsio -lm -lnsl -lsocket

//                                             ^^ setting the path for all the .so files 
#include <string.h>
#include <stdio.h>
// #include "fitsio.h"
#include "./../cfitsio_latest/cfitsio-4.1.0/fitsio.h"


int main(int argc, char *argv[])
{
const char * s_path_file_name_input =  "./../../../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts";

// fitsfile *fptr;         
// char card[FLEN_CARD]; 
// int status = 0,  nkeys, ii;  /* MUST initialize status */

// fits_open_file(&fptr, s_path_file_name_input, READONLY, &status);
//    fits_get_hdrspace(fptr, &nkeys, NULL, &status);

//    for (ii = 1; ii <= nkeys; ii++)  { 
//       fits_read_record(fptr, ii, card, &status); /* read keyword */
//       printf("%s\n", card);
//    }
//    printf("END\n\n");  /* terminate listing with END */
//    fits_close_file(fptr, &status);

//    if (status)          /* print any error messages */
//       fits_report_error(stderr, status);
//    return(status);
}