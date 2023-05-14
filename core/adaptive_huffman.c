#include "adaptive_huffman.h"
#define OUTPUT "output.bin"


int node_exists(Node * root, char target){
    return 0;
}

Node * create_empty_tree(){
    // inicijalizovanje praznog stabla

    Node * root = malloc(sizeof(Node));
    root->right_child = NULL;
    root->left_child = NULL;
    root->parent = NULL;

    // NYT ima težinu 0 i identifikator 0
    root->symbol = 0;
    root->identifier = 0;
    root->weight = 0;

    return root;
}

Node * update_tree(Node * root){
    return root;
}

void print_code(char * buffer){
    FILE * out;
    out = fopen(OUTPUT, "wb");
    if(out == NULL){
        // GRESKA 1
    }
    size_t elements_written = 0;

    elements_written = fwrite(buffer, sizeof(char), strlen(buffer) * sizeof(char), out);

    fclose(out);
    if(elements_written == 0){
        // GRESKA 2, ako nista nije upisano u fajl
    }
}

char * transmit_code(Node * root, char symbol){
    char * buffer = malloc(100 * sizeof(char));

    if(root->left_child == NULL && root->right_child == NULL){
        // ako je stablo prazno
        strcpy(buffer, "00");

        return NULL;
    }
    else if (symbol == 0){
        // ako se transmituje NYT

        // ovo će morati da se izmeni posle
        return NULL;
    }
    else {
        // ako se transmituje char koji je već u stablu

        return NULL;
    }

}

void spawn_node(Node * root, char symbol){

}

void execute_adaptive_huffman(Node * root, char symbol){
    if(node_exists(root, symbol)){
        // simbol vec postoji
        transmit_code(root, symbol);
    }
    else {
        // simbol NE postoji
        char * buffer = transmit_code(root, 0);
        print_code(buffer);
        spawn_node(root, symbol);
    }
    update_tree(root);
}

// adaptive_huffman_start
int adaptive_huffman_start(){

    Node * root = create_empty_tree();


    return 0;
}