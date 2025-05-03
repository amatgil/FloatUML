#include "defs.h"
#include "intersect.h"
#include "raylib.h"
#include "raymath.h"
#include "umla.h"
#include "umlc.h"
#include "umld_class.h"
#include "umlr.h"
#include "utils.h"

struct HeldState {
    struct Classe *curr; // NULL if nothing is held
};

void startup_example(struct World *w) {
    w->classes = umlc_init();
    w->relacions = umlrs_init();

    w->style.font = LoadFont("external/Consolas/consolas.ttf");
    w->style.fontsize = 32;

    struct Classe *a =
        umlc_append(&w->classes, create_class("Hello", 200, 200, NULL));
    struct Classe *b =
        umlc_append(&w->classes, create_class("Goodbye", 400, 400, NULL));
    struct Classe *c =
        umlc_append(&w->classes, create_class("Third option", 600, 350, NULL));

    umla_append(&a->attribs, create_attribute("dni", "String", 0, -1));
    umla_append(&a->attribs, create_attribute("nom", "String", 0, -1));
    umla_append(&a->attribs, create_attribute("edat", "Int", 1, 1));

    umla_append(&b->attribs, create_attribute("skjdhf", "String", 0, -1));
    umla_append(&b->attribs, create_attribute("very yes", "String", 0, -1));
    umla_append(&b->attribs, create_attribute("wahooo", "Data", 1, 1));

    umla_append(&c->attribs, create_attribute("dia", "String", 0, -1));
    umla_append(&c->attribs, create_attribute("existencia", "Int", 0, -1));

    struct Relacio r1 = umlr_init();
    umlr_append(&r1, a);
    umlr_append(&r1, b);
    umlrs_append(&w->relacions, r1);

    struct Relacio r2 = umlr_init();
    umlr_append(&r2, b);
    umlr_append(&r2, c);
    umlrs_append(&w->relacions, r2);
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    struct HeldState held_state = {.curr = NULL};

    InitWindow(screenWidth, screenHeight, "floatUML");

    SetTargetFPS(60);

    struct World w;
    startup_example(&w);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < w.classes.len; ++i)
            umld_class(w.classes.cs[i], &w.style);

        for (int i = 0; i < w.relacions.len; ++i) {
            struct Relacio r = w.relacions.rs[i];
            if (r.len == 2) {
                struct Classe *a = r.cs[0];
                struct Classe *b = r.cs[1];
                Rectangle arect = umld_rect_of(*a, &w.style);
                Rectangle brect = umld_rect_of(*b, &w.style);
                Vector2 pt1 =
                    int_seg_rect(rect_center(arect), rect_center(brect), arect);

                Vector2 pt2 =
                    int_seg_rect(rect_center(arect), rect_center(brect), brect);

                DrawLineEx(pt1, pt2, 3, BLACK);
            }
        }

        EndDrawing();

        if (IsMouseButtonDown(0)) {
            if (held_state.curr) {
                held_state.curr->pos =
                    Vector2Add(held_state.curr->pos, GetMouseDelta());
            } else {
                Vector2 mpos = GetMousePosition();
                for (int i = 0; i < w.classes.len; ++i) {
                    Rectangle rect = umld_rect_of(w.classes.cs[i], &w.style);
                    if (CheckCollisionPointRec(mpos, rect)) {
                        held_state.curr = &w.classes.cs[i];
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
