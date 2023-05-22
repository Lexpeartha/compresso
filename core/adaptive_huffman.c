#include "adaptive_huffman.h"
#define OUTPUT "output.bin"
#define DECOMPRESSED "decompressed.bin"
#define BUFFER_SIZE 100
#define NYT 3 // a randomly picked number that won't occur in files
#define INTERNAL_NODE 4 // also a random number, used to identify what is an internal node

int number_of_nodes = 0;

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
    queue->nodes = malloc(number_of_nodes * sizeof(Node *));
    stack->nodes = malloc(number_of_nodes * sizeof(Node *));
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
}


// returns the node that has the target symbol
Node * get_node(Node * node, long int symbol){
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
char * path_to_node(Node * root, long int symbol){
    Node * found_node = get_node(root, symbol);
    Node * next_node = malloc(sizeof(Node));
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

    return reverse;
}

// deletes a file
void delete_file(char * filename){
    if (remove(filename) != 0){
        // if nothing is deleted
        printf("No file was deleted.");
    }
}

void transmit_code(Node * root, long int symbol, int exists){
    if(root->left_child == NULL && root->right_child == NULL){
        // the output file needs to be deleted first, because codes are appended
        delete_file(OUTPUT);
    }

    FILE * out;
    out = fopen(OUTPUT, "ab");
    if(out == NULL){
        printf("Can't open file.");
        exit(1);
    }
    size_t elements_written = 0;

    char * buffer = malloc(BUFFER_SIZE * sizeof(char));
    long ascii_code;

    // if the tree is empty
    if(root->left_child == NULL && root->right_child == NULL){
        ascii_code = symbol;
        elements_written = fwrite(&ascii_code, sizeof(long int), 1, out);
    }
    // if it's the first occurrence
    else if (exists == 0){

        // find NYT node
        strcpy(buffer, path_to_node(root, NYT));
        for(int j = 0; j < strlen(buffer); j++){
            ascii_code = buffer[j];
            elements_written = fwrite(&ascii_code, sizeof(long int), 1, out);
        }

        // print out the new code for the symbol
        ascii_code = symbol;
        elements_written += fwrite(&ascii_code, sizeof(long int), 1, out);
    }
    // if a char already present in the tree is transmitted
    else {
        strcpy(buffer, path_to_node(root, symbol));
        for(int j = 0; j < strlen(buffer); j++){
            ascii_code = buffer[j];
            elements_written = fwrite(&ascii_code, sizeof(long int), 1, out);
        }
    }
    fclose(out);
    if(elements_written == 0){
        printf("No elements written!");
        exit(2);
    }
}

// a function that 'extends' the NYT node; NYT becomes an internal node R, the left child becomes
// the new NYT, and the right one is for the symbol S
void spawn_node(Node * root, long int symbol){
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

    Node ** eligible_nodes = malloc(number_of_nodes * sizeof(Node **));
    int eligible_nodes_length = 0;

    // level order traversal; searching for a node that has the same weight but greater id
    Node * temp = root;
    while(temp != NULL){
        if(temp->weight == target_node->weight && temp->identifier > target_node->identifier){
            eligible_nodes[eligible_nodes_length++] = temp;
        }
        if(temp->left_child != NULL) enqueue(queue, temp->left_child);
        if(temp->right_child != NULL) enqueue(queue, temp->right_child);
        temp = dequeue(queue);
    }


    int max = 0;
    for(int i = 0; i < eligible_nodes_length; i++){
        if(eligible_nodes[i]->identifier > eligible_nodes[max]->identifier) max = i;
    }
    if(eligible_nodes_length > 0) {
        return eligible_nodes[max];
    }
    else return NULL;
}

// swaps two nodes in the tree
Node * swap_nodes(Node * node1, Node * node2){

    if(node1 == NULL || node2 == NULL){
        // unexpected error
        return NULL;
    }

    // swapping symbols
    long int temp_symbol = node1->symbol;
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

void update_tree(Node * root, long int symbol){
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
void execute_adaptive_huffman(Node * root, char symbol){
    // symbol already exists
    Node * found_node = get_node(root, symbol);
    char node_symbol = NYT;
    
    if (found_node != NULL){
        transmit_code(root, symbol, 1);
        node_symbol = symbol;
    }
    // symbol does NOT exist
    else {
        transmit_code(root, symbol, 0);
        spawn_node(root, symbol);
    }
    update_tree(root, node_symbol);

}

long int navigate(Node * root, const char * buffer, int buffer_len){
    Node * temp = root;
    for(int i = 0; i < buffer_len; i++){
        if(buffer[i] == '0') temp = temp->left_child;
        else if(buffer[i] == '1') temp = temp->right_child;
    }
    return temp->symbol;
}

int adaptive_huffman_decode(char * filename){

    delete_file(DECOMPRESSED);

    FILE * in;
    FILE * out;
    in = fopen(OUTPUT, "rb");
    out = fopen(filename, "ab");
    if(in == NULL){
        printf("Can't open file.");
        exit(1);
    }
    if(out == NULL){
        printf("Can't open file.");
        exit(1);
    }

    long int data;

    number_of_nodes = 0;
    Node * root = create_empty_tree();
    char * buffer = malloc(BUFFER_SIZE * sizeof(char));
    int buffer_len = 0;


    // reading from a binary file
    while(fread(&data, sizeof(long int), 1, in) == 1){
        if(number_of_nodes == 1){
            spawn_node(root, data);
            fwrite(&data, sizeof(long int), 1, out);
        }
        else if ((char) data == '0' || (char) data == '1') {
            buffer[buffer_len++] = (char) data;
            long int navigate_result = navigate(root, buffer, buffer_len);
            if(navigate_result == NYT){
                // next data line should be printed; spawn and write the next char, update tree
                fread(&data, sizeof(long int), 1, in);
                spawn_node(root, data);
                fwrite(&data, sizeof(long int), 1, out);
                buffer_len = 0;
                update_tree(root, NYT);
            }
            else if (navigate_result != INTERNAL_NODE){
                // write char and update tree
                fwrite(&navigate_result, sizeof(long int), 1, out);
                update_tree(root, navigate_result);
                buffer_len = 0;
            }
        }

    }

    fclose(in);
    fclose(out);

    return 0;
}

// adaptive_huffman_encode
int adaptive_huffman_encode(){

    Node * root = create_empty_tree();

    char example[1000];

    // example input
    strcpy(example, "test123");

    for(int i = 0; i < strlen(example); i++){
        execute_adaptive_huffman(root, example[i]);
    }

    // DELETE LATER
    adaptive_huffman_decode(DECOMPRESSED);

    return 0;
}