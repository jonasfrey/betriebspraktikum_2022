#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define N_FITS_HEADER_MAX_LINE_LENGTH 80


// coding standard
// |prefix|datatype(s)|description|example|  
// |---|---|---|---|  
// |n_|int,float,double|number|1.1, 2, -20...|
// |b_|boolean|true or false, or int|true, false, 1=true,123=true,0=false|
// |s_|string|"hello", 'h'|string or char any alphanumeric symbol|
// |a_|array|[1,2,3], [-1,0.443,"asdf"]| array or list|
// 
// |f_|function|void(i){ i+2 }|function without return value| 
// |f_n_|function|int(i){ return i*2 }|function with return value, second prefix is the type of the return|
// |f_assign|function|int(char*s_test) {*s_test[0] ='a'}| function which expects a pointer, and which will change the value of the pointer| 

// fits image struct 
struct O_fits_image {
  int n_width;
  int n_height;
  char * s_path_file_name; 
  char * a_data; // only the image data
  long n_length_a_data;
  char * s_header;
  int n_length_s_header;
  char * s_header_with_line_breaks; // the header string with a linebreak \n after every 80 chars, 
  int n_length_s_header_with_line_breaks;
  char * a_buffer; // the whole data from the file, with the header
  long n_length_a_buffer;
};

// fits image struct 
struct O_fits_image_header_property {
    char * s_property_searchstring;
    char * s_line;
    char * s_property; 
    char * s_value; 
    char * s_comment; 
    long  n_value_long; 
    double   n_value_double;
};

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

float f_n_random_normalized()
{
      float r = (float)rand() / (float)RAND_MAX;
      return r;
}

void f_assign_header_string(
    struct O_fits_image * o_fits_image
){
    
    o_fits_image->s_header = malloc(1);
    o_fits_image->s_header_with_line_breaks = malloc(1);
    int b_header_end_reached = 0;
    o_fits_image->n_length_s_header = 0;
    int n_counter_new_lines = 0; 
    long n_i = 0;
    while (n_i < o_fits_image->n_length_a_buffer)
    {
        if((n_i+1)%N_FITS_HEADER_MAX_LINE_LENGTH == 0){
            // realloc(o_fits_image->s_header_with_line_breaks, n_i+n_counter_new_lines+1);
            // o_fits_image->s_header_with_line_breaks[n_i+n_counter_new_lines] = '\n';
            n_counter_new_lines ++;
        }
        // printing a header line
        if (!b_header_end_reached)
        {
            // realloc(o_fits_image->s_header_with_line_breaks, n_i+n_counter_new_lines+1);
            // o_fits_image->s_header_with_line_breaks[n_i+n_counter_new_lines] = o_fits_image->a_buffer[n_i];
            realloc(o_fits_image->s_header, n_i+1);
            o_fits_image->s_header[n_i] = o_fits_image->a_buffer[n_i];

        }

        // checking for end of header marked with "END" keyword
        if (
            o_fits_image->a_buffer[n_i - 1] == 'D' &&
            o_fits_image->a_buffer[n_i - 2] == 'N' &&
            o_fits_image->a_buffer[n_i - 3] == 'E'
            )
        {
            // printf("header end is reached");
            b_header_end_reached = 1;
            o_fits_image->n_length_s_header = n_i;
        }

        n_i = n_i + 1;
    }
    
    // o_fits_image->s_header[n_i+1] = '\0';
    // o_fits_image->s_header_with_line_breaks[n_i+n_counter_new_lines+1] = '\0';


    o_fits_image->a_data = o_fits_image->a_buffer+o_fits_image->n_length_s_header; // increment the index/pointer with the header length
    o_fits_image->n_length_a_data = o_fits_image->n_length_a_buffer-o_fits_image->n_length_s_header;
    exit(0);
    printf("o_fits_image->a_buffer[0]: %c\n", o_fits_image->a_buffer[0]);
    printf("o_fits_image->n_length_a_buffer: %li\n", o_fits_image->n_length_a_buffer);
    printf("o_fits_image->a_data[0]: %c\n", o_fits_image->a_data[0]);
    printf("o_fits_image->n_length_data: %li\n", o_fits_image->n_length_a_data);
}

