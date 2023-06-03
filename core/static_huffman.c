#include "static_huffman.h"
#define OUTPUT "static_output"
#define INTERNAL_NODE 0
#define MAX 10000

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
    uint8_t temp2 = heap->nodes[ind1]->data;
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

Heap * form_min_heap(hash_entry * table) {
    // initializing the heap
    Heap * heap = malloc(sizeof(Heap));
    heap->nodes = malloc(HASH_COUNT(table) * sizeof(HeapNode *));
    heap->len = 0;

    hash_entry * s;
    for (s = table; s != NULL; s = s->hh.next) {
        HeapNode * new = malloc(sizeof(HeapNode));
        if(new == NULL){
            printf("\nMemory allocation failed! ");
            exit(3);
        }
        new->frequency = s->frequency;
        new->data = s->symbol;
        insert_value(heap, new);
    }
    if(heap == NULL){
        printf("\nMemory allocation failed! ");
        exit(4);
    }
    return heap;
}

int node_is_leaf(HeapNode * node){
    if (node->left_child == NULL && node->right_child == NULL) return 1;
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
    //printf("\nExtracted: %c (%ld)", returning->data, returning->frequency);

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
        internal_node->data = INTERNAL_NODE; // default value indicating this is an INTERNAL NODE
        internal_node->frequency = left->frequency + right->frequency;

        // the newly formed internal node is returned to the heap
        insert_value(heap, internal_node);
    }

    return extract_min(heap);
}

