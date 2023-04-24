#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"


void adaptiveHuffman(char rawInput[], int * rawInputLength){
    struct Node * tree = malloc(sizeof(struct Node));

    tree->parent = NULL;
    tree->childrenCount = 0;

    for(int i = 0; i < *rawInputLength; i++){

    }
}

int huffmanStart() {

    char rawInput[MAX_LENGTH];
    int rawInputLength = 0;
    struct Node priorityQueue[MAX_LENGTH];

    // testiranje, obrisati posle
    strcpy(rawInput, "ABCAAABCABCD");
    rawInputLength = strlen(rawInput);

    adaptiveHuffman(rawInput, &rawInputLength);

    return 0;
}