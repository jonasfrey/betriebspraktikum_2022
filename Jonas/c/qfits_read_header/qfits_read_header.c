
// c file 
#include <stdio.h>

// how to build fitsio.h 
// download http://heasarc.gsfc.nasa.gov/FTP/software/fitsio/c/cfitsio_latest.tar.gz from https://heasarc.gsfc.nasa.gov/docs/software/fitsio/fitsio.html
//  >  ./configure [--prefix=/target/installation/path]
//  >  make          (or  'make shared')
//  >  make install  (this step is optional)

#include "./../cfitsio_latest/cfitsio-4.1.0/fitsio.h"

const char * s_path_file_name_input =  "./../../../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts";


int main() {
   /* my first program in C */
   printf("main function called");
   
   return 0;
}
