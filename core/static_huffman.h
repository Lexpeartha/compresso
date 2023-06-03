#ifndef SPECIJALNAGRUPA_MAIN_H
#define SPECIJALNAGRUPA_MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdint.h>
#include <unistd.h>
#include "uthash.h"
#define MAX_HUFFMAN_CODE_LEN 500
#define MAX_STACK_SIZE 256

typedef struct {
    long int frequency;
    uint8_t data;
    struct HeapNode * left_child;
    struct HeapNode * right_child;
} HeapNode;

typedef struct {
    HeapNode ** nodes;
    int len;
} Heap;

typedef struct {
    uint8_t symbol;
    long int frequency;
    UT_hash_handle hh;
} hash_entry;

typedef struct {
    uint8_t symbol;
    char * huffman;
    UT_hash_handle hh;
} output_hash;

typedef struct {
    uint8_t byte;
    short int index;
} Byte_buffer;

typedef struct  {
    HeapNode* node;
    int counter;
    int code[MAX_HUFFMAN_CODE_LEN];
} StackNode;

typedef struct {
    StackNode data[MAX_STACK_SIZE];
    int top;
} Stack;

void add_output_hash_char(uint8_t symbol, char * string, output_hash ** table);
#endif //SPECIJALNAGRUPA_MAIN_H
