#include "static_huffman.h"

int heap_parent(int index){
    return (index - 1) / 2;
}

int heap_left_child(int index){
    return (index * 2) + 1;
}

int heap_right_child(int index){
    return (index * 2) + 2;
}

// A function that swaps two nodes in the heap
// Arguments: indices of the two nodes that need to be swapped
void swap_heap_nodes(Heap * heap, int ind1, int ind2){

    long int temp1 = heap->nodes[ind1].frequency;
    long int temp2 = heap->nodes[ind1].data;
    struct HeapNode * temp3 = heap->nodes[ind1].left_child;
    struct HeapNode * temp4 = heap->nodes[ind1].right_child;

    heap->nodes[ind1].frequency = heap->nodes[ind2].frequency;
    heap->nodes[ind1].data = heap->nodes[ind2].data;
    heap->nodes[ind1].left_child = heap->nodes[ind2].left_child;
    heap->nodes[ind1].right_child = heap->nodes[ind2].right_child;

    heap->nodes[ind2].frequency = temp1;
    heap->nodes[ind2].data = temp2;
    heap->nodes[ind2].left_child = temp3;
    heap->nodes[ind2].right_child = temp4;
}


void insert_value(Heap * heap, long int frequency, long int datapoint, struct HeapNode * left, struct HeapNode * right){
    heap->len++;

    HeapNode new;
    new.frequency = frequency;
    new.data = datapoint;
    new.left_child = left;
    new.right_child = right;

    int i = heap->len - 1;

    while (i && new.frequency < heap->nodes[(i - 1) / 2].frequency) {
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->nodes[i] = new;
}

Heap * form_min_heap(array * frequencies, array * data) {
    // initializing the heap
    Heap * heap = malloc(sizeof(Heap));
    heap->nodes = malloc(data->len * sizeof(HeapNode));
    heap->len = 0;

    // forming the heap
    for (int i = 0; i < frequencies->len; i++) {
        insert_value(heap, frequencies->arr[i], data->arr[i], NULL, NULL);
    }

    return heap;
}

int node_is_leaf(HeapNode * node){
    if (!node->left_child && !node->right_child) return 1;
    else return 0;
}

void heapify(Heap * heap, int index){

    int left_index = heap_left_child(index);
    int right_index = heap_right_child(index);

    int minimum = index;

    if(heap->nodes[left_index].frequency < heap->nodes[minimum].frequency && left_index < heap->len){
        minimum = left_index;
    }
    if(heap->nodes[right_index].frequency < heap->nodes[minimum].frequency && right_index < heap->len){
        minimum = right_index;
    }

    // change the order?
    if(minimum != index){
        // yes
        swap_heap_nodes(heap, index, minimum);
        heapify(heap, minimum);
    }
}

HeapNode * extract_min(Heap * heap){

    // initialization
    HeapNode * returning = malloc(sizeof(HeapNode));
    returning->frequency = heap->nodes[0].frequency;
    returning->data = heap->nodes[0].data;
    returning->left_child = heap->nodes[0].left_child;
    returning->right_child = heap->nodes[0].right_child;


    // last element becomes root
    heap->nodes[0].frequency = heap->nodes[heap->len - 1].frequency;
    heap->nodes[0].data = heap->nodes[heap->len - 1].data;
    heap->nodes[0].left_child = heap->nodes[heap->len - 1].left_child;
    heap->nodes[0].right_child = heap->nodes[heap->len - 1].right_child;

    // elimination of hole left by extraction of node
    heap->len--;

    heapify(heap, 0);

    return returning;
}

HeapNode * execute_static_huffman(Heap * heap){
    while(heap->len > 1){
        HeapNode * left = extract_min(heap);
        HeapNode * right = extract_min(heap);

        HeapNode * internal_node = malloc(sizeof(HeapNode));
        internal_node->left_child = (struct HeapNode *) left;
        internal_node->right_child = (struct HeapNode *) right;
        internal_node->data = 0; // default value indicating this is an INTERNAL NODE
        internal_node->frequency = left->frequency + right->frequency;

        // the newly formed internal node is returned to the heap
        insert_value(heap, internal_node->frequency, internal_node->data, internal_node->left_child, internal_node->right_child);
    }

    return extract_min(heap);
}

void get_huffman_codes(HeapNode * node, int counter, int  code[]){

    if(node->left_child != NULL){
        code[counter] = 0;
        get_huffman_codes((HeapNode *) node->left_child, counter + 1, code);
    }
    if(node->right_child != NULL){
        code[counter] = 1;
        get_huffman_codes((HeapNode *) node->right_child, counter + 1, code);
    }
    if(node_is_leaf(node)){
        printf("\nHuffman code for %c is: ", node->data);
        for(int i = 0; i < counter; i++){
            printf("%d", code[i]);
        }
        //counter = 0;
    }
}

// static_huffman_encode
int static_huffman_encode() {
    char * filename = "decompressed.bin";
    FILE * in;
    in = fopen(filename, "rb");
    if(in == NULL){
        printf("Can't open file.");
        exit(1);
    }
    long int data;

    Dictionary * dictionary = malloc(sizeof(Dictionary));
    dictionary->keys = malloc(1000 * sizeof(long int));
    dictionary->frequencies = malloc(1000 * sizeof(long int));
    dictionary->huffman_codes = malloc(1000 * sizeof(long int));
    dictionary->len = 0;


    // reading from a binary file
    while(fread(&data, sizeof(long int), 1, in) == 1){
        int found = 0;
        if(dictionary->len == 0){
            dictionary->keys[0] = data;
            dictionary->frequencies[0] = 1;
            dictionary->len++;
        }
        else {
            for(int i = 0; i < dictionary->len; i++){
                if(dictionary->keys[i] == data){
                    dictionary->frequencies[i]++;
                    found = 1;
                }
            }
            if(!found){
                dictionary->keys[dictionary->len] = data;
                dictionary->frequencies[dictionary->len] = 1;
                dictionary->len++;
            }
        }

    }

    fclose(in);

    array * frequencies = malloc(sizeof(array));
    array * chars = malloc(sizeof(array));
    frequencies->len = dictionary->len;
    chars->len = dictionary->len;
    frequencies->arr = malloc(frequencies->len * sizeof(long int));
    chars->arr = malloc(chars->len * sizeof(long int));
    frequencies->arr = dictionary->frequencies;
    chars->arr = dictionary->keys;

    Heap * min_heap = form_min_heap(frequencies, chars);

    HeapNode * static_huffman_root = execute_static_huffman(min_heap);

    int throwaway[5000];
    int counter = 0;
    get_huffman_codes(static_huffman_root, counter, throwaway);

    return 0;
}