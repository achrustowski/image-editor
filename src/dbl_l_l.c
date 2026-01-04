#include "dbl_l_l.h"
#include <stdio.h>
#include <stdlib.h>

Node* make_node(Command c)
{
    Node* node = (Node*)malloc(sizeof(Node));

    if (node == NULL)
    {
        printf("Memory allocation error\n");
        return NULL;
    }

    node->cmd = c;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void free_from(Node* n)
{
    while (n)
    {
        Node* next = n->next;
        free(n);
        n = next;
    }
}

/*** Perform a new command; if we are not at tail, truncate redo part ***/
void history_perform(History* h, Command c)
{
    // IF cursor IS NOT NULL, cut_from is assigned a command onward from which (h->cursor->next)
    // everything is invalid, otherwise assign head - everything is redo (delete all) 
    Node* cut_from = (h->cursor) ? h->cursor->next : h->head;

    // if there is redo history (we are not at tail)
    if (cut_from)
    {
        // disconnecting the list - logically detaching redo history from valid history
        if (cut_from->prev)
        {
            // cutting in the middle; cut_from is the command ownard from which (including itself)
            // we want to delete the redo history, we need to set cut_from->prev->next to NULL
            // in order to modify the entire linked list and not just the local variable cut_from
            // like we would have done if we were to set cut_from = NULL
            cut_from->prev->next = NULL;
        } else
        {
            // cutting everything; in the case when cut_from is the first element in redo history
            // cut_from->prev is NULL, so we must reset the list by doing
            h->head = NULL;
        }
        free_from(cut_from);
        h->tail = h->cursor;
    }

    // Append new command (node); new node is not applied yet, just created
    Node* n = make_node(c);

    // if the list is empty; this can be the case if the list was empty originally or cleared already
    if (!h->head)
    {
        h->head = n;
        h->tail = n;
    } else
    // non-empty list case;     
    {
        // put n at the end of the list (tail->next)
        h->tail->next = n;
        // link n->prev to the tail of the list
        n->prev = h->tail;
        // set tail of the list to be n
        h->tail = n;
    }

    // Apply command and move cursor
    c.do_fn(h->edit, c.payload);
    // this makes sure that all commands up to cursor are applied
    h->cursor = n;
}
