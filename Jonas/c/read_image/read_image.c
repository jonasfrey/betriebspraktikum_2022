#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define N_FITS_HEADER_MAX_LINE_LENGTH 80

// credits: https://www.delftstack.com/howto/c/trim-string-in-c/#:~:text=string%20temporary%20string-,Use%20Another%20Custom%20Function%20to%20Trim%20a%20String%20in%20C,the%20first%20non%2Dspace%20character.
char *trimString(char *str)
{
    char *end;

    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}
char *trim_string(char *str)
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

char *f_get_header_string(
    char *a_buffer,      // the actual buffer of the fits file,
    long n_buffer_length // the length of the buffer, 
){
    char *s_header_string = malloc(1);
    int b_header_end_reached = 0;
    long n_i = 0;
    while (n_i < n_buffer_length)
    {
        // printing a header line
        if (!b_header_end_reached)
        {
            realloc(s_header_string, n_i+1);
            s_header_string[n_i] = a_buffer[n_i];
        }

        // checking for end of header marked with "END" keyword
        if (a_buffer[n_i] == 'E')
        {
            if (
                a_buffer[n_i + 1] == 'N' &&
                a_buffer[n_i + 2] == 'D')
            {
                // printf("header end is reached");
                b_header_end_reached = 1;
                // from the ending tag "END" -> 'N' and 'D'
                n_i = n_i + 2;
            }
        }

        n_i = n_i + 1;
    }


    return s_header_string;
}
// returns the header, with new lines
char *f_get_header_string_with_linebreaks(
    char *a_buffer,      // the actual buffer of the fits file,
    long n_buffer_length // the length of the buffer, 
)
{

    int n_index = 0; 
    char *s_header_string = f_get_header_string(
        a_buffer, 
        n_buffer_length
    );
    
    int n_s_header_string_length = strlen(s_header_string);
    int n_counter_newlines = 0;
    int n_counter_newlines_expected = (int)(n_s_header_string_length/(float)(N_FITS_HEADER_MAX_LINE_LENGTH));

    char * s_header_string_with_newlines = malloc(n_s_header_string_length+n_counter_newlines_expected);
    
    while(n_index < (n_s_header_string_length-1)){
        
        // printf("%c", a_buffer[n_i]);
        if ((n_index + 1) % (N_FITS_HEADER_MAX_LINE_LENGTH) == 0)
        {
            // printf("\n");
            s_header_string_with_newlines[n_index+n_counter_newlines] = '\n';
            n_counter_newlines++;
        }
        s_header_string_with_newlines[n_index+n_counter_newlines] = s_header_string[n_index];
        n_index++;
    }

    free(s_header_string);
    return s_header_string_with_newlines;
    // return s_header_string_with_newlines; 

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
//
void f_get_header_line_prop_val_comment(
    char *s_searchterm, // the not case sensitive key you are looking for , for example "NAXES" or "naxes"
    // char * s_header_comment, // this variable will be filled with the header comment, if not existing it will be empty
    char *a_buffer,      // the actual buffer of the fits file,
    long n_buffer_length, // the length of the buffer]
    char ** s_header_line, // will be filled with 'prop = val / comment'
    char ** s_header_prop, // will be filled with 'prop'
    char ** s_header_val, // will be filled with 'val'
    char ** s_header_comment // will be filled with 'comment'
)
{

    char * s_prop_value = malloc(1);
    char * s_header_string = f_get_header_string(
        a_buffer,
        n_buffer_length
    );

    int n_i = 0;
    printf("address of *s_header_prop %p\n", *s_header_prop);
    *s_header_line = realloc(*s_header_line, N_FITS_HEADER_MAX_LINE_LENGTH+1); 
    *s_header_prop = realloc(*s_header_prop, N_FITS_HEADER_MAX_LINE_LENGTH+1); 
    *s_header_val = realloc(*s_header_val, N_FITS_HEADER_MAX_LINE_LENGTH+1); 
    *s_header_comment = realloc(*s_header_comment, N_FITS_HEADER_MAX_LINE_LENGTH+1); 

    printf("address of *s_header_prop %p\n", *s_header_prop);

    // char * s_index_of_search = strcasestr(s_header_string_with_linebreaks, s_searchterm); 
    
    printf("warning: the function f_get_header_prop_val_comment is case sensitive\n"); 

    char * s_index_of_search = strstr(s_header_string, s_searchterm); 
    
    if(s_index_of_search == NULL){
        printf("property %s was not found in fits header\n", s_searchterm);
        return NULL;
    }

    // strncpy(s_header_string, *s_header_line,79); 
    memcpy(*s_header_line, s_index_of_search, N_FITS_HEADER_MAX_LINE_LENGTH);
    *s_header_line[N_FITS_HEADER_MAX_LINE_LENGTH] = '\0';

    long n_index_of_searchterm = f_n_index_of(*s_header_line, s_searchterm);
    char * s_equal_identifier = "=";
    char * s_slash_identifier = "/";
    int n_len_s_equal_identifier = strlen(s_equal_identifier);
    int n_len_s_slash_identifier = strlen(s_slash_identifier);

    long n_index_of_equal = f_n_index_of(*s_header_line, s_equal_identifier);
    long n_index_of_slash = f_n_index_of(*s_header_line, s_slash_identifier);
    
    // printf("index of =: %li , /: %li ", n_index_of_equal, n_index_of_slash);
    long n_end_index_value; 
    if(n_index_of_slash == -1){
        n_end_index_value = N_FITS_HEADER_MAX_LINE_LENGTH;
    }else{
        n_end_index_value = n_index_of_slash; 
    }
    memcpy(*s_header_prop, *s_header_line, strlen(s_searchterm));
    memcpy(*s_header_val, *s_header_line+n_index_of_equal+n_len_s_equal_identifier, n_end_index_value-n_index_of_equal-n_len_s_equal_identifier);
    
    if(n_index_of_slash != -1){
        memcpy(*s_header_comment, *s_header_line+n_index_of_slash+n_len_s_slash_identifier, N_FITS_HEADER_MAX_LINE_LENGTH-n_index_of_slash-n_len_s_slash_identifier);
    }

    *s_header_prop = trim_string(*s_header_prop);
    *s_header_val = trim_string(*s_header_val);
    *s_header_comment = trim_string(*s_header_comment);

    // char * *s_header_prop_trimmed = trimString(*s_header_prop);
    // char * *s_header_val_trimmed = trimString(*s_header_val);
    // char * *s_header_comment_trimmed = trimString(*s_header_comment);

    // memset(*s_header_prop, 0, N_FITS_HEADER_MAX_LINE_LENGTH);
    // memset(*s_header_val, 0, N_FITS_HEADER_MAX_LINE_LENGTH);
    // memset(*s_header_comment, 0, N_FITS_HEADER_MAX_LINE_LENGTH);
    // printf("*s_header_prop_trimmed %s\n", *s_header_prop_trimmed);


    // memcpy(*s_header_prop, *s_header_prop_trimmed, strlen(*s_header_prop_trimmed));
    // memcpy(*s_header_val, *s_header_val_trimmed, strlen(*s_header_val_trimmed));
    // memcpy(*s_header_comment, *s_header_comment_trimmed, strlen(*s_header_comment_trimmed));


    printf("*s_header_prop %s\n", *s_header_prop);
    printf("*s_header_val %s\n", *s_header_val);
    printf("*s_header_comment %s\n", *s_header_comment);


    printf("*s_header_prop address %p\n", *s_header_prop);
    printf("*s_header_val address %p\n", *s_header_val);
    printf("*s_header_comment address %p\n", *s_header_comment);

    // printf("\n%s\n", s_header_line);

    free(s_header_string);

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


int do_stuff()
{
    char *s_path_file_name_input = "./../../../2022-03-07T21-09-10_Coordinates_FILTER_30s_Severin-W.fts";
    char *s_path_file_name_output = "./output_image_from_c.fts";
    long n_buffer_length;
    char * a_buffer;

    f_open_fits(
        s_path_file_name_input,
        &a_buffer,
        &n_buffer_length);
    


    long n_i = 0;
    while (n_i < n_buffer_length)
    {


        // printf("n_i is %li \n", n_i);
        // printf("buffer[%li] is %x \n", buffer[n_i], n_i);
        if (n_i > 100000)
        {
            if (n_i % 2 == 0)
            {
                // convert into 16 bit value
                unsigned int n_16bits = a_buffer[n_i];
                n_16bits = n_16bits << 8;
                n_16bits = n_16bits | a_buffer[n_i + 1];
                // manipulate 16 bit value
                n_16bits = (int)n_16bits * 1;
                // convert into 2 values, each 8 bit
                unsigned int n_8bit_high = n_16bits >> 8;
                unsigned int n_8bit_low = n_16bits & 0b11111111;
                // a_buffer[n_i] = n_8bit_high;
                // a_buffer[n_i+1] = n_8bit_low;
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
        fwrite(a_buffer, n_buffer_length, 1, pFile);
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
    printf(".fits has \n%li bytes\n", n_buffer_length);
    // printf("test");


    // char * s_header_string = f_get_header_string(
    //     a_buffer, 
    //     n_buffer_length
    // );
    // printf("header string is:\n %s \n", s_header_string);

    // char * s_header_string_with_linebreaks = f_get_header_string_with_linebreaks(
    //     a_buffer, 
    //     n_buffer_length
    // );    
    // printf("header string with linebreaks is:\n %s \n", s_header_string_with_linebreaks);

    // get single prop value from header 
    char *s_searchterm = "NAXIS1";

    char * s_header_line = malloc(N_FITS_HEADER_MAX_LINE_LENGTH);
    char * s_header_prop = malloc(N_FITS_HEADER_MAX_LINE_LENGTH);
    char * s_header_val = malloc(N_FITS_HEADER_MAX_LINE_LENGTH);
    char * s_header_comment = malloc(N_FITS_HEADER_MAX_LINE_LENGTH);
    printf("address of s_header_prop %p\n", s_header_prop);

    f_get_header_line_prop_val_comment(
        s_searchterm,
        a_buffer,
        n_buffer_length, 
        &s_header_line,
        &s_header_prop, 
        &s_header_val, 
        &s_header_comment);
    printf("%s: s_header_prop:  %s\n",s_searchterm, s_header_prop);
    printf("%s: s_header_val:  %s\n",s_searchterm, s_header_val);
    printf("%s: s_header_comment:  %s\n",s_searchterm, s_header_comment);


    printf("s_header_prop address %p\n", s_header_prop);
    printf("s_header_val address %p\n", s_header_val);
    printf("s_header_comment address %p\n", s_header_comment);

    printf("%s: s_header_prop:  %s\n",s_searchterm, s_header_prop);
    printf("%s: s_header_val:  %s\n",s_searchterm, s_header_val);
    printf("%s: s_header_comment:  %s\n",s_searchterm, s_header_comment);
    
    printf("address of s_header_prop %p\n", s_header_prop);

}
// printf("buffer is %s", buffer);
// start to process your data / extract strings here...

int main()
{
    do_stuff();

    // f_char_pointer_test();
    return 0;
}