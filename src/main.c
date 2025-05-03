#include "defs.h"
#include "intersect.h"
#include "raylib.h"
#include "raymath.h"
#include "umla.h"
#include "umlc.h"
#include "umld_class.h"
#include "utils.h"


struct HeldState {
  struct Classe* curr; // NULL if nothing is held
};

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    struct HeldState held_state = { .curr = NULL };

    InitWindow(screenWidth, screenHeight, "floatUML");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    struct Style style;
    style.font = LoadFont("external/Consolas/consolas.ttf");
    style.fontsize = 32;

    struct Classes classes = umlc_init();
    struct Classe* a = umlc_append(&classes, create_class("Hello", 200, 200));
    struct Classe* b = umlc_append(&classes, create_class("Goodbye", 400, 400));

    umla_append(&a->attribs, create_attribute("dni", "String", 0, -1));
    umla_append(&a->attribs, create_attribute("nom", "String", 0, -1));
    umla_append(&a->attribs, create_attribute("edat", "Int", 1, 1));


    umla_append(&b->attribs, create_attribute("skjdhf", "String", 0, -1));
    umla_append(&b->attribs, create_attribute("very yes", "String", 0, -1));
    umla_append(&b->attribs, create_attribute("wahooo", "Data", 1, 1));

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < classes.len; ++i) {
          umld_class(classes.cs[i], &style);
        }

        /*
        Vector2 pt1 =
            int_seg_rect(rect_center(arect), rect_center(brect), arect);

        Vector2 pt2 =
            int_seg_rect(rect_center(arect), rect_center(brect), brect);

        DrawLineEx(pt1, pt2, 3, BLACK);
        */

        EndDrawing();

        Vector2 mpos = GetMousePosition();

        if (IsMouseButtonDown(0)) {
            if (held_state.curr) {
                held_state.curr->pos = Vector2Add(held_state.curr->pos, GetMouseDelta());
            } else {
                // Nothing is currently held
                for (int i = 0; i < classes.len; ++i) {
                    Rectangle rect = umld_rect_of(classes.cs[i]);
                    if (CheckCollisionPointRec(mpos, rect)) {
                        held_state.curr = &classes.cs[i];
                    }
                }
            }
        } else {
            held_state.curr = NULL;
        }
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
