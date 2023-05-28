#include "static_huffman.h"
#define OUTPUT "static_output"

hash_entry * hash_table = NULL;
output_hash * output_hash_table = NULL;

int heap_left_child(int index){
    return (index * 2) + 1;
}

int heap_right_child(int index){
    return (index * 2) + 2;
}

// A function that swaps two nodes in the heap
// Arguments: indices of the two nodes that need to be swapped
void swap_heap_nodes(Heap * heap, int ind1, int ind2){

    long int temp1 = heap->nodes[ind1]->frequency;
    char temp2 = heap->nodes[ind1]->data;
    struct HeapNode * temp3 = heap->nodes[ind1]->left_child;
    struct HeapNode * temp4 = heap->nodes[ind1]->right_child;

    heap->nodes[ind1]->frequency = heap->nodes[ind2]->frequency;
    heap->nodes[ind1]->data = heap->nodes[ind2]->data;
    heap->nodes[ind1]->left_child = heap->nodes[ind2]->left_child;
    heap->nodes[ind1]->right_child = heap->nodes[ind2]->right_child;

    heap->nodes[ind2]->frequency = temp1;
    heap->nodes[ind2]->data = temp2;
    heap->nodes[ind2]->left_child = temp3;
    heap->nodes[ind2]->right_child = temp4;
}


void insert_value(Heap * heap, HeapNode * new){

    heap->len++;

    int i = heap->len - 1;

    while (i && new->frequency < heap->nodes[(i - 1) / 2]->frequency) {
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->nodes[i] = new;
}

Heap * form_min_heap() {
    // initializing the heap
    Heap * heap = malloc(sizeof(Heap));
    heap->nodes = malloc(HASH_COUNT(hash_table) * sizeof(HeapNode *));
    heap->len = 0;

    hash_entry * s;
    for (s = hash_table; s != NULL; s = s->hh.next) {
        HeapNode * new = malloc(sizeof(HeapNode));
        new->frequency = s->frequency;
        new->data = s->symbol;
        insert_value(heap, new);
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

    if(left_index < heap->len && heap->nodes[left_index]->frequency < heap->nodes[minimum]->frequency){
        minimum = left_index;
    }
    if(right_index < heap->len && heap->nodes[right_index]->frequency < heap->nodes[minimum]->frequency){
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

    HeapNode * returning = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->len - 1];
    printf("\nExtracted: %c (%ld)", returning->data, returning->frequency);

    heap->len--;
    heapify(heap, 0);

    return returning;
}

HeapNode * form_huffman_tree(Heap * heap){
    while(heap->len != 1){
        HeapNode * left = extract_min(heap);
        HeapNode * right = extract_min(heap);

        HeapNode * internal_node = malloc(sizeof(HeapNode));
        internal_node->left_child = (struct HeapNode *) left;
        internal_node->right_child = (struct HeapNode *) right;
        internal_node->data = 0; // default value indicating this is an INTERNAL NODE
        internal_node->frequency = left->frequency + right->frequency;

        // the newly formed internal node is returned to the heap
        insert_value(heap, internal_node);
    }

    return extract_min(heap);
}

void print_byte_buffer2(Byte_buffer * byte_buffer){
    // print the contents of the byte_buffer
    FILE * out;
    out = fopen(OUTPUT, "a");
    if(out == NULL){
        printf("Can't open file.");
        exit(1);
    }

    int len = 7 - byte_buffer->index;
    if (len > 0) {
        fputc(byte_buffer->byte, out);
    }
    fclose(out);
    byte_buffer->byte = 0;
    byte_buffer->index = 7;
}

void fill_byte_buffer2(Byte_buffer * byte_buffer, const char * string){
    for(int i = 0; i < strlen(string); i++){
        if(string[i] == '0'){
            byte_buffer->byte &= ~(1 << byte_buffer->index);
        }
        else if(string[i] == '1'){
            byte_buffer->byte |= (1 << byte_buffer->index);
        }
        byte_buffer->index--;
        if(byte_buffer->index == -1) print_byte_buffer2(byte_buffer);
    }
}

char * stringify(const int array[], int count){
    char * temp = malloc((count + 1) * sizeof(char));
    for(int i = 0; i < count; i++){
        if(array[i] == 0) temp[i] = '0';
        else temp[i] = '1';
    }
    temp[count] = 0;
    return temp;
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
        // making of dictionary
        char * temp = stringify(code, counter);
        add_output_hash_char(node->data, temp, &output_hash_table);

        printf("\nHuffman code for %c is: ", node->data);
        for(int i = 0; i < counter; i++){
            printf("%d", code[i]);
        }
    }
}

void delete_file2(char * filename){
    if (remove(filename) != 0){
        // if nothing is deleted
        printf("\nNo file was deleted.");
    }
}

void add_char(char symbol, hash_entry ** table) {
    hash_entry * s;

    s = malloc(sizeof *s);
    s->symbol = symbol;
    s->frequency = 1;

    HASH_ADD(hh, *table, symbol, sizeof(char), s);
}

hash_entry * find_char(char symbol, hash_entry ** table) {
    hash_entry * s;
    HASH_FIND(hh, *table, &symbol, sizeof(char), s);
    return s;
}

void add_output_hash_char(char symbol, char * string, output_hash ** table){
    output_hash * s;

    s = malloc(sizeof *s);
    s->symbol = symbol;
    s->huffman = malloc((strlen(string) + 1) * sizeof(char));
    s->huffman = string;

    HASH_ADD(hh, *table, symbol, sizeof(char), s);
}

char * huffman_code_hash(char symbol, output_hash ** table) {
    output_hash * s;
    HASH_FIND(hh, *table, &symbol, sizeof(char), s);
    return s->huffman;
}

void print_hash(hash_entry ** table) {
    hash_entry * s;
    for (s = *table; s != NULL; s = s->hh.next) {
        printf("\nsymbol: %c, freq: %ld", s->symbol, s->frequency);
    }
}

void export_hash_table(hash_entry * hashTable, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return;
    }

    hash_entry * entry;
    for (entry = hashTable; entry != NULL; entry = entry->hh.next) {
        fprintf(file, "%c %ld\n", entry->symbol, entry->frequency);
    }

    fclose(file);
}

