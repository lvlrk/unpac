#include <raylib.h>
#include "guiapp.h"

int GuiApp::Main() {
    InitWindow(800, 600, "unpac");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
