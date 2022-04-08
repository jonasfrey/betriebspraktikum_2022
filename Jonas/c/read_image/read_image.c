#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// gcc -c read_image.c -o read_image && ./read_image
int main(){
    const char * s_path_file_name_input =  "./../../../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts";
    const char * s_path_file_name_output =  "'./output_image_from_c.fts";

    char * buffer = 0;
    long n_buffer_length;
    FILE * f = fopen (s_path_file_name_input, "rb");
    if (f == NULL) {
        perror("Failed: ");
        return 1;
    }
    if (f)
    {
      fseek (f, 0, SEEK_END);
      n_buffer_length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (n_buffer_length);
      if (buffer)
      {
        fread (buffer, 1, n_buffer_length, f);
      }
      fclose (f);
    }

    printf("bufferlen is %li", n_buffer_length);
    return 0;
    
    if (buffer)
    {
        long n_i = 0; 
        while(n_i < n_buffer_length){

            printf("n_i is %li \n", n_i);
            // printf("buffer[%li] is %x \n", buffer[n_i], n_i);
            
            n_i = n_i + 1;
        }
        // printf("buffer is %s", buffer);
      // start to process your data / extract strings here...
    }
    // printf("test");

}