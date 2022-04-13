#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>
#define N_FITS_HEADER_MAX_LINE_LENGTH 80

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
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

// the fits standard/specification says: "Each header or data unit must be an exact multiple of 2880 bytes long"
#define N_FITS_STANDARD_UNIT_FACTOR 2880 

struct O_fits_image {
int n_fits_standard_unit_factor;
  long n_bits_per_pixel;
  long n_bytes_per_pixel;
  long n_width;
  long n_height;
  char * s_path_file_name; 
  char * a_data; // only the image data
  long n_length_a_data;
  long n_length_a_data_padding;
  char * s_header;
  int n_length_s_header;
  int n_length_s_header_padding;
  char * s_header_with_line_breaks; // the header string with a linebreak \n after every 80 chars, 
  int n_length_s_header_with_line_breaks;
  char * a_buffer; // the whole data from the file, with the header
  long n_length_a_buffer;
//   struct O_fits_image_header_property * a_o_fits_image_header_property; // array of o_fits_image_header_property 
//   struct O_fits_image_header_property ** a_o_fits_image_header_property; // array of o_fits_image_header_property
    // struct O_fits_image_header_property * a_o_fits_image_header_property[1000];
    struct O_fits_image_header_property * a_o_fits_image_header_property;
    // struct O_fits_image_header_property * a_o_fits_image_header_property;
    
  long n_length_a_o_fits_image_header_property; 
};

