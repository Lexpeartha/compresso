//
// Created by harry on 4/24/23.
//

#ifndef SPECIJALNAGRUPA_MAIN_H
#define SPECIJALNAGRUPA_MAIN_H

#define MAX_LENGTH 500
#define MAX_CHILDREN 100

typedef struct {
    int frequency;
    int data;
    struct HeapNode * left_child;
    struct HeapNode * right_child;
} HeapNode;

typedef struct {
    int data;
    int huffman_code;
} Dictionary;

typedef struct {
    HeapNode * nodes;
    int len;
} Heap;

typedef struct {
    int * arr;
    int len;
} array;



#endif //SPECIJALNAGRUPA_MAIN_H
