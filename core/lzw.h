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
#include"uthash.h"

typedef struct{
    int *arr;
    int len;
}array;
typedef struct{
    int prefix;
    int new_char;
}dict_element;
typedef struct{
    dict_element *table;
    int len;
    int allocated;

}dictionary;
typedef struct {
    char *key;
    int value;
    UT_hash_handle hh;
} hash_entry;

void add_entry(char *key, int value);
int find_entry(char *key);
void free_hash_table();
char* make_key(int value1, int value2);
void generate_random_filename(char* filename_buffer);
void free_dictionary(dictionary *dict) ;
void free_array(array *a);
void add_to_dict(dictionary *dict, int prefix, int new_char);
int return_prefix(dictionary *dict, int code);
int return_new_char(dictionary *dict, int code);
array* print_string(dictionary *dict, int code);
int return_first(dictionary *dict, int code);
char* compress_lzw(char* input_file_name);
void decompress_lzw(char* input_file_name, char *output_file_name);
#endif //COMPRESSO_LZW_H
