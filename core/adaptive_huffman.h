#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef COMPRESSO_ADAPTIVE_HUFFMAN_H
#define COMPRESSO_ADAPTIVE_HUFFMAN_H

typedef struct Node {
    struct Node * left_child;
    struct Node * right_child;
    struct Node * parent;

    int weight;
    int identifier;
    long int symbol;
} Node;

typedef struct {
    struct Node ** nodes;
    int length;
} Queue_Stack;

typedef struct {
    int * arr;
    int len;
} array;

#endif //COMPRESSO_ADAPTIVE_HUFFMAN_H
