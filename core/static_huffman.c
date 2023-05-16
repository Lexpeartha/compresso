#include "static_huffman.h"

/*
 * TO-DO:
 *
 * - obrati pažnju na dinamičku alokaciju (naročito kod formiranja min heap-a)
 * - pazi na memory leak-ove! Oslobodi memoriju koju zauzimaju promenljive
 * - uradi čitanje iz binarnog fajla i ispis u binarni fajl
 * - napravi rečnik za parove karakter - huffman kod
 * - error handling?
 *
 */



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

    // Funkcija koja zamenjuje dva čvora u heap-u.
    // Dostavlja se indeks DETETA, i ono se menja sa svojim roditeljem.

    int index = *ind;
    int new_index = heap_parent(index);

    int temp1 = heap->nodes[index].frequency;
    int temp2 = heap->nodes[index].data;
    struct HeapNode * temp3 = heap->nodes[index].left_child;
    struct HeapNode * temp4 = heap->nodes[index].right_child;

    heap->nodes[index].frequency = heap->nodes[new_index].frequency;
    heap->nodes[index].data = heap->nodes[new_index].data;
    heap->nodes[index].left_child = heap->nodes[new_index].left_child;
    heap->nodes[index].right_child = heap->nodes[new_index].right_child;

    heap->nodes[new_index].frequency = temp1;
    heap->nodes[new_index].data = temp2;
    heap->nodes[new_index].left_child = temp3;
    heap->nodes[new_index].right_child = temp4;

    *ind = new_index;
}

// unos vrednosti iz fajla
void insert_value(Heap * heap, int frequency, int datapoint, struct HeapNode * left, struct HeapNode * right){
    int new_element_index = heap->len;
    heap->len++;
    heap->nodes[new_element_index].frequency = frequency;
    heap->nodes[new_element_index].data = datapoint;
    heap->nodes[new_element_index].left_child = left;
    heap->nodes[new_element_index].right_child = right;

    // heap-ifikovanje
    int index = new_element_index;
    while((heap->len != 0 ) && (heap->nodes[heap_parent(index)].frequency > heap->nodes[index].frequency)){
        swap_heap_nodes(heap, &index);
    }

}

Heap * form_min_heap(array * frequencies, array * data) {
    // inicijalizovanje heap-a
    Heap * heap = malloc(sizeof(Heap));
    heap->nodes = malloc(data->len * sizeof(HeapNode));
    heap->len = 0;

    // formiranje heap-a
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

    // da li treba promeniti redosled?
    if(minimum != index){
        // da
        int * throwaway = &minimum;
        swap_heap_nodes(heap, throwaway);
        heapify(heap, minimum);
    }
}

HeapNode * extract_min(Heap * heap){

    // Vrati podatke korena heap-a

    // inicijalizovanje
    HeapNode * returning = malloc(sizeof(HeapNode));
    returning->frequency = heap->nodes[0].frequency;
    returning->data = heap->nodes[0].data;
    returning->left_child = heap->nodes[0].left_child;
    returning->right_child = heap->nodes[0].right_child;

    // provera
    // printf("\n%d (%c) ", returning->frequency, returning->data);
    //

    // pomeranje poslednjeg elementa na koren
    heap->nodes[0].frequency = heap->nodes[heap->len - 1].frequency;
    heap->nodes[0].data = heap->nodes[heap->len - 1].data;
    heap->nodes[0].left_child = heap->nodes[heap->len - 1].left_child;
    heap->nodes[0].right_child = heap->nodes[heap->len - 1].right_child;
    heap->len--;

    // Ovo izaziva segmentacionu grešku...
    // heap = realloc(heap, heap->len * sizeof(HeapNode));

    heapify(heap, 0);

    return returning;
}

HeapNode * execute_static_huffman(Heap * heap){
    while(heap->len > 1){
        HeapNode * left = extract_min(heap);
        HeapNode * right = extract_min(heap);

        // ako je levi čvor list, a desni nije, moraju da se zamene
        if(node_is_leaf(left) && !node_is_leaf(right)){
            HeapNode * temp = left;
            left = right;
            right = temp;
        }

        HeapNode * internal_node = malloc(sizeof(HeapNode));
        internal_node->left_child = (struct HeapNode *) left;
        internal_node->right_child = (struct HeapNode *) right;
        internal_node->data = 0; // default vrednost koja ukazuje da je ovo INTERNAL NODE
        internal_node->frequency = left->frequency + right->frequency;

        insert_value(heap, internal_node->frequency, internal_node->data, internal_node->left_child, internal_node->right_child);
    }

    return extract_min(heap);
}

int get_huffman_codes(HeapNode * node, int counter, int code[]){


    if(node->left_child != NULL){
        code[counter] = 0;
        get_huffman_codes((HeapNode *) node->left_child, counter + 1, code);
    }
    if(node->right_child != NULL){
        code[counter] = 1;
        get_huffman_codes((HeapNode *) node->right_child, counter + 1, code);
    }
    if(node_is_leaf(node)){
        printf("Huffman code for %c is: ", node->data);
        for(int i = 0; i < counter; i++){
            printf("%d", code[i]);
        }
        printf("\n");
    }
}

void print_heap(Heap * heap){
    for(int i = 0; i < heap->len; i++){
        printf("%d (%c) ", heap->nodes[i].frequency, heap->nodes[i].data);
    }
}

int static_huffman_start() {

    /* ZA TESTIRANJE. OBRISATI KASNIJE */
    array * test;
    test = malloc(sizeof(array));
    test->arr = malloc( 6 * sizeof(int));
    test->arr[0] = 15;
    test->arr[1] = 13;
    test->arr[2] = 2;
    test->arr[3] = 3;
    test->arr[4] = 4;
    test->arr[5] = 9;
    test->len = 6;

    array * characters = malloc(sizeof(array));
    characters->arr = malloc(6 * sizeof(int));
    characters->arr[0] = 'a';
    characters->arr[1] = 'b';
    characters->arr[2] = 'c';
    characters->arr[3] = 'd';
    characters->arr[4] = 'e';
    characters->arr[5] = 'f';
    characters->len = 6;
    /* ZA TESTIRANJE. OBRISATI KASNIJE */


    // čitanje iz fajla
//    char * file_name = malloc(100 * sizeof(char));
//    printf("Input file name: ");
//    scanf("%s", file_name);
//    if(file_name[strlen(file_name) - 1] == '\n'){
//        file_name[strlen(file_name) - 1] = 0;
//    }
//
//    long int file_line;
//    FILE * in;
//    FILE * out;
//
//    out = fopen("n.bin", "wb");
//    if(out == NULL){
//        return 1;
//    }
//
//    char primer[5] = {'a', 'b', 'c', 'd', '\0'};
//
//    fwrite(&primer, sizeof(char), 1, out);

//    fread(&tmp, sizeof(long int), 1, fp);

    Heap * min_heap = form_min_heap(test, characters);

    //Provera da je min heap dobro napravljen
    //print_heap(min_heap);

    HeapNode * static_huffman_root = execute_static_huffman(min_heap);

    int throwaway[5000];
    int counter = 0;
    get_huffman_codes(static_huffman_root, counter, throwaway);

    return 0;
}