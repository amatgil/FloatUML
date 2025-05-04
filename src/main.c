#include "defs.h"
#include "intersect.h"
#include "parser.h"
#include "raylib.h"
#include "raymath.h"
#include "textarea.h"
#include "umla.h"
#include "umlc.h"
#include "umld_class.h"
#include "umld_rel.h"
#include "umlr.h"
#include "umls.h"
#include "utils.h"
#include <stdint.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

typedef int32_t Bool;

#define MAX_TEXT_IN_TEXTAREA 2000
#define PERCENTATGE_MIDA_TEXTBOX 3 // width/PERCENTATGE_MIDA_TEXTBOX
#define TEXTBOX_LEFTPAD 8
#define TEXTBOX_VERTPAD 5
#define TEXT_COLOR RAYWHITE

struct State {
    struct Classe *curr_held; // NULL if nothing is held
    Bool textbox_up;
    Bool saving_to_texture;
};

void UpdateDrawFrame(void); // Update and Draw one frame (for web)

int screenWidth;
int screenHeight;
float width_textarea;
Rectangle textarea;

TextArea tarea;

struct State st = {
    .curr_held = NULL,
    .textbox_up = false,
    .saving_to_texture = false,
};

uint32_t cellSize = 32;

RenderTexture2D save_target;
struct World w;

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTextureFilter(w.style.font.texture, TEXTURE_FILTER_TRILINEAR);

    screenWidth = 1000;
    screenHeight = 700;
    width_textarea = (float)screenWidth / (float)PERCENTATGE_MIDA_TEXTBOX;
    textarea = (Rectangle){screenWidth - width_textarea, 0, width_textarea,
                           screenHeight};

    w = umlw_init("external/Consolas/consolas.ttf", 22);
    tarea = create_text_area(textarea);
    InitWindow(screenWidth, screenHeight, "floatUML");
    save_target = LoadRenderTexture(screenWidth, screenHeight);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
        UpdateDrawFrame();

#endif
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

void UpdateDrawFrame(void) {

    while (!WindowShouldClose()) {
        int new_height = GetScreenHeight();
        int new_width = GetScreenWidth();

        // This also leaks memory (we lose the old texture)
        if (new_height != screenHeight || new_width != screenWidth)
            save_target = LoadRenderTexture(new_width, new_width);

        width_textarea = (float)screenWidth / (float)PERCENTATGE_MIDA_TEXTBOX;
        textarea = (Rectangle){screenWidth - width_textarea, 0, width_textarea,
                               screenHeight};

        if (st.saving_to_texture)
            BeginTextureMode(save_target);
        else
            BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x <= screenWidth; x += cellSize) {
            DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
        }

        for (int y = 0; y <= screenHeight; y += cellSize) {
            DrawLine(0, y, screenWidth, y, LIGHTGRAY);
        }

        /* for (int i = 0; i < w.classes.len; ++i) { */
        /*     if (w.classes.cs[i].superclasse != NULL) */
        /*         draw_subclass_relation(*w.classes.cs[i].superclasse, */
        /*                                w.classes.cs[i], &w.style); */
        /* } */

        for (int i = 0; i < w.classes.len; ++i) {
            w.classes.cs[i].pos.x =
                w.classes.cs[i].pos.x * new_width / screenWidth;
            w.classes.cs[i].pos.y =
                w.classes.cs[i].pos.y * new_height / screenHeight;
            umld_class(w.classes.cs[i], &w.style);
        }

        for (int i = 0; i < w.relacions.len; ++i)
            draw_relation(w.relacions.rs[i], &w.style);

        if (st.textbox_up) {
            int update = uml_text_area_pull_events(&tarea);
            umld_text_area(&tarea, &w);
            if (update) {
                struct StrSlice slice = from_cpts_to_strslice(tarea.cpts);
                parse(&slice, &w);
            }
        } else {
            char *text = "F10 to toggle terminal";
            DrawText(text,
                     screenWidth - MeasureText(text, w.style.fontsize) -
                         TEXTBOX_LEFTPAD,
                     screenHeight - w.style.fontsize, w.style.fontsize, BLACK);
        }

        if (st.saving_to_texture)
            EndTextureMode();
        else
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
                for (int i = 0; i < w.relacions.rs->len; ++i) {
                    if (w.relacions.rs->associativa != NULL) {
                        Rectangle rect = umld_rect_of(
                            *w.relacions.rs->associativa, &w.style);
                        if (CheckCollisionPointRec(mpos, rect)) {
                            st.curr_held = w.relacions.rs->associativa;
                        }
                    }
                }
            }
        } else
            st.curr_held = NULL;

        if (st.saving_to_texture) {
            ExportImage(LoadImageFromTexture(save_target.texture),
                        "/tmp/output.png");
        }
        st.saving_to_texture = false;

        if (IsKeyPressed(KEY_F10))
            st.textbox_up = !st.textbox_up;
        if (IsKeyPressed(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            printf("saving to texture: on \n");
            st.saving_to_texture = true;
        }

        screenHeight = new_height;
        screenWidth = new_width;
    }
}
