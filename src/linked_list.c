#include "linked_list.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

Node* create_node(Texture2D t)
{
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (!new_node)
    {
        printf("Memory allocation error\n");
        return NULL;
    }

    new_node->t = t;
    new_node->next = NULL;

    return new_node;
}

void insert_first(Node** head, Texture2D t)
{
    Node* new_node = create_node(t);

    new_node->next = *head;
    *head = new_node;
}

void insert_last(Node** head, Texture2D t)
{
    Node* new_node = create_node(t);

    if (*head == NULL)
    {
        *head = new_node;
    } else
    {
        Node* tmp = *head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = new_node;
    }
}

void delete_last(Node** head)
{
    if (*head == NULL)
    {
        printf("List is empty\n");
        return;
    }
    if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }
    Node* tmp = *head;
    while (tmp->next->next != NULL)
    {
        tmp = tmp->next;
    }
    free(tmp->next);
    tmp->next = NULL;
}
