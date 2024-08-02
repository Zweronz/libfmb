#include <Engine.h>
#include <raylib.h>
#include <stdio.h>

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

void engine_init()
{
    InitWindow(1200, 720, "Foursaken Repacker");
    SetTargetFPS(60);

    Font defaultFont = LoadFontEx("gfx/comic book.ttf", 36, 0, 250);

    SetTextLineSpacing(16);

    while (!WindowShouldClose())
    {
        engine_update();
        BeginDrawing();

        ClearBackground((Color){40, 40, 40, 255});
        DrawRectangleRec((Rectangle){350, 60, 800, 600}, (Color){60, 60, 60, 255});

        if (engine_button((Rectangle){0, 0, 150, 60}, (Color){30, 30, 30, 255}, (Color){25, 25, 25, 255}, (Color){20, 20, 20, 255}, defaultFont, "Open", 24, WHITE))
        {
            printf("open");
        }

        Vector2 pos = { 300, 360 };
        EndDrawing();
    }

    UnloadFont(defaultFont);

    CloseWindow();
}