void print_byte_buffer2(Byte_buffer * byte_buffer){
    // print the contents of the byte_buffer
    FILE * out;
    out = fopen(OUTPUT, "ab");
    if(out == NULL){
        printf("Can't open file.");
        exit(1);
    }

    int len = 7 - byte_buffer->index;
    if (len > 0) {
        fwrite(&byte_buffer->byte, sizeof(uint8_t), 1, out);
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






void push_stack(Stack * stack, StackNode temp) {
    if (stack->top < MAX_STACK_SIZE - 1) {
        stack->data[++stack->top] = temp;
    }
}

StackNode pop_stack(Stack * stack) {
    if (stack->top >= 0) {
        StackNode temp = stack->data[stack->top--];
        return temp;
    }
}

int empty(Stack * stack) {
    return (stack->top == -1);
}

StackNode makeStackNode(HeapNode* n, int c, int cd[]) {
    StackNode temp;
    temp.node = n;
    temp.counter = c;
    memcpy(temp.code, cd, sizeof(int) * MAX_HUFFMAN_CODE_LEN);
    return temp;
}

void get_huffman_codes(HeapNode* node, int counter, int code[]) {
    Stack * stack = malloc(sizeof(Stack));
    stack->top = -1;
    push_stack(stack, makeStackNode(node, counter, code));

    while (!empty(stack)) {
        StackNode current = pop_stack(stack);
        node = current.node;
        counter = current.counter;
        code = current.code;

        if (node->left_child != NULL) {
            code[counter] = 0;
            push_stack(stack, makeStackNode((HeapNode *) node->left_child, counter + 1, code));
        }
        if (node->right_child != NULL) {
            code[counter] = 1;
            push_stack(stack, makeStackNode((HeapNode *) node->right_child, counter + 1, code));
        }
        if (node_is_leaf(node)) {
            // Making of dictionary
            char* temp = stringify(code, counter);
            add_output_hash_char(node->data, temp, &output_hash_table);

            printf("\nHuffman code for %c is: ", node->data);
            for (int i = 0; i < counter; i++) {
                printf("%d", code[i]);
            }
        }
    }

    free(stack);
}

void delete_file2(char * filename){
    if (remove(filename) != 0){
        // if nothing is deleted
        printf("\nNo file was deleted.");
    }
}

void add_char(uint8_t symbol, hash_entry ** table) {
    hash_entry * s;

    s = malloc(sizeof *s);
    s->symbol = symbol;
    s->frequency = 1;

    HASH_ADD(hh, *table, symbol, sizeof(char), s);
}

hash_entry * find_char(uint8_t symbol, hash_entry ** table) {
    hash_entry * s;
    HASH_FIND(hh, *table, &symbol, sizeof(uint8_t), s);
    return s;
}

char how_many_useful_bits2(Byte_buffer * byte_buffer){
    return (char) (7 - byte_buffer->index);
}

void add_output_hash_char(uint8_t symbol, char * string, output_hash ** table){
    output_hash * s;

    s = malloc(sizeof *s);
    s->symbol = symbol;
    s->huffman = malloc((strlen(string) + 1) * sizeof(char));
    s->huffman = string;

    HASH_ADD(hh, *table, symbol, sizeof(uint8_t), s);
}

char * huffman_code_hash(uint8_t symbol, output_hash ** table) {
    output_hash * s;
    HASH_FIND(hh, *table, &symbol, sizeof(uint8_t), s);
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
        fwrite(&entry->symbol, sizeof(uint8_t), 1, file);
        fwrite(&entry->frequency, sizeof(long int), 1, file);
        //fprintf(file, "%c %ld\n", entry->symbol, entry->frequency);
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
    uint8_t symbol; // key
    long int freq; // value
    size_t x;

    while(1){
        fread(&symbol, sizeof(uint8_t), 1, file);
        x = fread(&freq, sizeof(long int), 1, file);

        if(x == 0) break;

        hash_entry * entry = (hash_entry*)malloc(sizeof(hash_entry));
        if (entry == NULL) {
            printf("Memory allocation error\n");
            fclose(file);
            return NULL;
        }
        entry->symbol = symbol;
        entry->frequency = freq;

        HASH_ADD(hh, hashTable, symbol, sizeof(uint8_t), entry);
    }

    fclose(file);
    return hashTable;
}

void remove_first_element2(uint8_t * buffer, int * len){
    for(int i = 0; i < *len; i++){
        buffer[i] = buffer[i + 1];
    }
    (*len)--;
}

void free_hash_table(output_hash * table){

    output_hash *current_entry, *tmp;

    // Iterate over each entry in the hash table
    HASH_ITER(hh, table, current_entry, tmp) {
        // Remove the current entry from the hash table
        HASH_DEL(hash_table, current_entry);

        // Free the key and value associated with the entry
        free(current_entry->huffman);
        free(current_entry);

        // Note: If your value contains dynamically allocated memory,
        // make sure to free it before freeing the hash_entry struct.
    }

}

void free_hash_table_hash_entry(hash_entry * table){

    hash_entry *current_entry, *tmp;

    // Iterate over each entry in the hash table
    HASH_ITER(hh, table, current_entry, tmp) {
        // Remove the current entry from the hash table
        HASH_DEL(hash_table, current_entry);

        // Free the key and value associated with the entry
        free(current_entry);

        // Note: If your value contains dynamically allocated memory,
        // make sure to free it before freeing the hash_entry struct.
    }

}


uint8_t navigate2(HeapNode * root, uint8_t * buffer, int buffer_len){
    HeapNode * temp2 = root;
    for(int i = 0; i < buffer_len; i++){
        if(buffer[i] == '0') temp2 = (HeapNode *) temp2->left_child;
        else if(buffer[i] == '1') temp2 = (HeapNode *) temp2->right_child;
    }
    return temp2->data;
}


int static_huffman_decode(char * filename){

    printf("\n\nDECOMPRESSION.");

    hash_entry * novi = import_hash_table("hes_tabela");
    delete_file2(filename);

    // form the tree
    Heap * min_heap = form_min_heap(novi);
    HeapNode * decompression_root = form_huffman_tree(min_heap);

    FILE * in;
    FILE * out;
    in = fopen(OUTPUT, "rb");
    out = fopen(filename, "ab");
    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }
    if(out == NULL){
        printf("Can't open file.\n");
        exit(1);
    }

    uint8_t buffer[MAX];
    int buffer_len = 0;
    uint8_t temp_new[MAX];
    int temp_new_len = 0;

    fseek(in, 0, SEEK_END);
    long int size = ftell(in);
    long int counter = 0;
    printf("\nSize of compressed file: %ld B", size);
    fseek(in, 0, SEEK_SET);

    uint8_t data;

    while(counter < size){
        if(counter == size - 2){
            uint8_t penultimate;
            uint8_t ultimate;
            fread(&penultimate, sizeof(uint8_t), 1, in);
            fread(&ultimate, sizeof(uint8_t), 1, in);
            counter += 2;
            printf("\nDa: %d", ultimate);

            for (int i = 7; i >= 7 - ultimate + 1; i--) {
                int bit = (penultimate >> i) & 1;
                if(bit == 0) buffer[buffer_len++] = '0';
                else buffer[buffer_len++] = '1';
            }

        }
        else{
            fread(&data, sizeof(uint8_t), 1, in);
            counter++;

            // transfer the bits from the newly read byte to a buffer
            for (int i = 7; i >= 0; i--) {
                int bit = (data >> i) & 1;
                if(bit == 0) buffer[buffer_len++] = '0';
                else buffer[buffer_len++] = '1';
            }
        }


        while(1){
            temp_new[temp_new_len++] = buffer[0];
            remove_first_element2(buffer, &buffer_len);
            uint8_t navigate_result = navigate2(decompression_root, temp_new, temp_new_len);

            if(navigate_result != INTERNAL_NODE){
                // write symbol
                fwrite(&navigate_result, sizeof(uint8_t), 1, out);
                temp_new_len = 0;
            }

            if(buffer_len == 0) break;
        }
    }


    return 0;
}

// static_huffman_encode
int static_huffman_encode() {
    char * filename = "astronaut.png";
    //filename = "tmpfile-Drxw2V";
    //filename = "tmpfile-jJbl8hnull";
    //filename = "robinson_crusoe.txt";
    // determining the extension of the file


    FILE * in;
    in = fopen(filename, "rb");

    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }


    // reading from a file
    uint8_t data;

    int count = 0;
    // read file
    while(fread(&data, sizeof(uint8_t), 1, in) == 1) {
        count++;
        hash_entry *s = find_char(data, &hash_table);
        if (s == NULL) add_char(data, &hash_table);
        else s->frequency++;

    }

    printf("\nNumber of read chars: %d", count);

    fclose(in);

    print_hash(&hash_table);



    Heap * min_heap = form_min_heap(hash_table);

    HeapNode * static_huffman_root = form_huffman_tree(min_heap);


    int throwaway[500];
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

    printf("\n\nIteriram.");

    // iterating through the file again, and writing codes into export file
    printf("\nSize of dictionary: %d\n\n", HASH_COUNT(output_hash_table));
    while (fread(&data, sizeof(uint8_t), 1, in) == 1) {
        char * code = huffman_code_hash(data, &output_hash_table);
        fill_byte_buffer2(byte_buffer, code);
    }

    fclose(in);

    printf("\nWarning! Byte buffer not empty! %d", byte_buffer->index);

    uint8_t useful_bits = how_many_useful_bits2(byte_buffer);
    printf("\nUseful bits: %d", useful_bits);
    print_byte_buffer2(byte_buffer);

    FILE * out;
    out = fopen(OUTPUT, "ab");
    if(out == NULL){
        printf("Can't open file.");
        exit(1);
    }
    fwrite(&useful_bits, sizeof(uint8_t), 1, out);
    fclose(out);

    export_hash_table(hash_table, "hes_tabela");
    free_hash_table(output_hash_table);

    static_huffman_decode("static_decompressed.txt");

    //free_hash_table_hash_entry(hash_table);

    return 0;
}