long f_n_index_of(
    char * s_haystack, 
    char * s_needle
){
    char * s_char_pointer_substrng = strstr(s_haystack, s_needle);
    long n_index = -1;

    if(s_char_pointer_substrng != NULL){
        // printf("pointer s_char_pointer_substrng %p \n", s_char_pointer_substrng);
        // printf("pointer s_haystack %p \n", s_haystack);
        n_index = s_char_pointer_substrng - s_haystack;
        // printf("index of = -> %li\n", n_index_of_equal);
    }
    return n_index;
}

// gcc -c read_image.c -o read_image && ./read_image
void f_open_fits(
    char *s_path_file_name, // a path file name to the file
    char **a_buffer,         // a pointer to the buffer where the fits will be loaded into
    long * n_buffer_length    // a pointer to the integer
)
{
    *n_buffer_length = 0;

    FILE *f = fopen(s_path_file_name, "rb");
    if (f == NULL)
    {
        perror("Failed: ");
        return 1;
    }
    if (f)
    {
        fseek(f, 0, SEEK_END);
        *n_buffer_length = ftell(f);
        fseek(f, 0, SEEK_SET);
        *a_buffer = malloc(*n_buffer_length);
        if (*a_buffer)
        {
            fread(*a_buffer, 1, *n_buffer_length, f);
        }
        fclose(f);
    }

    // printf("bufferlen is %li\n", *n_buffer_length);

    if (*a_buffer)
    {
        printf("open file success\n");
    }
}
void f_assign_o_fits_image_header_property(
    struct O_fits_image * o_fits_image, 
    struct O_fits_image_header_property * o_fits_image_header_property, 
    char * s_fits_image_header_propert_searchstring
){

    char * s_header_string = o_fits_image->s_header;

    char * s_prop_value = malloc(1);
    int n_i = 0;
    o_fits_image_header_property->s_property_searchstring = s_fits_image_header_propert_searchstring;

    o_fits_image_header_property->s_line = malloc(N_FITS_HEADER_MAX_LINE_LENGTH+1);
    // o_fits_image_header_property->s_property = memcpy(N_FITS_HEADER_MAX_LINE_LENGTH+1);
    // o_fits_image_header_property->s_value = memcpy(N_FITS_HEADER_MAX_LINE_LENGTH+1);
    // o_fits_image_header_property->s_comment = memcpy(N_FITS_HEADER_MAX_LINE_LENGTH+1);
    // o_fits_image_header_property->n_value_double = memcpy(N_FITS_HEADER_MAX_LINE_LENGTH+1);
    // o_fits_image_header_property->n_value_long = memcpy(N_FITS_HEADER_MAX_LINE_LENGTH+1);

    char * s_index_of_search = strstr(s_header_string, o_fits_image_header_property->s_property_searchstring); 
    
    if(s_index_of_search == NULL){
        printf("property %s was not found in fits header\n", o_fits_image_header_property->s_property_searchstring);
    }

    // strncpy(s_header_string, s_header_line,79); 
    memcpy(o_fits_image_header_property->s_line, s_index_of_search, N_FITS_HEADER_MAX_LINE_LENGTH);
    o_fits_image_header_property->s_line[N_FITS_HEADER_MAX_LINE_LENGTH] = '\0';
    // printf("o_fits_image_header_property->s_line:%s\n", o_fits_image_header_property->s_line);
    int n_line_counter = 0;
    int n_property_counter = 1;
    int n_value_counter = 0;
    int n_comment_counter = 0;
    char s_equal_identifier = '=';
    char s_slash_identifier = '/';
    char * s_property = malloc(1);
    char * s_value = malloc(1);
    char * s_comment = malloc(1);

    while(n_line_counter < strlen(o_fits_image_header_property->s_line)){
        // printf("o_fits_image_header_property->s_line[n_line_counter]:%c\n", o_fits_image_header_property->s_line[n_line_counter]);

        if(o_fits_image_header_property->s_line[n_line_counter] == s_equal_identifier){
            n_value_counter = 1;
            n_line_counter++;//skip the identifier

        }
        if(o_fits_image_header_property->s_line[n_line_counter] == s_slash_identifier){
            n_comment_counter = 1;
            n_line_counter++;//skip the identifier

        }

        if(
            n_property_counter
            && !(n_value_counter)
            && !(n_comment_counter)
        ){
            s_property = realloc(s_property, n_property_counter+1);
            s_property[n_property_counter-1] = o_fits_image_header_property->s_line[n_line_counter];
            s_property[n_property_counter] = '\0';
            // s_property[n_property_counter-1] = 'a';
        }
        if(
            n_value_counter
            && !(n_comment_counter)
        ){
            s_value = realloc(s_value, n_value_counter+1);
            s_value[n_value_counter-1] = o_fits_image_header_property->s_line[n_line_counter];
            s_value[n_value_counter] = '\0';
        }
        if(
            n_comment_counter
        ){
            s_comment = realloc(s_comment, n_comment_counter+1);
            s_comment[n_comment_counter-1] = o_fits_image_header_property->s_line[n_line_counter];
            s_comment[n_comment_counter] = '\0';
        }
        if(n_value_counter) n_value_counter++;
        if(n_comment_counter) n_comment_counter++;
        n_property_counter++;
        n_line_counter++;
    }

    // long n_index_of_searchterm = f_n_index_of(
    //     o_fits_image_header_property->s_line,
    //     o_fits_image_header_property->s_property_searchstring
    // );
    // int n_len_s_equal_identifier = strlen(s_equal_identifier);
    // int n_len_s_slash_identifier = strlen(s_slash_identifier);

    // long n_index_of_equal = f_n_index_of(o_fits_image_header_property->s_line, s_equal_identifier);
    // long n_index_of_slash = f_n_index_of(o_fits_image_header_property->s_line, s_slash_identifier);
    
    // // printf("index of =: %li , /: %li ", n_index_of_equal, n_index_of_slash);
    // long n_end_index_value; 
    // if(n_index_of_slash == -1){
    //     n_end_index_value = N_FITS_HEADER_MAX_LINE_LENGTH;
    // }else{
    //     n_end_index_value = n_index_of_slash; 
    // }
    // memcpy(
    //     o_fits_image_header_property->s_property,
    //     o_fits_image_header_property->s_line,
    //     strlen(o_fits_image_header_property->s_property_searchstring)
    //     );
    // memcpy(
    //     o_fits_image_header_property->s_value,
    //     o_fits_image_header_property->s_line+n_index_of_equal+n_len_s_equal_identifier,
    //     n_end_index_value-n_index_of_equal-n_len_s_equal_identifier
    //     );
    
    // if(n_index_of_slash != -1){
    //     memcpy(
    //         o_fits_image_header_property->s_comment,
    //         o_fits_image_header_property->s_line+n_index_of_slash+n_len_s_slash_identifier,
    //         N_FITS_HEADER_MAX_LINE_LENGTH-n_index_of_slash-n_len_s_slash_identifier
    //         );
    // }



    // printf("s_property:%s\n", s_property);
    // printf("s_value:%s\n", s_value);
    // printf("s_comment:%s\n", s_comment);

    s_property = trimwhitespace(s_property);
    s_value = trimwhitespace(s_value);
    s_comment = trimwhitespace(s_comment);


    // printf("s_property:%s\n", s_property);
    // printf("s_value:%s\n", s_value);
    // printf("s_comment:%s\n", s_comment);
    o_fits_image_header_property->s_property = s_property;
    o_fits_image_header_property->s_value = s_value;
    o_fits_image_header_property->s_comment = s_comment;
    o_fits_image_header_property->n_value_long = atol(s_value);
    o_fits_image_header_property->n_value_double = atof(s_value);

    printf("o_fits_image_header_property->s_property:%s\n", o_fits_image_header_property->s_property);
    printf("o_fits_image_header_property->s_value:%s\n", o_fits_image_header_property->s_value);
    printf("o_fits_image_header_property->s_comment:%s\n", o_fits_image_header_property->s_comment);
    printf("o_fits_image_header_property->n_value_long:%li\n", o_fits_image_header_property->n_value_long);
    printf("o_fits_image_header_property->n_value_double:%f\n", o_fits_image_header_property->n_value_double);

}
void f_assign_fits_struct(
    struct O_fits_image * o_fits_image, 
    char * s_path_file_name 
){

    o_fits_image->s_path_file_name = malloc(strlen(s_path_file_name)+1);
    strcpy(o_fits_image->s_path_file_name, s_path_file_name);
    // printf("o_fits_image.s_path_file_name:%s\n", o_fits_image->s_path_file_name);

    // // open fits image 
    f_open_fits(
        o_fits_image->s_path_file_name, 
        &o_fits_image->a_buffer, 
        &o_fits_image->n_length_a_buffer
    );

    // test if fits was opened
    // int n_i = 0; 
    // while(n_i < 100){
    //     printf("%c\n", o_fits_image->a_buffer[n_i]);
    // }
    
    
    // get header information 
    f_assign_header_string(
        o_fits_image
    );
    

    // test is a_data is correctly assigned
    // int n_i = 0; 
    // while(n_i < 100){
    //     printf("%c\n", o_fits_image->a_data[n_i]);
    //     n_i++;
    // }

    // printf("o_fits_image->s_header:%s\n", o_fits_image->s_header);

    
    // o_fits_image->s_header_with_line_breaks =  f_s_get_header_string_with_linebreaks(
    //     o_fits_image->a_buffer,
    //     o_fits_image->a_buffer_length
    // );


    // printf("o_fits_image->s_header_with_line_breaks:%s\n", o_fits_image->s_header_with_line_breaks);

    exit(0);

}

