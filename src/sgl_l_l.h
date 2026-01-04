#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <raylib.h>

typedef struct Node Node;

struct Node
{
    Texture2D t;
    Node* next;
};

Node* singly_create_node(Texture2D t);
void singly_insert_first(Node** head, Texture2D t);
void singly_insert_last(Node** head, Texture2D t);
void singly_delete_last(Node** head);

#endif
