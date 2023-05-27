#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef COMPRESSO_ADAPTIVE_HUFFMAN_H
#define COMPRESSO_ADAPTIVE_HUFFMAN_H

typedef struct Node {
    struct Node * left_child;
    struct Node * right_child;
    struct Node * parent;

    long int weight;
    long int identifier;
    char symbol;
} Node;

typedef struct {
    struct Node ** nodes;
    int length;
} Queue_Stack;

typedef struct {
    int * arr;
    int len;
} array;

typedef struct {
    unsigned char byte;
    short int index;
} Byte_buffer;

#endif //COMPRESSO_ADAPTIVE_HUFFMAN_H