#ifndef SPECIJALNAGRUPA_MAIN_H
#define SPECIJALNAGRUPA_MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdint.h>
#include <unistd.h>
#include "uthash.h"
#define MAX_HUFFMAN_CODE_LEN 100000

typedef struct {
    long int frequency;
    char data;
    struct HeapNode * left_child;
    struct HeapNode * right_child;
} HeapNode;

typedef struct {
    HeapNode ** nodes;
    int len;
} Heap;

typedef struct {
    char symbol;
    long int frequency;
    UT_hash_handle hh;
} hash_entry;

typedef struct {
    char symbol;
    char * huffman;
    UT_hash_handle hh;
} output_hash;

typedef struct {
    unsigned char byte;
    short int index;
} Byte_buffer;

void add_output_hash_char(char symbol, char * string, output_hash ** table);
#endif //SPECIJALNAGRUPA_MAIN_H
