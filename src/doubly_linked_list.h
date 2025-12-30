#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <raylib.h>
#include <stdbool.h>

typedef struct Node Node;

struct Node
{
    Texture2D* t;
    bool active;
    Node* prev;
    Node* next;
};

Node* doubly_create_node(Texture2D* t);
void doubly_insert_first(Node** head, Texture2D* t);

#endif
