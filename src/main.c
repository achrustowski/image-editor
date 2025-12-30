#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <raymath.h>
#include "linked_list.h"
#include <stdbool.h>

#define S_W     1024
#define S_H     768

static Node* head = NULL;

static bool is_mouse_pressed = false;
static void handle_usr_input(Texture2D t, Rectangle r);
static void set_window_size(Texture2D t);
static Texture2D crop_texture(Texture2D t, Rectangle r);
static Rectangle generate_crop_area();

static void handle_usr_input(Texture2D t, Rectangle r)
{
    if (IsKeyPressed(KEY_C))
    {
        insert_last(&head, crop_texture(t, r));
    }
    if (IsKeyPressed(KEY_U))
    {
        delete_last(&head);
    }
}

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

static Texture2D crop_texture(Texture2D t, Rectangle r)
{
    Image i = LoadImageFromTexture(t);
    ImageCrop(&i, r);
    t = LoadTextureFromImage(i);
    UnloadImage(i);

    return t;
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

void drop_file(Texture2D* t)
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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(S_W, S_H, "Crop Shit");

    while(!WindowShouldClose())
    {
        drop_file(&t);

        if (!is_mouse_pressed)
        {
            crop_area = crop;
        }

        handle_usr_input(t, crop_area);

        crop = generate_crop_area();

        BeginDrawing();
        ClearBackground(BLACK);
        if (head == NULL)
        {
            set_window_size(t);
            DrawTexture(t, 0, 0, WHITE);
        } else
        {
            Node* tmp = head;
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            set_window_size(tmp->t);
            DrawTexture(tmp->t, 0, 0, WHITE);
        }
        DrawRectangleLinesEx(crop, 1.0f, RED);
        EndDrawing();
    }

    UnloadTexture(t);
    CloseWindow();

    return 0;
}
