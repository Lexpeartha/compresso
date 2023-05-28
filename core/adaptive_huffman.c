#include "adaptive_huffman.h"
#define OUTPUT "adaptive_output"
#define DECOMPRESSED "decompressed"
#define BUFFER_SIZE 100
#define NYT (-10) // a randomly picked number that won't occur in files
#define INTERNAL_NODE (-11) // also a random number, used to identify what is an internal node
#define MAX 10000

int number_of_nodes = 0;

void print_byte_buffer(Byte_buffer * byte_buffer){
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

void fill_byte_buffer(Byte_buffer * byte_buffer, char * string, int is_ascii, char ascii_symbol){
    // if the buffer needs to be filled with an ascii character
    if(is_ascii){
        for(int i = 0; i < 8; i++){
            // iterating from LSB to MSB
            if(ascii_symbol & 1){
                // it's a 1
                byte_buffer->byte |= (1 << byte_buffer->index);
            }
            else {
                // it's a 0
                byte_buffer->byte &= ~(1 << byte_buffer->index);
            }
            byte_buffer->index--;
            if(byte_buffer->index == -1) print_byte_buffer(byte_buffer);

            ascii_symbol >>= 1;
        }
    }
    // if the buffer needs to be filled with 1s and 0s
    else {
        for(int i = 0; i < strlen(string); i++){
            if(string[i] == '0'){
                byte_buffer->byte &= ~(1 << byte_buffer->index);
            }
            else if(string[i] == '1'){
                byte_buffer->byte |= (1 << byte_buffer->index);
            }
            byte_buffer->index--;
            if(byte_buffer->index == -1) print_byte_buffer(byte_buffer);
        }
    }
}

Node * create_empty_tree(){
    // initialization of an empty tree
    Node * root = malloc(sizeof(Node));
    root->left_child = NULL;
    root->right_child = NULL;
    root->parent = NULL;

    // NYT has a weight of 0 and the identifier 0
    root->symbol = NYT;
    root->identifier = 0;
    root->weight = 0;

    number_of_nodes++;

    return root;
}

void enqueue(Queue_Stack * queue, Node * node){
    queue->nodes[queue->length] = node;
    queue->length++;
}

Node * dequeue(Queue_Stack * queue){
    if(queue->length == 0){
        // if the queue is empty
        return NULL;
    }
    else {
        Node * temp = queue->nodes[0];
        for(int i = 0; i < queue->length - 1; i++){
            queue->nodes[i] = queue->nodes[i + 1];
        }
        queue->length--;
        return temp;
    }
}

void push(Queue_Stack * stack, Node * node){
    stack->nodes[stack->length] = node;
    stack->length++;
}

Node * pop(Queue_Stack * stack){
    if(stack->length == 0){
        // if the stack is empty
        return NULL;
    }
    else {
        stack->length--;
        Node * temp = stack->nodes[stack->length];
        return temp;
    }
}

void update_identifiers(Node * root){
    // I'm using reverse level order
    // this implementation uses a QUEUE and a STACK

    // Queue_Stack -> a structure used for both a queue and stack;
    // no need to have them separately
    Queue_Stack * queue = malloc(sizeof(Queue_Stack));
    Queue_Stack * stack = malloc(sizeof(Queue_Stack));
    queue->nodes = malloc(number_of_nodes * sizeof(Node));
    stack->nodes = malloc(number_of_nodes * sizeof(Node));
    queue->length = 0;
    stack->length = 0;

    enqueue(queue, root);
    while(queue->length != 0){
        Node * temp_node = dequeue(queue);
        push(stack, temp_node);
        if(temp_node->right_child != NULL) enqueue(queue, temp_node->right_child);
        if(temp_node->left_child != NULL) enqueue(queue, temp_node->left_child);
    }

    // the for loop needs to be written like this, because the length of the stack changes;
    // otherwise it wouldn't execute properly
    int identifier = 0;
    for(int i = stack->length - 1; i >= 0; i--){
        Node * temp = pop(stack);
        temp->identifier = identifier++;
    }
    free(queue->nodes);
    free(stack->nodes);
    free(queue);
    free(stack);

}


// returns the node that has the target symbol
Node * get_node(Node * node, char symbol){
    if(node != NULL){
        if(node->symbol == symbol){
            return node;
        } else {
            Node * found_node = get_node(node->left_child, symbol);
            if(found_node == NULL) {
                found_node = get_node(node->right_child, symbol);
            }
            return found_node;
        }
    } else {
        // unexpected error
        return NULL;
    }
}


// writes out the code of a leaf
char * path_to_node(Node * root, char symbol){
    Node * found_node = get_node(root, symbol);
    Node * next_node;
    char * buffer = malloc(BUFFER_SIZE * sizeof(char));
    int buffer_length = 0;

    while (found_node->parent != NULL){
        next_node = found_node->parent;
        if(next_node->left_child == found_node){
            buffer[buffer_length++] = '0';
        }
        else {
            buffer[buffer_length++] = '1';
        }
        found_node = next_node;
    }

    // reversing the string, because it's written in reverse
    char * reverse = malloc(buffer_length* sizeof(char));
    int count = 0;

    for(int i = buffer_length - 1; i >= 0; i--){
        reverse[count++] = buffer[i];
    }
    reverse[count] = 0;

    free(buffer);

    return reverse;
}

// deletes a file
void delete_file(char * filename){
    if (remove(filename) != 0){
        // if nothing is deleted
        printf("\nNo file was deleted.");
    }
}

void transmit_code(Node * root, char symbol, int exists, Byte_buffer * byte_buffer){
    if(root->left_child == NULL && root->right_child == NULL){
        // the output file needs to be deleted first, because codes are appended
        delete_file(OUTPUT);
    }

    char * buffer = NULL;

    // if the tree is empty
    if(root->left_child == NULL && root->right_child == NULL){
        //printf("%c", symbol);
        fill_byte_buffer(byte_buffer, "", 1, symbol);
    }
    // if it's the first occurrence
    else if (exists == 0){

        // find NYT node
        buffer = path_to_node(root, NYT);
        fill_byte_buffer(byte_buffer, buffer, 0, 'a');

        // print out the new code for the symbol
        fill_byte_buffer(byte_buffer, "", 1, symbol);
    }
    // if a char already present in the tree is transmitted
    else {
        buffer = path_to_node(root, symbol);
        fill_byte_buffer(byte_buffer, buffer, 0, 'a');

    }
    free(buffer);
}

// a function that 'extends' the NYT node; NYT becomes an internal node R, the left child becomes
// the new NYT, and the right one is for the symbol S
void spawn_node(Node * root, char symbol){
    Node * nyt_node = get_node(root, NYT);

    nyt_node->left_child = malloc(sizeof(Node));
    nyt_node->right_child = malloc(sizeof(Node));

    // changing details of the old NYT node
    nyt_node->symbol = INTERNAL_NODE;
    nyt_node->weight = 1;
    nyt_node->identifier = 2;


    // updating the left child
    nyt_node->left_child->symbol = NYT;
    nyt_node->left_child->weight = 0;
    nyt_node->left_child->identifier = 0;
    nyt_node->left_child->parent = nyt_node;
    nyt_node->left_child->left_child = NULL;
    nyt_node->left_child->right_child = NULL;

    // updating the right child
    nyt_node->right_child->symbol = symbol;
    nyt_node->right_child->weight = 1;
    nyt_node->right_child->identifier = 1;
    nyt_node->right_child->parent = nyt_node;
    nyt_node->right_child->left_child = NULL;
    nyt_node->right_child->right_child = NULL;

    number_of_nodes += 2;

}

Node * find_greater_identifier(Node * root, Node * target_node){
    Queue_Stack * queue = malloc(sizeof(Queue_Stack));
    queue->nodes = malloc(number_of_nodes * sizeof(Node *));
    queue->length = 0;

    Node ** eligible_nodes = malloc(number_of_nodes * sizeof(Node *));
    int eligible_nodes_length = 0;

    // level order traversal; searching for a node that has the same weight but greater id
    Node * temp = root;
    while(temp != NULL){
        if(temp->weight == target_node->weight && temp->identifier > target_node->identifier){
            eligible_nodes[eligible_nodes_length] = malloc(sizeof(Node));
            eligible_nodes[eligible_nodes_length++] = temp;
        }
        if(temp->left_child != NULL) enqueue(queue, temp->left_child);
        if(temp->right_child != NULL) enqueue(queue, temp->right_child);
        temp = dequeue(queue);
    }

    free(queue->nodes);
    free(queue);

    int max = 0;
    for(int i = 0; i < eligible_nodes_length; i++){
        if(eligible_nodes[i]->identifier > eligible_nodes[max]->identifier) max = i;
    }
    if(eligible_nodes_length > 0) {
        return eligible_nodes[max];
    }
    else {
        free(eligible_nodes);
        return NULL;
    }
}

// swaps two nodes in the tree
Node * swap_nodes(Node * node1, Node * node2){

    if(node1 == NULL || node2 == NULL){
        // unexpected error
        return NULL;
    }

    // swapping symbols
    char temp_symbol = node1->symbol;
    node1->symbol = node2->symbol;
    node2->symbol = temp_symbol;


    Node * temp_left_child = node1->left_child;
    Node * temp_right_child = node1->right_child;

    node1->left_child = node2->left_child;
    node1->right_child = node2->right_child;

    node2->left_child = temp_left_child;
    node2->right_child = temp_right_child;


    if(node1->left_child != NULL) node1->left_child->parent = node1;
    if(node1->right_child != NULL) node1->right_child->parent = node1;
    if(node2->left_child != NULL) node2->left_child->parent = node2;
    if(node2->right_child != NULL) node2->right_child->parent = node2;


    return node2;
}

// function parameters
// Node * root: THE ROOT
// Node * mobile_node: A NODE THAT WE'RE COMPARING WITH OTHER NODES OF THE SAME WEIGHT
void update_weights(Node * root, Node * mobile_node){
    // in the first step, the weights are already assigned properly; automatically moving on to the parent
    while(mobile_node != NULL){
        Node * temp = find_greater_identifier(root, mobile_node);
        if(temp != NULL){
            if(mobile_node->parent != temp) mobile_node = swap_nodes(mobile_node, temp);
        }
        mobile_node->weight++;
        mobile_node = mobile_node->parent;

    }
}

void update_tree(Node * root, char symbol){
    update_identifiers(root);
    if(symbol == NYT){
        // arguments: root, parent of a parent of an NYT node
        // because that triplet is already assigned correct weight values
        Node * temp = get_node(root, NYT)->parent;
        update_weights(root, temp->parent);
    }
    else {
        update_weights(root, get_node(root, symbol));
    }
}

// processing of one char by the huffman algorithm
void execute_adaptive_huffman(Node * root, char symbol, Byte_buffer * byte_buffer){
    // symbol already exists
    Node * found_node = get_node(root, symbol);
    char node_symbol = NYT;

    if (found_node != NULL){
        transmit_code(root, symbol, 1, byte_buffer);
        node_symbol = symbol;
    }
    // symbol does NOT exist
    else {
        transmit_code(root, symbol, 0, byte_buffer);
        spawn_node(root, symbol);
    }
    update_tree(root, node_symbol);

}

char navigate(Node * root, const char * buffer, int buffer_len){
    Node * temp2 = root;
    for(int i = 0; i < buffer_len; i++){
        if(buffer[i] == '0') temp2 = temp2->left_child;
        else if(buffer[i] == '1') temp2 = temp2->right_child;
    }
    return temp2->symbol;
}

char charify_ascii(char input){
    char reversed = 0;
    int number_of_bits = 8; // Number of bits in a char

    for (int i = 0; i < number_of_bits; i++) {
        reversed <<= 1;
        if (input & 1)
            reversed |= 1;
        input >>= 1;
    }

    return reversed;
}

void remove_first_element(char * buffer, int * len){
    for(int i = 0; i < *len; i++){
        buffer[i] = buffer[i + 1];
    }
    (*len)--;
}

char charify_string(char * string){
    char x = strtol(string, NULL, 2);
    return charify_ascii(x);
}

int adaptive_huffman_decode(char * filename){

    printf("\nDecoding has started.");
    delete_file(filename);

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

    char data;

    number_of_nodes = 0;
    Node * root2 = create_empty_tree();

    fseek(in, 0, SEEK_END);
    long int size = ftell(in);
    long int counter = 0;
    printf("\n\nSize of compressed file: %ld B", size);
    fseek(in, 0, SEEK_SET);

    fread(&data, sizeof(char), 1, in);
    spawn_node(root2, charify_ascii(data));
    char x = charify_ascii(data);
    counter++;
    fputc(x, out);

    char * buffer = malloc(MAX * sizeof(char));
    char temp_new[MAX];
    int temp_new_len = 0;
    int buffer_len = 0;
    int go = 1;

    while(counter <= size - 2 && go){
        fread(&data, sizeof(char), 1, in);
        counter++;

        // transfer the bits from the newly read byte to a buffer
        for (int i = 7; i >= 0; i--) {
            int bit = (data >> i) & 1;
            if(bit == 0) buffer[buffer_len++] = '0';
            else buffer[buffer_len++] = '1';
        }

        // try to navigate
        while(1){
            temp_new[temp_new_len++] = buffer[0];
            remove_first_element(buffer, &buffer_len);
            char navigate_result = navigate(root2, temp_new, temp_new_len);

            if(navigate_result == NYT){
                // read another byte, except if it's the penultimate
                if(counter + 1 == size - 1) {
                    go = 0;
                    break;
                }
                else {
                    fread(&data, sizeof(char), 1, in);
                    counter++;
                    // fill this into the buffer again
                    for (int i = 7; i >= 0; i--) {
                        int bit = (data >> i) & 1;
                        if(bit == 0) buffer[buffer_len++] = '0';
                        else buffer[buffer_len++] = '1';
                    }

                    // charify first eight bits of buffer
                    temp_new_len = 0;
                    for(int i = 0; i < 8; i++){
                        temp_new[temp_new_len++] = buffer[0];
                        remove_first_element(buffer, &buffer_len);
                    }

                    char symbol = charify_string(temp_new);
                    spawn_node(root2, symbol);
                    fputc(symbol, out);
                    temp_new_len = 0;
                    update_tree(root2, NYT);
                }
            }
            else if(navigate_result != INTERNAL_NODE){
                // write the symbol, update tree
                fputc(navigate_result, out);
                update_tree(root2, navigate_result);
                temp_new_len = 0;
            }

            if(buffer_len == 0) break;
        }
    }

    // process the last two lines of .bin file
    char penultimate, ultimate;
    fread(&penultimate, sizeof(char), 1, in);
    fread(&ultimate, sizeof(char), 1, in);

    int useful_bits = (int) ultimate;
    for (int i = 7; i >= 8 - useful_bits; i--) {
        int bit = (data >> i) & 1;
        if(bit == 0) buffer[buffer_len++] = '0';
        else buffer[buffer_len++] = '1';
    }


    fclose(out);
    fclose(in);


    return 0;
}

void free_tree(Node * root){
    // do nothing
}

char how_many_useful_bits(Byte_buffer * byte_buffer){
    return (char) (7 - byte_buffer->index);
}

// adaptive_huffman_encode
int adaptive_huffman_encode(){
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

    Node * root = create_empty_tree();

    FILE * in;
    in = fopen(filename, "rb");

    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }


    // reading from a file
    char data;
    Byte_buffer * byte_buffer = malloc(sizeof(Byte_buffer));
    byte_buffer->byte = 0;
    byte_buffer->index = 7;

    // if it's binary
    if(strcmp(extension, "txt") != 0){
        while(fread(&data, sizeof(char), 1, in) == 1){
            execute_adaptive_huffman(root, data, byte_buffer);
        }
    }
    // if it's a .txt
    else {
        char c;
        int count = 0;
        while((c = fgetc(in)) != EOF){
            count++;
            execute_adaptive_huffman(root, c, byte_buffer);
        }
        printf("\nNumber of read chars: %d", count);
    }

    fclose(in);

    printf("\nNumber of nodes: %d", number_of_nodes);
    printf("\n(Consuming %zu bytes before free() )", sizeof(Node) * number_of_nodes);

    char useful_bits = how_many_useful_bits(byte_buffer);
    printf("\nUseful bits: %d", useful_bits);
    print_byte_buffer(byte_buffer);

    FILE * out;
    out = fopen(OUTPUT, "a");
    if(out == NULL){
        printf("Can't open file.");
        exit(1);
    }

    fputc(useful_bits, out);
    fclose(out);

    // DELETE LATER
    adaptive_huffman_decode(DECOMPRESSED);

    free(root);
    free(filename_copy);
    free(byte_buffer);
    free(extension);



    return 0;
}