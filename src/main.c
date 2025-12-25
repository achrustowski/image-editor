#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <raymath.h>

#define S_W     1024
#define S_H     768

Texture2D texture = {0};
bool is_mouse_pressed = false;

bool is_vector_zero(Vector2 v)
{
    if (v.x == 0 && v.y == 0)
    {
        return true;
    } else
    {
        return false;
    }
}

void load_image(const char* filename)
{
    Image image = LoadImage(filename);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

Rectangle generate_crop_area()
{
    static Vector2 start = {0};
    static Vector2 end = {0};
    static bool dragging = false;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        start = GetMousePosition();
        end = start;
        dragging = true;
    }

    if (dragging)
    {
        end = GetMousePosition();
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
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

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(S_W, S_H, "Crop Shit");

    while(!WindowShouldClose())
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
                    if (IsTextureValid(texture))
                    {
                        UnloadTexture(texture);
                        load_image(file_path);
                    } else if (!IsTextureValid(texture))
                    {
                        load_image(file_path);
                    }
                } else
                {
                    printf("Incorrect file extension\n");
                }
            }
            UnloadDroppedFiles(dropped_files);
        }

        if (IsTextureValid(texture))
        {
            SetWindowSize(texture.width, texture.height);
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        DrawRectangleLinesEx(generate_crop_area(), 1.0f, RED);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
