
#include "defs.h"
#include "raylib.h"
#include "umla.h"
#include "umld_class.h"
#include "umls.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "floatUML");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    struct Classe a;
    a.nom = umls_from("Hola");
    a.attribs = umla_init();

    struct Attribute b = {umls_from("dni"), umls_from("String"), 0};

    umla_append(&a.attribs, b);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        umld_class(a, 40);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
