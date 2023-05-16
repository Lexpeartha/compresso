#include "adaptive_huffman.h"
#define OUTPUT "output.bin"
#define PRIVREMENO 1000
#define BUFFER_SIZE 100
#define NYT 3 // neki tamo bezveze broj koji nece da se pojavljuje
#define INTERNAL_NODE 4 // neki tamo bezveze broj da identifikuje R, tj. interni čvor
/*
 * TO-DO:
 *
 * - obrati pažnju na dinamičku alokaciju memorije (naročito na koren stabla)
 * - pazi na memory leak-ove! Oslobodi memoriju.
 *
 */


// SVE TRI FUNKCIJE PODRAZUMEVAJU DA JE
// VEKTOR [0, N - 1]
int parent(int index){
    return (index - 1)/2;
}

int left_child(int index){
    return (index * 2) + 1;
}

int right_child(int index){
    return (index * 2) + 2;
}
// -------------------------------------


// umetanje levog deteta za dati index roditelja
void insert_left(Tree * root, int parent_index){
    if(root->nodes[parent_index].symbol == '\0'){
        printf("Invalid parent index! Can't insert to the left.");
    }
    else {
        int child_index = left_child(parent_index);
        root->nodes[child_index].symbol = NYT;
        root->nodes[child_index].weight = 0;

        // povecaj velicinu root-a ako je index deteta veci od duzine
        if(child_index > root->len) root->len = child_index + 1;
    }
}

// umetanje desnog deteta za dati index roditelja
void insert_right(Tree * root, int parent_index, char symbol){
    if(root->nodes[parent_index].symbol == '\0'){
        printf("Invalid parent index! Can't insert to the right.");
    }
    else {
        int child_index = right_child(parent_index);
        root->nodes[child_index].symbol = symbol;
        root->nodes[child_index].weight = 1;

        // povecaj velicinu root-a ako je index deteta veci od duzine
        if(child_index > root->len) root->len = child_index + 1;
    }
}

int node_exists(Tree * root, char target){
    // da li će prazna mesta da prave problem?

    int end = root->len - 1;
    for(int i = end; i >= 0; i--){
        if(root->nodes[i].symbol == target) return 1;
    }
    return 0;
}

Tree * create_empty_tree(){
    // inicijalizovanje praznog stabla

    Tree * root = malloc(sizeof(Tree));
    root->nodes = malloc(PRIVREMENO * sizeof(Node));
    root->len = 1;


    // NYT ima težinu 0 i identifikator 0
    root->nodes[0].symbol = NYT;
    root->nodes[0].identifier = 0;
    root->nodes[0].weight = 0;

    return root;
}

Tree * update_tree(Tree * root){
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

// nalazi index lista u stablu
int find_node_index(Tree * root, char target){

    int end = root->len - 1;
    for(int i = end; i >= 0; i--){
        if(root->nodes[i].symbol == target) return i;
    }

    // neočekivana greška ako se ovo vrati
    return 0;
}


// ispiše kod lista.
// algoritam ide otpozadi, dok god ne naiđe na koren.
char * path_to_node(Tree * root, char symbol){
    int index = find_node_index(root, symbol);
    int new_index;
    char * buffer = malloc(BUFFER_SIZE * sizeof(char));
    int buffer_length = 0;

    while (index != 0){
        new_index = parent(index);
        if(left_child(new_index) == index){
            buffer[buffer_length++] = '0';
        }
        else {
            buffer[buffer_length++] = '1';
        }
        index = new_index;
    }

    // obrtanje stringa, pošto je ispisan naopačke
    char * reverse = malloc(buffer_length* sizeof(char));
    int count = 0;

    for(int i = buffer_length - 1; i >= 0; i--){
        reverse[count++] = buffer[i];
    }
    free(buffer);

    return reverse;
}

void transmit_code(Tree * root, char symbol){
    char * buffer = malloc(BUFFER_SIZE * sizeof(char));

    // ako je stablo prazno
    if(root->len == 1){
        strcpy(buffer, "00");
        print_code(buffer);
    }
    // ako je prvo pojavljivanje
    else if (symbol == NYT){

        // pronadji NYT
        strcpy(buffer, path_to_node(root, symbol));
        print_code(buffer);

        // odstampaj takodje i novi kod simbola (bice isti kao putanja do NYT + '1')
        strcat(buffer, "1");
        print_code(buffer);
    }
    // ako se transmituje char koji je već u stablu
    else {
        strcpy(buffer, path_to_node(root, symbol));
        print_code(buffer);
    }
}

void update_indices(Tree * root){
    // ...
}

// funkcija koja proširuje NYT; NYT postaje interni čvor R, levo dete postaje NYT, a desno postaje simbol S
void spawn_node(Tree * root, char symbol){
    int nyt_index = find_node_index(root, NYT);
    insert_left(root, nyt_index);
    insert_right(root, nyt_index, symbol);
    root->nodes[nyt_index].symbol = INTERNAL_NODE;
    root->nodes[nyt_index].weight = 1;

    update_indices(root);
}

// procesuiranje jednog karaktera huffmanov-im algoritmom
void execute_adaptive_huffman(Tree * root, char symbol){
    // simbol vec postoji
    if(node_exists(root, symbol)){
        transmit_code(root, symbol);
    }
        // simbol NE postoji
    else {
        transmit_code(root, NYT);
        spawn_node(root, symbol);
    }
    update_tree(root);
}

// adaptive_huffman_start
int adaptive_huffman_start(){

    Tree * root = create_empty_tree();
    char example[14];
    // primer sa ovog klipa:
    // https://www.youtube.com/watch?v=rsjIjuVCJ0U&list=PLN1c1qxGrBmHYQLE1LiGy3H9kHQaOJN2F&index=17
    // ABCCCDDDCABBB
    strcpy(example, "ABCCCDDDCABBB");

    for(int i = 0; i < strlen(example); i++){
        execute_adaptive_huffman(root, example[i]);
    }

    return 0;
}