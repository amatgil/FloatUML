#include "defs.h"
#include "intersect.h"
#include "raylib.h"
#include "raymath.h"
#include "umla.h"
#include "umlc.h"
#include "umld_class.h"
#include "umld_rel.h"
#include "umlr.h"
#include "utils.h"

typedef int32_t Bool;

#define MAX_TEXT_IN_TEXTAREA 2000
#define PERCENTATGE_MIDA_TEXTBOX 3 // width/PERCENTATGE_MIDA_TEXTBOX
#define TEXTBOX_LEFTPAD 8
#define TEXTBOX_VERTPAD 5
#define TEXT_COLOR RAYWHITE

struct State {
    struct Classe *curr_held; // NULL if nothing is held
    Bool textbox_up;
    char textbox_text[MAX_TEXT_IN_TEXTAREA];
    uint32_t text_cursor;
    uint32_t text_final_index;
    uint64_t frames_counter;
};

struct World startup_example() {
    struct World w = umlw_init("external/Consolas/consolas.ttf", 22);

    struct Classe *a =
        umlc_append(&w.classes, create_class("Hello", 200, 200, NULL));
    struct Classe *b =
        umlc_append(&w.classes, create_class("Goodbye", 400, 400, NULL));
    struct Classe *c =
        umlc_append(&w.classes, create_class("Third option", 600, 350, NULL));

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
    umlr_append(&r1, c, 0, -1);
    umlrs_append(&w.relacions, r1);

    struct Relacio r2 = umlr_init();
    umlr_append(&r2, b, 1, 10);
    umlr_append(&r2, c, 2, -1);
    umlrs_append(&w.relacions, r2);
    return w;
}

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 

    const int true_screen_width = 1000;
    const int true_screen_height = 1000;

    int screenWidth = 1000;
    int screenHeight = 1000;
    float width_textarea = (float)screenWidth / (float)PERCENTATGE_MIDA_TEXTBOX;
    Rectangle textarea = {screenWidth - width_textarea, 0, width_textarea,
                          screenHeight};

    struct State st = {
        .curr_held = NULL,
        .textbox_up = true,
        .text_cursor = 0,
        .textbox_text = {[0 ... MAX_TEXT_IN_TEXTAREA-1] = ' '},
        .text_final_index = 0,
    };
    

    InitWindow(screenWidth, screenHeight, "floatUML");

    SetTargetFPS(60);

    struct World w = startup_example();
    SetTextureFilter(w.style.font.texture, TEXTURE_FILTER_TRILINEAR);

    uint32_t cellSize = 32;

    while (!WindowShouldClose())
    {
        screenHeight = GetScreenHeight();
        screenWidth = GetScreenWidth();
        width_textarea = (float)screenWidth / (float)PERCENTATGE_MIDA_TEXTBOX;
        textarea = (Rectangle){screenWidth - width_textarea, 0, width_textarea, screenHeight};

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x <= screenWidth; x += cellSize) {
            DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
        }

        // Dibujar líneas horizontales
        for (int y = 0; y <= screenHeight; y += cellSize) {
            DrawLine(0, y, screenWidth, y, LIGHTGRAY);
        }

        for (int i = 0; i < w.classes.len; ++i)
            umld_class(w.classes.cs[i], &w.style);

        for (int i = 0; i < w.relacions.len; ++i)
            draw_relation(w.relacions.rs[i], &w.style);

        if (st.textbox_up) {
            Bool mouseOnText;
            if (CheckCollisionPointRec(GetMousePosition(), textarea)) mouseOnText = true;
            else mouseOnText = false;

            DrawRectangleRec(textarea, BLACK);
            DrawText(st.textbox_text, (int)textarea.x + TEXTBOX_LEFTPAD, (int)textarea.y + TEXTBOX_VERTPAD,
                     w.style.fontsize, TEXT_COLOR);
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();
            while (key > 0) {
                // Only ascii!!
                if ((key >= 32) && (key <= 125) &&
                    (st.text_cursor < MAX_TEXT_IN_TEXTAREA)) {
                    st.textbox_text[st.text_cursor] = (char)key;
                    st.textbox_text[++st.text_final_index] = '\0';
                    st.text_cursor++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
                if (st.text_cursor > 0) {
                    for (int i = st.text_cursor; i < MAX_TEXT_IN_TEXTAREA; ++i) {
                      st.textbox_text[i-1] = st.textbox_text[i];
                    }
                    st.text_cursor--;
                }
            }

            if (st.text_cursor < MAX_TEXT_IN_TEXTAREA) {
              char* nulltermed_text = malloc((st.text_cursor + 1)* sizeof(char));
              for (int i = 0; i < st.text_cursor; ++i) {
                nulltermed_text[i] = st.textbox_text[i];
              }
              nulltermed_text[st.text_cursor] = '\0';
              if (((st.frames_counter++ / 20) % 2) == 0)
                  DrawText("_",
                           textarea.x + TEXTBOX_LEFTPAD + MeasureText(nulltermed_text, w.style.fontsize),
                           textarea.y + TEXTBOX_VERTPAD + 4, w.style.fontsize,
                           TEXT_COLOR);

              free(nulltermed_text);
            }
        } else {
          char* text = "F10 to toggle terminal";
          DrawText(text, screenWidth - MeasureText(text, w.style.fontsize) - TEXTBOX_LEFTPAD, screenHeight - w.style.fontsize, w.style.fontsize, BLACK);
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
        else if (IsKeyPressed(KEY_LEFT) && st.text_cursor > 0)
            st.text_cursor--;
        else if (IsKeyPressed(KEY_RIGHT)
                 && st.text_cursor < MAX_TEXT_IN_TEXTAREA
                 && st.text_cursor < st.text_final_index)
            st.text_cursor++;
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
