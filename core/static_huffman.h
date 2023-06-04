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


int heap_left_child(int index);
int heap_right_child(int index);
void swap_heap_nodes(Heap * heap, int ind1, int ind2);
void insert_value(Heap * heap, HeapNode * new);
Heap * form_min_heap(hash_entry * table);
int node_is_leaf(HeapNode * node);
void heapify(Heap * heap, int index);
HeapNode * extract_min(Heap * heap);
HeapNode * form_huffman_tree(Heap * heap);
void print_byte_buffer2(Byte_buffer * byte_buffer);
void fill_byte_buffer2(Byte_buffer * byte_buffer, const char * string);
char * stringify(const int array[], int count);
void push_stack(Stack * stack, StackNode temp);
StackNode pop_stack(Stack * stack);
int empty(Stack * stack);
StackNode makeStackNode(HeapNode* n, int c, int cd[]);
void get_huffman_codes(HeapNode* node, int counter, int code[]);
void add_char(uint8_t symbol, hash_entry ** table);
hash_entry * find_char(uint8_t symbol, hash_entry ** table);
char how_many_useful_bits2(Byte_buffer * byte_buffer);
void add_output_hash_char(uint8_t symbol, char * string, output_hash ** table);
char * huffman_code_hash(uint8_t symbol, output_hash ** table);
void print_hash(hash_entry ** table);
void export_hash_table(hash_entry * hashTable, const char* filename);
hash_entry * import_hash_table(const char* filename);
void free_hash_table(output_hash * table);
uint8_t navigate2(HeapNode * root, uint8_t * buffer, int buffer_len);
int static_huffman_decode(char * filename);
int static_huffman_encode(char * filename);
#endif //SPECIJALNAGRUPA_MAIN_H
