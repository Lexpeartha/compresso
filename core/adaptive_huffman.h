#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef COMPRESSO_ADAPTIVE_HUFFMAN_H
#define COMPRESSO_ADAPTIVE_HUFFMAN_H

typedef struct {
    struct Node * left_child;
    struct Node * right_child;
    struct Node * parent;
    int weight;
    int identifier;
    char symbol;
} Node;

typedef struct {
    int * arr;
    int len;
} array;

#endif //COMPRESSO_ADAPTIVE_HUFFMAN_H
