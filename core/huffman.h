//
// Created by harry on 4/24/23.
//

#ifndef SPECIJALNAGRUPA_MAIN_H
#define SPECIJALNAGRUPA_MAIN_H

#define MAX_LENGTH 500
#define MAX_CHILDREN 100

struct Node {
    struct Node * parent;
    struct Node * children[MAX_CHILDREN];
    int childrenCount;

    int index;
    int weight;
};

#endif //SPECIJALNAGRUPA_MAIN_H
