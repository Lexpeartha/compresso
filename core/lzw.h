//
// Created by aleksa on 4/24/23.
//

#ifndef COMPRESSO_LZW_H
#define COMPRESSO_LZW_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<string.h>



typedef struct{
    int *arr;
    int len;
} array;

typedef struct{
    unsigned int prefix;
    unsigned int code;
    unsigned int new_char;
} dict_element;

typedef struct{
    dict_element *table;
    int len;
} dictionary;

int write_binary_file(const uint8_t *filename, const uint8_t *content, size_t size);

int is_in_dict(dictionary *dict, unsigned int prefix, unsigned int new_char);

void add_to_dict(dictionary *dict, unsigned int prefix, unsigned int new_char);

int return_prefix(dictionary *dict, unsigned int code);

int return_new_char(dictionary *dict, unsigned int code);

array* print_string(dictionary *dict, unsigned int code);

int return_first(dictionary *dict, unsigned int code);

void array_extend(array *arr1, array *arr2);

unsigned char* read_binary_file(const char* filename, size_t* size);

array* compress_lzw(char* input_file_name);

#endif //COMPRESSO_LZW_H
