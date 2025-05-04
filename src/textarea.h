#include "defs.h"
#include "raylib.h"
#include "umls.h"

#define MAX_CPTS 1000

typedef struct {
    int cpts[MAX_CPTS];
    int cursorPos;
    Rectangle area;
} TextArea;

TextArea create_text_area(Rectangle area) {
    TextArea a;
    for (int i = 0; i < 1000; i++) {
        a.cpts[i] = 0;
    }
    a.cursorPos = 0;
    a.area = area;
    return a;
}

struct StrSlice from_cpts_to_strslice(int *cpts) {

    struct StrSlice str = umls_init();
    umls_resize(&str, 1000);
    str.len = 1000;
    for (int i = 0; i < 1000; i++) {
        str.text[i] = cpts[i];
    }

    str.text[str.len] = 0;

    return str;
}

void umld_text_area(TextArea *a, struct World *w) {

    Vector2 ipos = (Vector2){a->area.x, a->area.y};
    Vector2 pos = ipos;
    int i = 0;
    int c = a->cpts[i];
    while (c != 0) {
        if (i == a->cursorPos) {
            DrawTextEx(w->style.font, "_", pos, w->style.fontsize, 0, BLACK);
        }
        if (is_visiblechar(c)) {
            Vector2 m = MeasureTextEx(w->style.font, TextFormat("%c", (char)c),
                                      w->style.fontsize, 0);
            DrawTextCodepoint(w->style.font, c, pos, w->style.fontsize, BLACK);
            pos.x += m.x;
        } else if (c == 10) {
            Vector2 m = MeasureTextEx(w->style.font, TextFormat("%c", 'O'),
                                      w->style.fontsize, 0);
            pos.y += m.y + 4;
            pos.x = ipos.x;
        } else if (c == 9) {
            Vector2 m = MeasureTextEx(w->style.font, TextFormat("%c", ' '),
                                      w->style.fontsize, 0);
            pos.x += m.x * 4;
        }

        i++;
        c = a->cpts[i];
    };
    if (i == a->cursorPos) {
        DrawTextEx(w->style.font, "_", pos, w->style.fontsize, 0, BLACK);
    }
}

void backspace_textarea(TextArea *a) {
    int i = a->cursorPos;
    if (i == 0)
        return;

    // whar
    while (i > 0 && i < MAX_CPTS - 1) {
        a->cpts[i - 1] = a->cpts[i];
        i++;
    }
    a->cursorPos--;
}

int uml_text_area_pull_events(TextArea *a) {
    int c = GetCharPressed();
    int update = 0;
    while (c != 0) {
        update = 1;
        if (is_visiblechar(c)) {
            a->cpts[a->cursorPos] = c;
            a->cursorPos++;
        }
        c = GetCharPressed();
    }
    while ((c = GetKeyPressed()) != 0) {
        update = 1;
        if (c == KEY_BACKSPACE) {
            backspace_textarea(a);
        } else if (c == KEY_ENTER) {
            a->cpts[a->cursorPos] = 10;
            a->cursorPos++;
        } else if (c == KEY_TAB) {
            a->cpts[a->cursorPos] = 9;
            a->cursorPos++;
        }
    }

    // Allow holding down da key
    if (IsKeyPressedRepeat(KEY_BACKSPACE))
        backspace_textarea(a);
    return update;
}
