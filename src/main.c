#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <raymath.h>
#include <stdbool.h>
#include "doubly_linked_list.h"

#define S_W     1024
#define S_H     768

static bool is_mouse_pressed = false;
static void drop_file(Node** head, Texture2D* t);
//static void handle_usr_input(Node** head, Texture2D* t, Rectangle r);
static void set_window_size(Texture2D t);
static void crop_texture(Texture2D* t, Rectangle r);
static Rectangle generate_crop_area();

static void set_window_size(Texture2D t)
{
    static int last_w = 0;
    static int last_h = 0;

    if (!IsTextureValid(t)) return;

    if (t.width != last_w || t.height != last_h)
    {
        SetWindowSize(t.width, t.height);
        last_w = t.width;
        last_h = t.height;
    }
}

static void crop_texture(Texture2D* t, Rectangle r)
{
    Image i = LoadImageFromTexture(*t);
    ImageCrop(&i, r);
    UnloadTexture(*t);
    *t = LoadTextureFromImage(i);
    UnloadImage(i);
}

static Rectangle generate_crop_area()
{
    static Vector2 start = {0};
    static Vector2 end = {0};
    static bool dragging = false;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        start = GetMousePosition();
        end = start;
        dragging = true;
    }
    if (dragging)
    {
        end = GetMousePosition();
    }
    if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
    {
        dragging = false;
    }

    float x = fminf(start.x, end.x);
    float y = fminf(start.y, end.y);
    float w = fabsf(start.x - end.x);
    float h = fabsf(start.y - end.y);

    if (w == 0 || h == 0)
    {
        return (Rectangle) {0};
    } else
    {
        return (Rectangle) {x, y, w, h};
    }
}

void drop_file(Node** head, Texture2D* t)
{
    if (IsFileDropped())
    {
        FilePathList dropped_files = LoadDroppedFiles();
        if (dropped_files.count > 0)
        {
            const char* file_path = dropped_files.paths[0];
            if (
                    IsFileExtension(file_path, ".jpg") ||
                    IsFileExtension(file_path, ".jpeg") ||
                    IsFileExtension(file_path, ".png") ||
                    IsFileExtension(file_path, ".gif")
               )
            {
                if (IsTextureValid(*t))
                {
                    UnloadTexture(*t);    
                    *t = LoadTexture(file_path);
                } else if (!IsTextureValid(*t))
                {
                    *t = LoadTexture(file_path);
                }
            } else
            {
                printf("Incorrect file extension\n");
            }
        }
        UnloadDroppedFiles(dropped_files);
    }
}

int main()
{
    Rectangle crop = {0};
    Rectangle crop_area = {0};
    Texture2D t = {0};
    Node* head = NULL;


    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(S_W, S_H, "Crop Shit");

    while(!WindowShouldClose())
    {
        drop_file(&head, &t);

        if (!is_mouse_pressed)
        {
            crop_area = crop;
        }

        crop = generate_crop_area();

        BeginDrawing();
        ClearBackground(BLACK);
        set_window_size(t);
        DrawTexture(t, 0, 0, WHITE);
        DrawRectangleLinesEx(crop, 1.0f, RED);
        EndDrawing();
    }

    UnloadTexture(t);
    CloseWindow();

    return 0;
}
