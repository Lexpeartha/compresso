#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef SPECIJALNAGRUPA_MAIN_H
#define SPECIJALNAGRUPA_MAIN_H


typedef struct {
    long int frequency;
    long int data;
    struct HeapNode * left_child;
    struct HeapNode * right_child;
} HeapNode;

typedef struct {
    HeapNode * nodes;
    int len;
} Heap;

typedef struct {
    long int * arr;
    long int len;
} array;

typedef struct {
    long int * keys;
    long int * frequencies;
    long int * huffman_codes;
    int len;
} Dictionary;

#endif //SPECIJALNAGRUPA_MAIN_H
