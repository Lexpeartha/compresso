#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef COMPRESSO_ADAPTIVE_HUFFMAN_H
#define COMPRESSO_ADAPTIVE_HUFFMAN_H

typedef struct {
    int weight;
    int identifier;
    char symbol;
} Node;

typedef struct {
    Node * nodes;
    int len;
} Tree;

typedef struct {
    int * arr;
    int len;
} array;

#endif //COMPRESSO_ADAPTIVE_HUFFMAN_H