hash_entry * import_hash_table(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return NULL;
    }

    hash_entry * hashTable = NULL;
    char symbol; // key
    long int freq; // value

    while (fscanf(file, "%c %ld\n", &symbol, &freq) != EOF) {
        hash_entry * entry = (hash_entry*)malloc(sizeof(hash_entry));
        if (entry == NULL) {
            printf("Memory allocation error\n");
            fclose(file);
            return NULL;
        }
        entry->symbol = symbol;
        entry->frequency = freq;

        HASH_ADD(hh, hashTable, symbol, sizeof(char), entry);
    }

    fclose(file);
    return hashTable;
}


// static_huffman_encode
int static_huffman_encode() {
    char * filename = "input.txt";
    //filename = "tmpfile-jJbl8hnull";
    char * filename_copy = malloc(200 * sizeof(char));
    filename_copy = strcpy(filename_copy, filename);

    // determining the extension of the file
    char * extension = malloc(10 * sizeof(char));
    char * token = strtok(filename_copy, ".");
    if(token != NULL){
        token = strtok(NULL, ".");
    }
    if(token != NULL){
        strcpy(extension, token);
    }
    else {
        strcpy(extension, "other");
    }

    FILE * in;
    in = fopen(filename, "rb");

    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }


    // reading from a file
    char data;

    int count = 0;
    // if it's binary
    if(strcmp(extension, "txt") != 0){
        while(fread(&data, sizeof(char), 1, in) == 1){

            hash_entry * s = find_char(data, &hash_table);
            if(s == NULL) add_char(data, &hash_table);
            else s->frequency++;

            count += 1;
        }
    }
    // if it's a .txt
    else {
        char c;
        while((c = fgetc(in)) != EOF){
            count++;
            hash_entry * s = find_char(c, &hash_table);
            if(s == NULL) add_char(c, &hash_table);
            else s->frequency++;
        }

    }

    printf("\nNumber of read chars: %d", count);

    fclose(in);

    print_hash(&hash_table);



    Heap * min_heap = form_min_heap();

    HeapNode * static_huffman_root = form_huffman_tree(min_heap);


    int throwaway[5000];
    int counter = 0;
    get_huffman_codes(static_huffman_root, counter, throwaway);


    in = fopen(filename, "rb");

    // delete the output file if it already exists
    delete_file2(OUTPUT);

    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }

    Byte_buffer * byte_buffer = malloc(sizeof(Byte_buffer));
    byte_buffer->byte = 0;
    byte_buffer->index = 7;

    // iterating through the file again, and writing codes into export file
    if(strcmp(extension, "txt") != 0){
        while(fread(&data, sizeof(char), 1, in) == 1){

        }
    }
    // if it's a .txt
    else {
        char c;
        printf("\nSize of dictionary: %d", HASH_COUNT(output_hash_table));
        while((c = fgetc(in)) != EOF){
            char * code = huffman_code_hash(c, &output_hash_table);
            fill_byte_buffer2(byte_buffer, code);
        }

    }

    fclose(in);

    print_byte_buffer2(byte_buffer);
    if(byte_buffer->index != 7) printf("\nWarning! Byte buffer not empty!");

    export_hash_table(hash_table, "hes_tabela.txt");

    printf("\n\n");
    print_hash(&hash_table);

    hash_entry * novi = import_hash_table("hes_tabela.txt");

    printf("\n\nOvo drugo:");
    print_hash(&novi);

    return 0;
}