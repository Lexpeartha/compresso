#include "adaptive_huffman.h"
#define OUTPUT "adaptive_output"
#define BUFFER_SIZE 100
#define NYT 255 // a randomly picked number that won't occur in files
#define INTERNAL_NODE 254 // also a random number, used to identify what is an internal node
#define MAX 10000

int number_of_nodes = 0;
int number_of_asciis = 0;

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
Node * get_node(Node * node, unsigned char symbol){
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
uint8_t * path_to_node(Node * root, uint8_t symbol, int * len){
    Node * found_node = get_node(root, symbol);
    Node * next_node;
    uint8_t * buffer = malloc(BUFFER_SIZE * sizeof(uint8_t));
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
    uint8_t * reverse = malloc(buffer_length* sizeof(uint8_t));
    int count = 0;

    for(int i = buffer_length - 1; i >= 0; i--){
        reverse[count++] = buffer[i];
    }
    reverse[count] = 0;
    *len = buffer_length;

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

// a function that 'extends' the NYT node; NYT becomes an internal node R, the left child becomes
// the new NYT, and the right one is for the symbol S
void spawn_node(Node * root, uint8_t symbol){
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
    unsigned char temp_symbol = node1->symbol;
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

void update_tree(Node * root, uint8_t symbol){
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

void print_byte_buffer(Byte_buffer * byte_buffer, char output_filename_temp[]){
    // print the contents of the byte_buffer
    FILE * out;
    out = fopen(output_filename_temp, "ab");
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

void fill_byte_buffer(Byte_buffer * byte_buffer, const uint8_t * string, int len, char output_filename_temp[]){
    for(int i = 0; i < len; i++){
        if(string[i] == '0'){
            byte_buffer->byte &= ~(1 << byte_buffer->index);
        }
        else if(string[i] == '1'){
            byte_buffer->byte |= (1 << byte_buffer->index);
        }
        byte_buffer->index--;
        if(byte_buffer->index == -1) print_byte_buffer(byte_buffer, output_filename_temp);
    }

}

void transmit_code(Node * root, uint8_t symbol, int exists, Byte_buffer * byte_buffer, char output_filename[], char output_filename_temp[]){

    if(root->left_child == NULL && root->right_child == NULL){
        // the output file needs to be deleted first, because codes are appended
        delete_file(output_filename);
        delete_file(output_filename_temp);
    }

    // output_filename --> ASCII codes
    // output_filename_temp --> 1s and 0s

    uint8_t * buffer = NULL;

    // if the tree is empty
    if(root->left_child == NULL && root->right_child == NULL){
        //printf("%c", symbol);
        FILE * out;
        out = fopen(output_filename, "ab");
        fwrite(&symbol, sizeof(uint8_t), 1, out);
        fclose(out);
        number_of_asciis = 1;
    }
        // if it's the first occurrence
    else if (exists == 0){
        int len = 0;
        // find NYT node
        buffer = path_to_node(root, NYT, &len);
        //printf("%s", buffer);
        fill_byte_buffer(byte_buffer, buffer, len, output_filename_temp);

        // print out the new code for the symbol
        //printf("%c", symbol);

        FILE * out;
        out = fopen(output_filename, "ab");
        fwrite(&symbol, sizeof(uint8_t), 1, out);
        fclose(out);

        number_of_asciis++;
    }
        // if a char already present in the tree is transmitted
    else {
        int len = 0;
        buffer = path_to_node(root, symbol, &len);
        //printf("%s", buffer);
        fill_byte_buffer(byte_buffer, buffer, len, output_filename_temp);

    }
    free(buffer);
}

// processing of one char by the huffman algorithm
void execute_adaptive_huffman(Node * root, unsigned char symbol, Byte_buffer * byte_buffer, char output_filename[], char output_filename_temp[]){
    // symbol already exists
    Node * found_node = get_node(root, symbol);
    uint8_t node_symbol = NYT;

    if (found_node != NULL){
        transmit_code(root, symbol, 1, byte_buffer, output_filename, output_filename_temp);
        node_symbol = symbol;
    }
        // symbol does NOT exist
    else {
        transmit_code(root, symbol, 0, byte_buffer, output_filename, output_filename_temp);
        spawn_node(root, symbol);
    }
    update_tree(root, node_symbol);

}

uint8_t navigate(Node * root, const uint8_t buffer[], int buffer_len){
    Node * temp2 = root;
    for(int i = 0; i < buffer_len; i++){
        if(buffer[i] == '0') temp2 = temp2->left_child;
        else if(buffer[i] == '1') temp2 = temp2->right_child;
    }
    return temp2->symbol;
}

void remove_first_element(uint8_t * buffer, int * len){
    for(int i = 0; i < *len; i++){
        buffer[i] = buffer[i + 1];
    }
    (*len)--;
}

int adaptive_huffman_decode(char * filename){

    char filename_copy[150];
    strcpy(filename_copy, filename);
    char decompressed[150];
    sprintf(decompressed, "%s-DECOMPRESSED", filename);

    FILE * in;
    FILE * out;
    in = fopen(filename, "rb");
    out = fopen(decompressed, "ab");
    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }
    if(out == NULL){
        printf("Can't open file.\n");
        exit(1);
    }

    number_of_nodes = 0;
    Node * root = create_empty_tree();
    uint8_t data;
    uint8_t aux;

    char * token = strtok(filename_copy, "-");
    if(token == NULL){
        printf("Invalid file selected.");
        exit(1);
    }

    int number_of_chars = atoi(token);
    token = strtok(NULL, "-");

    if(token == NULL){
        printf("Invalid file selected.");
        exit(1);
    }
    int useful_bits = atoi(token);

    int char_pointer = 0;
    int currently_reading = number_of_chars;


    uint8_t temp_new[150];
    int temp_new_len = 0;
    uint8_t buffer[150];
    int buffer_len = 0;


    fseek(in, 0, SEEK_END);
    long int size = ftell(in);
    long int counter = 0;
    fseek(in, currently_reading, SEEK_SET);

    while(counter < size - number_of_chars){
        memset(buffer, 0, 150);
        fseek(in, currently_reading, SEEK_SET);
        if(counter == size - 1 - number_of_chars && useful_bits != 0){
            uint8_t ultimate;
            fread(&ultimate, sizeof(uint8_t), 1, in);




            for (int i = 7; i >= 7 - useful_bits + 1; i--) {
                int bit = (ultimate >> i) & 1;
                if(bit == 0) buffer[buffer_len++] = '0';
                else buffer[buffer_len++] = '1';
            }
            //printf("\nProcitano: %s", buffer);

        }
        else{
            fread(&data, sizeof(uint8_t), 1, in);


            // transfer the bits from the newly read byte to a buffer
            for (int i = 7; i >= 0; i--) {
                int bit = (data >> i) & 1;
                if(bit == 0) buffer[buffer_len++] = '0';
                else buffer[buffer_len++] = '1';
            }
            //printf("\nProcitano: %s", buffer);
        }
        counter++;
        currently_reading++;

        if(number_of_nodes == 1){
            fseek(in, char_pointer, SEEK_SET);
            fread(&aux, sizeof(uint8_t), 1, in);

            spawn_node(root, aux);

            fwrite(&aux, sizeof(uint8_t), 1, out);
            char_pointer++;
            fseek(in, currently_reading, SEEK_SET);
        }

        while(1){
            temp_new[temp_new_len++] = buffer[0];
            remove_first_element(buffer, &buffer_len);
            uint8_t navigate_result = navigate(root, temp_new, temp_new_len);

            if(navigate_result == NYT){
                // print the next char
                fseek(in, char_pointer, SEEK_SET);
                fread(&aux, sizeof(uint8_t), 1, in);
                fwrite(&aux, sizeof(uint8_t), 1, out);
                char_pointer++;
                memset(temp_new, 0, 150);
                temp_new_len = 0;

                spawn_node(root, aux);
                update_tree(root, NYT);

            }
            else if (navigate_result != INTERNAL_NODE){
                fwrite(&navigate_result, sizeof(uint8_t), 1, out);
                update_tree(root, navigate_result);
                memset(temp_new, 0, 150);
                temp_new_len = 0;
            }

            if(buffer_len <= 0) break;

        }

    }


    return 0;
}

uint8_t how_many_useful_bits(Byte_buffer * byte_buffer){
    return (uint8_t) (7 - byte_buffer->index);
}

// adaptive_huffman_encode
int adaptive_huffman_encode(char filename[150]){
//    char filename[150] = "input.txt";
//    strcpy(filename, "primer.doc");
    //strcpy(filename, "tmpfile-Drxw2V");
    char output_filename[150];
    long rand = random() % 100 + 1;
    long rand2 = random() % 100 + 1;
    sprintf(output_filename, "adaptive_output");

    char output_filename_temp[150];
    sprintf(output_filename_temp, "%s-temp", output_filename);

    Node * root = create_empty_tree();

    FILE * in;
    in = fopen(filename, "rb");

    if(in == NULL){
        printf("Can't open file.\n");
        exit(1);
    }

    // reading from a file
    uint8_t data;
    Byte_buffer * byte_buffer = malloc(sizeof(Byte_buffer));
    byte_buffer->byte = 0;
    byte_buffer->index = 7;

    while(fread(&data, sizeof(uint8_t), 1, in) == 1){
        execute_adaptive_huffman(root, data, byte_buffer, output_filename, output_filename_temp);
    }


    fclose(in);

    printf("\nNumber of nodes: %d", number_of_nodes);
    printf("\n(Consuming %zu bytes before free() )", sizeof(Node) * number_of_nodes);

    uint8_t useful_bits = how_many_useful_bits(byte_buffer);
    printf("\nUseful bits: %d", useful_bits);
    // printing what's left in the byte buffer
    print_byte_buffer(byte_buffer, output_filename_temp);


    printf("\n\nNumber of ASCIIs: %d", number_of_asciis);


    char changed_name[150];
    // %s-NUMBER OF ASCIIS-NUMBER OF USEFUL BITS AT THE PENULTIMATE BYTE
    sprintf(changed_name, "%d-%d-%s", number_of_asciis, useful_bits, output_filename);
    FILE * destination;
    FILE * source;
    destination = fopen(output_filename, "ab");
    source = fopen(output_filename_temp, "rb");

    while(fread(&data, sizeof(uint8_t), 1, source) == 1){
        fwrite(&data, sizeof(uint8_t), 1, destination);
    }

    // delete temp file
    delete_file(output_filename_temp);


    rename(output_filename, changed_name);


    fclose(destination);
    fclose(source);

    free(root);
    free(byte_buffer);

    return 0;
}