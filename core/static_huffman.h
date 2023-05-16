#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    HeapNode * nodes;
    int len;
} Heap;

typedef struct {
    int * arr;
    int len;
} array;



#endif //SPECIJALNAGRUPA_MAIN_H
