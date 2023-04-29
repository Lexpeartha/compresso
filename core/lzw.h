//
// Created by aleksa on 4/24/23.
//

#ifndef COMPRESSO_LZW_H
#define COMPRESSO_LZW_H
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdint.h>
#include <unistd.h>


typedef struct{
    long int *arr;
    long int len;
}array;
typedef struct{
    long int prefix;
    long int code;
    long int new_char;
}dict_element;
typedef struct{
    dict_element *table;
    long int len;

}dictionary;

void generate_random_filename(char* filename_buffer);
void free_dictionary(dictionary *dict);
void free_array(array *a);
long int is_in_dict(dictionary *dict, long int prefix, long int new_char);
void add_to_dict(dictionary *dict,  long int prefix, long int new_char);
long int return_prefix(dictionary *dict, long int code);
long int return_new_char(dictionary *dict, long int code);
array* print_string(dictionary *dict, long int code);
long int return_first(dictionary *dict, long int code);
char* compress_lzw(char* input_file_name);
void decompress(char* input_file_name, char *output_file_name);
#endif //COMPRESSO_LZW_H
