#include "defs.h"
#include "intersect.h"
#include "raylib.h"
#include "raymath.h"
#include "umla.h"
#include "umlc.h"
#include "umld_class.h"
#include "umlr.h"
#include "utils.h"

typedef int32_t Bool;

#define MAX_TEXT_IN_TEXTAREA 3000
#define PERCENTATGE_MIDA_TEXTBOX 3 // width/PERCENTATGE_MIDA_TEXTBOX

struct State {
    struct Classe *curr_held; // NULL if nothing is held
    Bool textbox_up;
    char textbox_text[MAX_TEXT_IN_TEXTAREA];
    uint32_t text_cursor;
    uint64_t frames_counter;
};

void startup_example(struct World *w) {
    w->classes = umlc_init();
    w->relacions = umlrs_init();

    w->style.font = LoadFont("external/Consolas/consolas.ttf");
    w->style.fontsize = 22;

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
    umlr_append(&r1, a, 7, 10);
    umlr_append(&r1, b, 0, 200);
    umlrs_append(&w->relacions, r1);

    struct Relacio r2 = umlr_init();
    umlr_append(&r2, b, 1, 10);
    umlr_append(&r2, c, 2, -1);
    umlrs_append(&w->relacions, r2);
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    struct State st = {
        .curr_held = NULL,
        .textbox_up = true,
        .text_cursor = 0,
        .textbox_text = {[0 ... 1023] = ' '},
    };
    Rectangle textarea = {screenWidth - screenWidth / PERCENTATGE_MIDA_TEXTBOX, 0, screenWidth / PERCENTATGE_MIDA_TEXTBOX,
                          screenHeight};

    InitWindow(screenWidth, screenHeight, "floatUML");

    SetTargetFPS(60);

    struct World w;
    startup_example(&w);

    while (!WindowShouldClose())
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

        if (st.textbox_up) {
            Bool mouseOnText;
            if (CheckCollisionPointRec(GetMousePosition(), textarea)) mouseOnText = true;
            else mouseOnText = false;

            DrawRectangleRec(textarea, BLACK);
            DrawText(st.textbox_text, (int)textarea.x + 5, (int)textarea.y + 8,
                     w.style.fontsize, MAROON);
            if (mouseOnText) {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0) {
                    // Only ascii!!
                    if ((key >= 32) && (key <= 125) &&
                        (st.text_cursor < MAX_TEXT_IN_TEXTAREA)) {
                        st.textbox_text[st.text_cursor] = (char)key;
                        st.textbox_text[st.text_cursor + 1] = '\0';
                        st.text_cursor++;
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (st.text_cursor > 0) {
                        st.text_cursor--;
                        st.textbox_text[st.text_cursor] = '\0';
                    }
                }
            } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

            if (st.text_cursor < MAX_TEXT_IN_TEXTAREA) {
                if (((st.frames_counter++ / 20) % 2) == 0)
                    DrawText("_",
                             textarea.x + 8 +
                                 MeasureText(st.textbox_text, w.style.fontsize),
                             textarea.y + 12, w.style.fontsize, MAROON);
            }
        }

        EndDrawing();

        if (IsMouseButtonDown(0)) {
            if (st.curr_held) {
                st.curr_held->pos =
                    Vector2Add(st.curr_held->pos, GetMouseDelta());
            } else {
                Vector2 mpos = GetMousePosition();
                for (int i = 0; i < w.classes.len; ++i) {
                    Rectangle rect = umld_rect_of(w.classes.cs[i], &w.style);
                    if (CheckCollisionPointRec(mpos, rect)) {
                        st.curr_held = &w.classes.cs[i];
                    }
                }
            }
        } else st.curr_held = NULL;

        if (IsKeyPressed(KEY_F10)) st.textbox_up = !st.textbox_up;
        
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
