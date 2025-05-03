
#include "defs.h"
#include "intersect.h"
#include "raylib.h"
#include "raymath.h"
#include "umla.h"
#include "umld_class.h"
#include "utils.h"

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "floatUML");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    struct Style style;
    style.font = LoadFont("external/Consolas/consolas.ttf");
    style.fontsize = 32;

    struct Classe a = create_class("Hello", 200, 200);

    umla_append(&a.attribs, create_attribute("dni", "String", 0, -1));
    umla_append(&a.attribs, create_attribute("nom", "String", 0, -1));
    umla_append(&a.attribs, create_attribute("edat", "Int", 1, 1));

    struct Classe b = create_class("Hello", 400, 400);

    umla_append(&b.attribs, create_attribute("dni", "String", 0, -1));
    umla_append(&b.attribs, create_attribute("nom", "String", 0, -1));
    umla_append(&b.attribs, create_attribute("edat", "Int", 1, 1));

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        Rectangle arect = umld_class(a, &style);
        Rectangle brect = umld_class(b, &style);

        Vector2 pt1 =
            int_seg_rect(rect_center(arect), rect_center(brect), arect);

        Vector2 pt2 =
            int_seg_rect(rect_center(arect), rect_center(brect), brect);

        DrawLineEx(pt1, pt2, 3, BLACK);

        EndDrawing();

        Vector2 mpos = GetMousePosition();

        if (CheckCollisionPointRec(mpos, arect) && IsMouseButtonDown(0)) {
            a.pos = Vector2Add(a.pos, GetMouseDelta());
        }

        if (CheckCollisionPointRec(mpos, brect) && IsMouseButtonDown(0)) {
            b.pos = Vector2Add(b.pos, GetMouseDelta());
        }
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
