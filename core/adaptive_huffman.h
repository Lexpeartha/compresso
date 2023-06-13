#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef COMPRESSO_ADAPTIVE_HUFFMAN_H
#define COMPRESSO_ADAPTIVE_HUFFMAN_H

typedef struct Node {
    struct Node *left_child;
    struct Node *right_child;
    struct Node *parent;

    long int weight;
    long int identifier;
    uint8_t symbol;
    int flag;
} Node;

typedef struct {
    struct Node **nodes;
    int length;
} Queue_Stack;

typedef struct {
    uint8_t byte;
    short int index;
} Byte_buffer;


Node *create_empty_tree();

void enqueue(Queue_Stack *queue, Node *node);

void push(Queue_Stack *stack, Node *node);

Node *pop(Queue_Stack *stack);

void update_identifiers(Node *root);

Node *get_node(Node *node, unsigned char symbol);

uint8_t *path_to_node(Node *root, uint8_t symbol, int *len);

void delete_file(char *filename);

void spawn_node(Node *root, uint8_t symbol);

Node *find_greater_identifier(Node *root, Node *target_node);

Node *swap_nodes(Node *node1, Node *node2);

void update_weights(Node *root, Node *mobile_node);

void update_tree(Node *root, uint8_t symbol);

void print_byte_buffer(Byte_buffer *byte_buffer, char output_filename_temp[]);

void fill_byte_buffer(Byte_buffer *byte_buffer, const uint8_t *string, int len, char output_filename_temp[]);

void transmit_code(Node *root, uint8_t symbol, int exists, Byte_buffer *byte_buffer, char output_filename[],
                   char output_filename_temp[]);

void execute_adaptive_huffman(Node *root, unsigned char symbol, Byte_buffer *byte_buffer, char output_filename[],
                              char output_filename_temp[]);

//uint8_t navigate(Node * root, const uint8_t buffer[], int buffer_len);
void remove_first_element(uint8_t *buffer, int *len);

int adaptive_huffman_decode(char *filename, char *output_file);

int adaptive_huffman_encode(char *filename, char *output_file);

uint8_t how_many_useful_bits(Byte_buffer *byte_buffer);

void free_tree2(Node *root);

#endif //COMPRESSO_ADAPTIVE_HUFFMAN_H