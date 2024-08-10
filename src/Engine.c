#include "engine.h"
#include "filestream.h"
#include "umb.h"
#include <raylib.h>
#include <stdio.h>
#include <malloc.h>

void engine_update()
{

}

bool engine_button(Rectangle rect, Color color, Color hoverColor, Color pressColor, Font font, const char* text, float fontSize, Color textColor)
{
    bool pressed = false;

    if (CheckCollisionPointRec(GetMousePosition(), rect))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            DrawRectangleRec(rect, pressColor);
        }
        else
        {
            DrawRectangleRec(rect, hoverColor);
        }

        pressed = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }
    else
    {
        DrawRectangleRec(rect, color);
    }

    DrawTextEx(font, text, (Vector2){rect.x + (rect.width / 4), rect.y + (rect.height / 4)}, fontSize, 0, textColor);

    return pressed;
}

bool engine_default_button(Rectangle rect, Font defaultFont, const char* text)
{
    return engine_button(rect, (Color){30, 30, 30, 255}, (Color){25, 25, 25, 255}, (Color){20, 20, 20, 255}, defaultFont, text, 24, WHITE);
}

void engine_init()
{
    InitWindow(1200, 720, "Foursaken Repacker");
    SetTargetFPS(60);

    Font defaultFont = LoadFontEx("gfx/comic book.ttf", 36, 0, 250);

    SetTextLineSpacing(16);

    Camera camera = {0};

    camera.position = (Vector3){ 0, 0, 0 };
    camera.target = (Vector3){ 0, 0, 0 };
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 60;
    camera.projection = CAMERA_PERSPECTIVE;

    Stream* stream = stream_create("dog.umb");
    UMB* umb = umb_from_stream(stream);

    stream_close(stream);
    Mesh* meshes = umb_to_meshes(umb);

    int objectCount = umb->numObjects;
    umb_delete(umb);

    Texture2D texture = LoadTexture("pvrc_dog texture bloody.png");
    Material material = LoadMaterialDefault();

    Matrix identity = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, texture);

    while (!WindowShouldClose())
    {
        engine_update();
        BeginDrawing();

        ClearBackground((Color){40, 40, 40, 255});
        DrawRectangleRec((Rectangle){350, 60, 800, 600}, (Color){60, 60, 60, 255});

        if (engine_default_button((Rectangle){0, 0, 150, 60}, defaultFont, "Open"))
        {
            printf("open");
        }

        BeginMode3D(camera);

        for (int i = 0; i < objectCount; i++)
        {
            DrawMesh(meshes[i], material, identity);
        }

        EndMode3D();

        Vector2 pos = { 300, 360 };
        EndDrawing();
    }

    for (int i = 0; i < objectCount; i++)
    {
        UnloadMesh(meshes[i]);
    }

    free(meshes);

    UnloadFont(defaultFont);
    UnloadTexture(texture);

    UnloadMaterial(material);

    CloseWindow();
}