int do_stuff()
{
    // char *s_path_file_name_input = "./../../../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts";
    // char *s_path_file_name_input = "./../../../2020-03-16T20-16-29_Coordinates_FILTER_30s_Maria-B.fts";
    char *s_path_file_name_input = "./../../../2019-10-26T03-26-55_M51_Clear_280s.fts";
    
    char *s_path_file_name_output = "./output_image_from_c.fts";
    
    struct O_fits_image o_fits_image;

    f_assign_fits_struct(
        &o_fits_image, 
        s_path_file_name_input
    );



    // write new data
    long n_i = 0;
    while (n_i < o_fits_image.n_length_a_data)
    {
        // printf("n_i is %li \n", n_i);
        // printf("buffer[%li] is %x \n", buffer[n_i], n_i);
        if (n_i > 100000)
        {
            if (n_i % 2 == 0)
            {
                // convert into 16 bit value
                unsigned int n_16bits = o_fits_image.a_data[n_i];
                n_16bits = n_16bits << 8;
                n_16bits = n_16bits | o_fits_image.a_data[n_i + 1];
                // manipulate 16 bit value
                n_16bits = (int)n_16bits * 1;
                // convert into 2 values, each 8 bit
                unsigned int n_8bit_high = n_16bits >> 8;
                unsigned int n_8bit_low = n_16bits & 0b11111111;
                // o_fits_image.a_data[n_i] = n_8bit_high;
                // o_fits_image.a_data[n_i+1] = n_8bit_low;
                o_fits_image.a_data[n_i] = 128;
            }else{
                o_fits_image.a_data[n_i] = 0;
            }
            if( (n_i % 4) < 2 ){
                o_fits_image.a_data[n_i] = (int) (f_n_random_normalized()*255);
            }
            // a_buffer[n_i] = (int)(pow(a_buffer[n_i],1));
            // a_buffer[n_i] = (int)(a_buffer[n_i]*3.2);
        }

        n_i = n_i + 1;
    }
    
    // write file
    FILE *pFile;
    /* Write your buffer to disk. */
    // pFile = fopen(s_path_file_name_output,"wb");
    pFile = fopen(s_path_file_name_output, "w+");
    // printf("pfile is %i", pFile);
    if (pFile)
    {
        fwrite(o_fits_image.a_buffer, o_fits_image.n_length_a_buffer, 1, pFile);
        puts("Wrote to file!");

        // char s_command[80];
        // strcpy(s_command, "gimp");
        // strcat(s_command, " ");
        // strcat(s_command, s_path_file_name_output);
        // system(s_command);
    }
    else
    {
        puts("Something wrong writing to File.");
    }

    printf("o_fits_image.n_length_a_data: %li", o_fits_image.n_length_a_data);
    printf("o_fits_image.n_length_a_buffer: %li", o_fits_image.n_length_a_buffer);


}
// printf("buffer is %s", buffer);
// start to process your data / extract strings here...

int main()
{
    do_stuff();

    // f_char_pointer_test();
    return 0;
}