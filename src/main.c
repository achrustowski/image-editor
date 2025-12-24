#include <stdio.h>
#include <raylib.h>

#define S_W     1024
#define S_H     768

Texture2D texture = {0};

void load_image(const char* filename)
{
    Image image = LoadImage(filename);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

int main()
{
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
                        printf("Texture loaded\n");
                        UnloadTexture(texture);
                        load_image(file_path);
                    } else if (!IsTextureValid(texture))
                    {
                        printf("No texture loaded\n");
                        load_image(file_path);
                    }
                } else
                {
                    printf("Incorrect file extension\n");
                }
            }
            UnloadDroppedFiles(dropped_files);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
