#include "doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

Node* doubly_create_node(Texture2D* t)
{
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (!new_node)
    {
        printf("Error allocating memory\n");
        return NULL;
    }
    new_node->t = t;
    new_node->active = true;
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

void doubly_insert_first(Node** head, Texture2D* t)
{
    Node* new_node = doubly_create_node(t);

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    (*head)->active = false;
    new_node->next = *head;
    new_node->active = true;
    (*head)->prev = new_node;
    *head = new_node;
}
