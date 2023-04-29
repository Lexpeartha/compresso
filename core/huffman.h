//
// Created by harry on 4/24/23.
//

#ifndef SPECIJALNAGRUPA_MAIN_H
#define SPECIJALNAGRUPA_MAIN_H

#define MAX_LENGTH 500
#define MAX_CHILDREN 100

typedef struct {
    struct Node * parent;
    struct Node * left_child;
    struct Node * right_child;

    int data;
    int frequency;
} Node;

typedef struct {
    int * arr;
    int len;
} array;

typedef struct {
    int * arr;
    int * data;
    int len;
} Heap;

#endif //SPECIJALNAGRUPA_MAIN_H
