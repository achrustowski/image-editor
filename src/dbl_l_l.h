#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <raylib.h>
#include <stdbool.h>

/*** Texture state ***/
typedef struct
{
    Texture2D texture;
} Edit;

/*** Command interface ***/
typedef void (*Do_Fn)(Edit*, void*);
typedef void (*Undo_Fn)(Edit*, void*);

typedef struct
{
    Do_Fn do_fn;
    Undo_Fn undo_fn;
    void* payload;
} Command;

/*** Doubly linked list node for history ***/
typedef struct Node
{
    Command cmd;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct History
{
    Node* head;       // first command node (after Start)
    Node* tail;       // last command node
    Node* cursor;     // last-applied command (NULL means Start)
    Edit* edit;       // target command
} History;

/*** Helpers *///
Node* make_node(Command c);
void free_from(Node* n);

void history_perform(History* h, Command c);
int history_undo(History* h);
int history_redo(History* h);

/*** Example command: crop an image ***/
typedef struct
{
    Texture2D before;   // texture before cropping (for undo)
    Rectangle crop;     // crop rectangel to apply
    int applied;        // flag to avoid double-free issues
} Crop_Payload;

void crop_do(Edit*, void* payload);
void crop_undo(Edit*, void* payload);

Command make_crop_command(Rectangle crop);

#endif
