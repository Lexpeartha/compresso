#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"

int heap_parent(int index){
    return (index - 1) / 2;
}

int heap_left_child(int index){
    return (index * 2) + 1;
}

int heap_right_child(int index){
    return (index * 2) + 2;
}

void swap_heap_nodes(Heap * heap, int * ind){
    int index = *ind;
    int new_index = heap_parent(index);
    int temp1 = heap->arr[index];
    int temp2 = heap->data[index];
    heap->arr[index] = heap->arr[new_index];
    heap->data[index] = heap->data[new_index];
    heap->arr[new_index] = temp1;
    heap->data[new_index] = temp2;

    *ind = new_index;
}

void insert_value(Heap * heap, int frequency, int datapoint){
    // unos nove vrednosti
    int new_element_index = heap->len;
    heap->len++;
    heap->arr[new_element_index] = frequency;
    heap->data[new_element_index] = datapoint;

    // heap-ifikovanje
    int index = new_element_index;
    while((heap->len != 0 ) && (heap->arr[heap_parent(index)] > heap->arr[index])){
        swap_heap_nodes(heap, &index);
    }

}

Heap * form_min_heap(array * frequencies, array * data) {
    // inicijalizovanje heap-a
    Heap * heap = malloc(sizeof(Heap));
    heap->arr = malloc(sizeof(int));
    heap->data = malloc(sizeof(int));
    heap->len = 0;

    // formiranje heap-a
    for (int i = 0; i < frequencies->len; i++) {
        insert_value(heap, frequencies->arr[i], data->arr[i]);
    }

    return heap;
}

int extract_min(Heap * heap){

}

Node * static_huffman(Heap * heap){

}

void print_heap(Heap * heap){
    for(int i = 0; i < heap->len; i++){
        printf("%d (%c) ", heap->arr[i], heap->data[i]);
    }
}

int main() {

    /* ZA TESTIRANJE. OBRISATI KASNIJE */
    array * test;
    test = malloc(sizeof(array));
    test->arr = malloc(sizeof(int));
    test->arr[0] = 15;
    test->arr[1] = 13;
    test->arr[2] = 2;
    test->arr[3] = 3;
    test->arr[4] = 4;
    test->arr[5] = 9;
    test->len = 6;

    array * characters = malloc(sizeof(array));
    characters->arr = malloc(sizeof(int));
    characters->arr[0] = 'a';
    characters->arr[1] = 'b';
    characters->arr[2] = 'c';
    characters->arr[3] = 'd';
    characters->arr[4] = 'e';
    characters->arr[5] = 'f';
    characters->len = 6;
    /* ZA TESTIRANJE. OBRISATI KASNIJE */

    Heap * min_heap = form_min_heap(test, characters);
    print_heap(min_heap);

    Node * static_huffman_root = static_huffman(min_heap);

    return 0;
}