// fits image struct 
struct O_fits_image_header_property {
    char * s_line;
    char * s_property; 
    char * s_value; 
    char * s_comment; 
    long  n_value_long; 
    double   n_value_double;
    // struct O_fits_image_header_property * o_next_struct; 
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

void f_print_a_o_fits_image_header_property(
    struct O_fits_image * o_fits_image
){
    // printf("f_print_a_o_fits_image_header_property called");
    int n_index = 1; 
    // printf("o_fits_image->n_length_a_o_fits_image_header_property:%i", o_fits_image->n_length_a_o_fits_image_header_property);

    while(n_index < o_fits_image->n_length_a_o_fits_image_header_property-1){
        printf("a_o_fits_image_header_property[n_index].s_line: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_line);
        printf("a_o_fits_image_header_property[n_index].s_property: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_property);
        printf("a_o_fits_image_header_property[n_index].s_value: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_value);
        printf("a_o_fits_image_header_property[n_index].s_comment: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_comment);
        printf("a_o_fits_image_header_property[n_index].n_value_double: %f\n",o_fits_image->a_o_fits_image_header_property[n_index].n_value_double);
        printf("a_o_fits_image_header_property[n_index].n_value_long: %li\n",o_fits_image->a_o_fits_image_header_property[n_index].n_value_long);
        n_index++;
    }
}
void f_assign_header_string(
    struct O_fits_image * o_fits_image
){
    o_fits_image->s_header = malloc(1);
    // o_fits_image->s_header_with_line_breaks = malloc(1);
    o_fits_image->s_header_with_line_breaks = malloc(1);

    int n_counter_header_lines = 0;
    int b_header_end_reached = 0;
    o_fits_image->n_length_s_header = 0;
    long n_i_s_header_with_line_breaks = 0; 
    long n_i = 0;
    

    struct O_fits_image_header_property a_o_fits_image_header_property[1000];


    char * s_header_line = o_fits_image->a_buffer; 

    while (n_i < o_fits_image->n_length_a_buffer)
    {

        o_fits_image->s_header_with_line_breaks = realloc(o_fits_image->s_header_with_line_breaks, n_i_s_header_with_line_breaks+1);
        o_fits_image->s_header_with_line_breaks[n_i_s_header_with_line_breaks] = o_fits_image->a_buffer[n_i];

        o_fits_image->s_header = realloc(o_fits_image->s_header, n_i+1);
        o_fits_image->s_header[n_i] = o_fits_image->a_buffer[n_i];
        
        if((n_i+1)%N_FITS_HEADER_MAX_LINE_LENGTH == 0){
            // create a new struct for current header line
            
            // o_fits_image->a_o_fits_image_header_property = realloc(o_fits_image->a_o_fits_image_header_property, sizeof(struct O_fits_image_header_property) * (n_counter_header_lines+1));
            // o_fits_image->a_o_fits_image_header_property[n_counter_header_lines] = (struct O_fits_image_header_property*)malloc(sizeof(struct O_fits_image_header_property));
            // o_fits_image->a_o_fits_image_header_property[n_counter_header_lines] = struct O_fits_image_header_property o_fits_image_header_property;
            f_assign_o_fits_image_header_property(
                &o_fits_image,
                &a_o_fits_image_header_property[n_counter_header_lines],
                // &a_o_fits_image_header_property[n_counter_header_lines],
                s_header_line
            );

            n_counter_header_lines++;
            n_i_s_header_with_line_breaks ++;
            o_fits_image->s_header_with_line_breaks = realloc(o_fits_image->s_header_with_line_breaks, n_i_s_header_with_line_breaks+1);
            o_fits_image->s_header_with_line_breaks[n_i_s_header_with_line_breaks] = '\n';
            // printf("size of current struct %li", sizeof(o_fits_image_header_property));

            

            // printf("o_fits_image->a_o_fits_image_header_property[n_counter_header_lines]->s_property,adress:%p\n", o_fits_image->a_o_fits_image_header_property[n_counter_header_lines]->s_property);

            //increment header line pointer/index
            s_header_line = s_header_line + N_FITS_HEADER_MAX_LINE_LENGTH;
        

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
            o_fits_image->n_length_s_header_padding = o_fits_image->n_fits_standard_unit_factor - (o_fits_image->n_length_s_header % o_fits_image->n_fits_standard_unit_factor);
            o_fits_image->s_header[n_i] = '\0';
        }

        n_i++;
        n_i_s_header_with_line_breaks++;
        if(b_header_end_reached){
            break;
        }

    }

    // exit(0);
    // printf("n_counter_header_lines: %i\n",n_counter_header_lines);
    o_fits_image->n_length_a_o_fits_image_header_property = n_counter_header_lines;
    // printf("o_fits_image->n_length_a_o_fits_image_header_property: %li\n",o_fits_image->n_length_a_o_fits_image_header_property);
    o_fits_image->a_o_fits_image_header_property = a_o_fits_image_header_property;
    // int n_index = 0;
    // while(n_index < o_fits_image->n_length_a_o_fits_image_header_property-1){
    //     printf("a_o_fits_image_header_property[n_index].s_line: %s\n",a_o_fits_image_header_property[n_index].s_line);
    //     n_index++;
    // }
    // f_print_a_o_fits_image_header_property(o_fits_image);

    // o_fits_image->s_header[n_i+1] = '\0';
    // o_fits_image->s_header_with_line_breaks[n_i+n_counter_new_lines+1] = '\0';



    // printf("o_fits_image->a_buffer[0]: %c\n", o_fits_image->a_buffer[0]);
    // printf("o_fits_image->n_length_a_buffer: %li\n", o_fits_image->n_length_a_buffer);
    // printf("o_fits_image->a_data[0]: %c\n", o_fits_image->a_data[0]);
    // printf("o_fits_image->n_length_data: %li\n", o_fits_image->n_length_a_data);
    // exit(0);
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

// void f_o_find_o_fits_image_header_property(
struct O_fits_image_header_property f_o_find_o_fits_image_header_property(
    struct O_fits_image * o_fits_image, 
    char * s_fits_header_keyword_searchterm
){
    // f_print_a_o_fits_image_header_property(o_fits_image);
    // printf("f_print_a_o_fits_image_header_property called\n");
    int n_index = 0; 
    int bn_index_found = 0;
    int b_return_strcmp = 0;

    printf("o_fits_image->n_length_a_o_fits_image_header_property:%li", o_fits_image->n_length_a_o_fits_image_header_property);

    while(n_index < o_fits_image->n_length_a_o_fits_image_header_property-1){

        if(o_fits_image->a_o_fits_image_header_property[n_index].s_property == NULL){
            n_index++;
            continue;
        }
        // printf("s_fits_header_keyword_searchterm:%s\n", s_fits_header_keyword_searchterm);
        // printf("a_o_fits_image_header_property[n_index].s_property:%s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_property);
        b_return_strcmp = strcmp(o_fits_image->a_o_fits_image_header_property[n_index].s_property, s_fits_header_keyword_searchterm);
        // printf("b_return_strcmp:%i\n", b_return_strcmp);
        if(
            b_return_strcmp == 0
        ){
            bn_index_found = n_index;
            // printf("a_o_fits_image_header_property[n_index].s_line: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_line);
            // printf("a_o_fits_image_header_property[n_index].s_property: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_property);
            // printf("a_o_fits_image_header_property[n_index].s_value: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_value);
            // printf("a_o_fits_image_header_property[n_index].s_comment: %s\n",o_fits_image->a_o_fits_image_header_property[n_index].s_comment);
            // printf("a_o_fits_image_header_property[n_index].n_value_double: %f\n",o_fits_image->a_o_fits_image_header_property[n_index].n_value_double);
            // printf("a_o_fits_image_header_property[n_index].n_value_long: %li\n",o_fits_image->a_o_fits_image_header_property[n_index].n_value_long);
            break;
        }
        n_index++;
    }

    if(bn_index_found == 0){
        printf("could not find .fits header with property value: %s\n", s_fits_header_keyword_searchterm);
        printf("attention, the property is CaSeSenSitiVe!\n");
        printf("available fits headers are:\n%s\n", o_fits_image->s_header_with_line_breaks);
        exit(0);
    }
    return o_fits_image->a_o_fits_image_header_property[bn_index_found];

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
int f_assign_o_fits_image_header_property(
    struct O_fits_image * o_fits_image, 
    struct O_fits_image_header_property * o_fits_image_header_property, 
    char * s_header_line_start // the pointer/index of the start of a fits header line
 ){

    //  printf("address of O_fits_image_header_property struct is: %p", o_fits_image_header_property);

    char * s_header_string = o_fits_image->s_header;
    
    int n_i = 0;

    o_fits_image_header_property->s_line = malloc(N_FITS_HEADER_MAX_LINE_LENGTH+1);

    memcpy(o_fits_image_header_property->s_line, s_header_line_start, N_FITS_HEADER_MAX_LINE_LENGTH);
    o_fits_image_header_property->s_line[N_FITS_HEADER_MAX_LINE_LENGTH] = '\0';
    // printf("\no_fits_image_header_property->s_line:%s\n", o_fits_image_header_property->s_line);
    

    int n_line_counter = 0;
    int n_property_counter = 1;
    int n_value_counter = 0;
    int n_comment_counter = 0;
    char s_equal_identifier = '=';
    char s_slash_identifier = '/';
    // char * s_property = malloc(1);
    // char * s_value = malloc(1);
    // char * s_comment = malloc(1);

    // while(n_line_counter < strlen(o_fits_image_header_property->s_line)){
    //     // printf("o_fits_image_header_property->s_line[n_line_counter]:%c\n", o_fits_image_header_property->s_line[n_line_counter]);

    //     if(o_fits_image_header_property->s_line[n_line_counter] == s_equal_identifier){
    //         n_value_counter = 1;
    //         n_line_counter++;//skip the identifier

    //     }
    //     if(o_fits_image_header_property->s_line[n_line_counter] == s_slash_identifier){
    //         n_comment_counter = 1;
    //         n_line_counter++;//skip the identifier

    //     }

    //     if(
    //         n_property_counter
    //         && !(n_value_counter)
    //         && !(n_comment_counter)
    //     ){
    //         s_property = realloc(s_property, n_property_counter+1);
    //         s_property[n_property_counter-1] = o_fits_image_header_property->s_line[n_line_counter];
    //         s_property[n_property_counter] = '\0';
    //         // s_property[n_property_counter-1] = 'a';
    //     }
    //     if(
    //         n_value_counter
    //         && !(n_comment_counter)
    //     ){
    //         s_value = realloc(s_value, n_value_counter+1);
    //         s_value[n_value_counter-1] = o_fits_image_header_property->s_line[n_line_counter];
    //         s_value[n_value_counter] = '\0';
    //     }
    //     if(
    //         n_comment_counter
    //     ){
    //         s_comment = realloc(s_comment, n_comment_counter+1);
    //         s_comment[n_comment_counter-1] = o_fits_image_header_property->s_line[n_line_counter];
    //         s_comment[n_comment_counter] = '\0';
    //     }
    //     if(n_value_counter) n_value_counter++;
    //     if(n_comment_counter) n_comment_counter++;
    //     n_property_counter++;
    //     n_line_counter++;
    // }

    char * n_index_of_equal_symbol = strstr(o_fits_image_header_property->s_line, "=");
    char * n_index_of_slash_symbol = strstr(o_fits_image_header_property->s_line, "/");
    if(n_index_of_equal_symbol == NULL && n_index_of_slash_symbol == NULL){
        return 0;
    }
    // printf("n_index_of_equal_symbol :%lld\n", n_index_of_equal_symbol);
    // printf("n_index_of_slash_symbol :%lld\n", n_index_of_slash_symbol);
    // OBJCTDEC= '+47 11 42.9'        / [dms +N J2000] Target declination
    // ^(0)
    // o_fits_image_header_property->s_line
    //         ^(8)
    //         n_index_of_equal_symbol
    //                                ^(32)
    //                                n_index_of_slash_symbol                     
    //                                                                  ^(80)
    //                                                                  n_index_of_line_end (80 characters)         
    char * n_index_of_line_end = (o_fits_image_header_property->s_line+N_FITS_HEADER_MAX_LINE_LENGTH);
    int n_length_s_property = n_index_of_equal_symbol - o_fits_image_header_property->s_line; 
    int n_length_s_value = n_index_of_slash_symbol - n_index_of_equal_symbol; 
    int n_length_s_comment = n_index_of_line_end - n_index_of_slash_symbol;
    // printf("n_index_of_line_end     :%lld\n", n_index_of_line_end);
    // printf("n_length_s_property:%i\n", n_length_s_property);
    // printf("n_length_s_value:%i\n", n_length_s_value);
    // printf("n_length_s_comment:%i\n", n_length_s_comment);
    if(n_index_of_slash_symbol == NULL){
        n_length_s_value = n_index_of_line_end - n_index_of_equal_symbol;
        n_length_s_comment = 0;
    }
    char * s_property = malloc(n_length_s_property+1);
    char * s_value = malloc(n_length_s_value+1);
    char * s_comment = malloc(n_length_s_comment+1);

    s_property[n_length_s_property] = '\0';
    s_value[n_length_s_value] = '\0';
    s_comment[n_length_s_comment] = '\0';

    memcpy(s_property, o_fits_image_header_property->s_line, n_length_s_property);
    memcpy(s_value, n_index_of_equal_symbol+1, n_length_s_value);
    if(n_length_s_comment>0){
        memcpy(s_comment, n_index_of_slash_symbol+1, n_length_s_comment);
    }

    // printf("o_fits_image_header_property->s_line     :%s\n", o_fits_image_header_property->s_line);
    // printf("o_fits_image_header_property->s_line     :%lld\n", o_fits_image_header_property->s_line);

    // printf("n_index_of_line_end     :%lld\n", n_index_of_line_end);
    // printf("s_header_line_start:%p\n", s_header_line_start);
    // printf("n_index_of_line_end:%p\n", n_index_of_line_end);


    // printf("s_property:%s\n", s_property);
    // printf("s_value:%s\n", s_value);
    // printf("s_comment:%s\n", s_comment);


    // char *result = strstr(str, "abc");


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

    // printf("o_fits_image_header_property->s_property,adress:%p\n", o_fits_image_header_property->s_property);
    // exit(0);

    // printf("o_fits_image_header_property->s_line:%s\n", o_fits_image_header_property->s_line);
    // printf("o_fits_image_header_property->s_property:%s\n", o_fits_image_header_property->s_property);
    // printf("o_fits_image_header_property->s_value:%s\n", o_fits_image_header_property->s_value);
    // printf("o_fits_image_header_property->s_comment:%s\n", o_fits_image_header_property->s_comment);
    // printf("o_fits_image_header_property->n_value_long:%li\n", o_fits_image_header_property->n_value_long);
    // printf("o_fits_image_header_property->n_value_double:%f\n", o_fits_image_header_property->n_value_double);

    // printf("\n");
}
void f_search_header_line_old(){
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
}
void f_assign_fits_struct(
    struct O_fits_image * o_fits_image, 
    char * s_path_file_name 
){

    // i want to have this on the struct
    o_fits_image->n_fits_standard_unit_factor = N_FITS_STANDARD_UNIT_FACTOR;

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
    

    // printf("%p\n",o_fits_image->a_o_fits_image_header_property[0].s_line);

    // printf("o_fits_image->s_header:%s\n", o_fits_image->s_header);
    // printf("o_fits_image->s_header_with_line_breaks:%s\n", o_fits_image->s_header_with_line_breaks);
    struct O_fits_image_header_property o_fits_image_header_property_naxis1 = f_o_find_o_fits_image_header_property(
        o_fits_image, 
        "NAXIS1"
    );

    o_fits_image->n_width = o_fits_image_header_property_naxis1.n_value_long;

    struct O_fits_image_header_property o_fits_image_header_property_naxis2 = f_o_find_o_fits_image_header_property(
        o_fits_image, 
        "NAXIS2"
    );
    o_fits_image->n_height = o_fits_image_header_property_naxis2.n_value_long;

    struct O_fits_image_header_property o_fits_image_header_property_bitpix = f_o_find_o_fits_image_header_property(
        o_fits_image, 
        "BITPIX"
    );

    o_fits_image->n_bits_per_pixel = o_fits_image_header_property_bitpix.n_value_long;
    o_fits_image->n_bytes_per_pixel = o_fits_image->n_bits_per_pixel/8;

    o_fits_image->n_length_a_data = (
        o_fits_image->n_width *
        o_fits_image->n_height *
        o_fits_image->n_bytes_per_pixel
    );

    o_fits_image->n_length_a_data_padding = 
        o_fits_image->n_fits_standard_unit_factor - 
        (o_fits_image->n_length_a_data % o_fits_image->n_fits_standard_unit_factor);

    o_fits_image->a_data = 
        o_fits_image->a_buffer+o_fits_image->n_length_s_header + 
        o_fits_image->n_length_s_header_padding
        + 1; // increment the index/pointer with the header length

    // printf("o_fits_image->n_width:%li\n", o_fits_image_header_property_naxis1.n_value_long);
    // printf("o_fits_image->n_height:%li\n", o_fits_image_header_property_naxis2.n_value_long);

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

    setlocale(LC_NUMERIC, "");
    printf(ANSI_COLOR_GREEN "Hooray, .f(r)its was loaded successfully! \n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "o_fits_image->s_header_with_line_breaks :" ANSI_COLOR_RESET "\n%s\n", o_fits_image.s_header_with_line_breaks);
    // printf(ANSI_COLOR_CYAN "o_fits_image->s_header :" ANSI_COLOR_RESET "\n%s\n", o_fits_image.s_header);
    // after header
    printf(ANSI_COLOR_CYAN "o_fits_image->s_path_file_name              :" ANSI_COLOR_RESET "%s\n", o_fits_image.s_path_file_name);
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_s_header (chars)     :" ANSI_COLOR_RESET "%li\n", o_fits_image.n_length_s_header);
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_a_data   (bytes)     :" ANSI_COLOR_RESET "%'ld\n", o_fits_image.n_length_a_data);
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_a_buffer (bytes)     :" ANSI_COLOR_RESET "%'ld\n", o_fits_image.n_length_a_buffer);
    printf(ANSI_COLOR_CYAN "o_fits_image->n_width (NAXIS1)              :" ANSI_COLOR_RESET "%li\n", o_fits_image.n_width);
    printf(ANSI_COLOR_CYAN "o_fits_image->n_height (NAXIS2)             :" ANSI_COLOR_RESET "%li\n", o_fits_image.n_height);
    printf(ANSI_COLOR_CYAN "o_fits_image->n_bits_per_pixel (BITPIX)     :" ANSI_COLOR_RESET "%li\n", o_fits_image.n_bits_per_pixel);
    int n_bytes_per_pixel = o_fits_image.n_bits_per_pixel/8;
    printf(ANSI_COLOR_CYAN "bytes per pixel (BITPIX/8)                  :" ANSI_COLOR_RESET "%i\n", n_bytes_per_pixel);
    int n_amount_of_pixels = (o_fits_image.n_width*o_fits_image.n_height);
    printf(ANSI_COLOR_CYAN "amount of pixels(width*height)              :" ANSI_COLOR_RESET "%'ld\n", (long) n_amount_of_pixels);
    long n_expected_number_of_bytes = n_amount_of_pixels * (o_fits_image.n_bits_per_pixel/8);
    printf(ANSI_COLOR_CYAN "expected num of bytes (width*height*2)      :" ANSI_COLOR_RESET "%'ld\n", (long) (n_expected_number_of_bytes));
    printf(ANSI_COLOR_CYAN "n_length_buffer - expctd. n of byts         :" ANSI_COLOR_RESET "%'ld\n", (long) o_fits_image.n_length_a_buffer );
    printf(ANSI_COLOR_CYAN "o_fits_image->n_fits_standard_unit_factor   :" ANSI_COLOR_RESET "%'ld\n", (long) o_fits_image.n_fits_standard_unit_factor );
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_s_header_padding     :" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_CYAN "(2880-(n_length_s_header %% 2880))           :" ANSI_COLOR_RESET "%'ld\n", (long) o_fits_image.n_length_s_header_padding );
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_a_data_padding       :" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_CYAN "(2880-(n_length_a_data %% 2880))             :" ANSI_COLOR_RESET "%'ld\n", (long) o_fits_image.n_length_a_data_padding );
    
    int n_buffer_length_calculated_with_header_and_data = 
        o_fits_image.n_length_s_header +
        o_fits_image.n_length_s_header_padding +
        o_fits_image.n_length_a_data +
        o_fits_image.n_length_a_data_padding;
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_s_header +            :" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_s_header_padding +    :" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_a_data +              :" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_CYAN "o_fits_image->n_length_a_data_padding +      :" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_CYAN "                                             :" ANSI_COLOR_RESET "%'d\n", (long) n_buffer_length_calculated_with_header_and_data );

    // int n_index_buffer = 0;
    // the fits standard says: "Each header or data unit must be an exact multiple of 2880 bytes long"
    // while(n_index_buffer < o_fits_image.n_length_s_header + o_fits_image.n_length_s_header_padding){
    //     printf("%c", o_fits_image.a_buffer[n_index_buffer]);
    //     n_index_buffer ++;
    // }
    // printf("n_index_buffer:%i\n", n_index_buffer);
    // printf("o_fits_image.a_buffer[n_index_buffer+1]:%i\n", o_fits_image.a_buffer[n_index_buffer+1]);
    // printf("o_fits_image.a_buffer[n_index_buffer+2]:%i\n", o_fits_image.a_buffer[n_index_buffer+2]);
    // printf("o_fits_image.a_buffer[n_index_buffer+3]:%i\n", o_fits_image.a_buffer[n_index_buffer+3]);

    // printf("o_fits_image.a_data[0]:%i\n", o_fits_image.a_data[0]);
    // printf("o_fits_image.a_data[1]:%i\n", o_fits_image.a_data[1]);
    // printf("o_fits_image.a_data[2]:%i\n", o_fits_image.a_data[2]);
    // exit(0);

    printf("o_fits_image.a_data[0]:%i\n", o_fits_image.a_data[0]);
    printf("o_fits_image.a_data[1]:%i\n", o_fits_image.a_data[1]);
    printf("o_fits_image.a_data[2]:%i\n", o_fits_image.a_data[2]);    

    // o_fits_image.a_data[0] = 0xff00ffeeff00;
    // o_fits_image.a_data[0] = 0xfefefe;
    // o_fits_image.a_data[0] = 0xefef;
    // printf("o_fits_image.a_data[0]:%x\n", (__uint8_t) o_fits_image.a_data[0]);
    // printf("o_fits_image.a_data[1]:%x\n", (__uint8_t) o_fits_image.a_data[1]);
    
    // // check to see what comes after header padding, it should be image data? 
    // int n_manual_after_padding = 100;
    // for (size_t i = n_index_buffer; i < n_index_buffer+n_manual_after_padding; i++)
    // {
    //     /* code */
    //     printf("%i\n", o_fits_image.a_buffer[n_index_buffer]);

    // }
    
    
    // write new data
    long n_index_pixel = 0; 
    double n_value_per_pixel = pow(2, (double) o_fits_image.n_bits_per_pixel) / (o_fits_image.n_height*o_fits_image.n_width);
    while (n_index_pixel < o_fits_image.n_height*o_fits_image.n_width)
    {
        int n_index_lower_byte = n_index_pixel*2;
        int n_index_higher_byte = (n_index_pixel*2)+1;
        // o_fits_image.a_data[n_index_higher_byte] = 0b11111111;
        // o_fits_image.a_data[n_index_lower_byte] = 0b00000000;
        
        // o_fits_image.a_data[n_index_higher_byte] = (__int8_t)  (f_n_random_normalized() * 255);
        // o_fits_image.a_data[n_index_lower_byte] = (__int8_t) ( f_n_random_normalized() * 255);

        // o_fits_image.a_data[n_index_higher_byte] = (__int8_t)  (f_n_random_normalized() * 255);
        // o_fits_image.a_data[n_index_lower_byte] = (__int8_t) 0;

        // int n_value_16_bit = (int) (n_index_pixel % ((int) (pow(2, (double) o_fits_image.n_bits_per_pixel))));
        __uint16_t n_value_16_bit = (int) (n_index_pixel * n_value_per_pixel);
        __uint8_t n_value_lower_byte = n_value_16_bit; 
        __uint8_t n_value_higher_byte = (n_value_16_bit>>8); 
        // o_fits_image.a_data[n_index_pixel] = (__int8_t) f_n_random_normalized() * 255;

        o_fits_image.a_data[n_index_higher_byte] = n_value_higher_byte;
        o_fits_image.a_data[n_index_lower_byte] = n_value_lower_byte;

        n_index_pixel++;
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
        puts(ANSI_COLOR_GREEN "WrOtE " ANSI_COLOR_RESET ANSI_COLOR_YELLOW" tO " ANSI_COLOR_RESET ANSI_COLOR_MAGENTA " fILe!" ANSI_COLOR_RESET);

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

}
// printf("buffer is %s", buffer);
// start to process your data / extract strings here...

int main()
{
    do_stuff();

    // f_char_pointer_test();
    return 0;
}