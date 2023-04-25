//
// Created by aleksa on 4/24/23.
//

#include "lzw.h"

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct{
    unsigned int prefix;
    unsigned int code;
    unsigned int new_char;
}dict_element;

typedef struct{
    dict_element *table;
    int len;

}dictionary;

int is_in_dict(dictionary *dict, unsigned int prefix, unsigned int new_char){
    for(int i = 0;i < dict->len;i++){
        if(dict->table[i].prefix == prefix && dict->table[i].new_char == new_char)
            return dict->table[i].code;
    }
    return -1;
}

void add_to_dict(dictionary *dict, unsigned int prefix, unsigned int new_char){
    dict->len++;
    dict->table = (dict_element*) realloc(dict->table, dict->len * sizeof(dict_element));
    dict->table[dict->len-1].code = dict->len-1;
    dict->table[dict->len-1].new_char = new_char;
    dict->table[dict->len-1].prefix = prefix;
}




int* compress_lzw(char* inputFileName){
    dictionary *dict;
    dict = (dictionary*)malloc(sizeof(dictionary));
    dict->table = (dict_element*)malloc(256*sizeof(dict_element));
    dict->len = 256;

    for(int i = 0;i < dict->len;i++){
        dict->table[i].code = i;
        dict->table[i].prefix = -1;
        dict->table[i].new_char = i;
    }


    FILE *fp = fopen(inputFileName, "rb");
    int *buffer = malloc(sizeof(int));
    int buffer_counter = 0;
    long file_size;
    if(fp == NULL) {
        printf("ERROR OPENING FILE\n");
        return;
    }

    int string = fgetc(fp);
    int character = 0;
    while(string != EOF && character != EOF){
        character = fgetc(fp);
        int tmp = is_in_dict(dict, string, character);
        if(tmp != -1)
            string = tmp;
        else{
            buffer_counter++;
            buffer[buffer_counter-1] = string;
            buffer = realloc(buffer, buffer_counter*sizeof(int));
            add_to_dict(dict, string, character);
            string = character;
        }
    }



    return buffer;
}

