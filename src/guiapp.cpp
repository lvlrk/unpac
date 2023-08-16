#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "guiapp.h"
#include "util.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

GuiApp::GuiApp() {
    textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
}

int GuiApp::Main() {
    /* styling */
    InitWindow(800, 600, "unpac");
    if(ChangeStyle(currentStyleIdx)) {
        std::cerr << "unpac gui error: bad style\n";
        return 1;
    }

    while(!WindowShouldClose()) {
        if(IsFileDropped()) {
            droppedFiles = LoadDroppedFiles();

            if(droppedFiles.count > 0) {
                inputFiles.resize(droppedFiles.count);
                for(int i = 0; i < droppedFiles.count; i++)
                    inputFiles.push_back(droppedFiles.paths[i]);
            }

            UnloadDroppedFiles(droppedFiles);
        }

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        Rectangle styleButtonRec = {20, 20, (float)MeasureText(currentStyle.c_str(), fontSize), fontSize + (float)((float)fontSize / 2)};
        if(styles[currentStyleIdx] == "jungle") styleButtonRec.width *= 2;
        if(styles[currentStyleIdx] == "bluish") styleButtonRec.width *= 1.5;
        // styleButtonRec.x = width - (styleButtonRec.x * styleButtonRec.width) - 20;

        if(GuiButton(styleButtonRec, currentStyle.c_str())) {
            if(currentStyleIdx + 1 > styles.size() - 1) {
                currentStyleIdx = 0;
                if(ChangeStyle(currentStyleIdx)) {
                    std::cerr << "unpac gui error: bad style\n";
                    return 1;
                }
            } else {
                if(ChangeStyle(++currentStyleIdx)) {
                    std::cerr << "unpac gui error: bad style\n";
                    return 1;
                }
            }
        }

        for(int i = 0; i < inputFiles.size(); i++) {
            DrawText(inputFiles[i].c_str(), fontSize, (fontSize * i) + (20 * i), fontSize, textColor);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// shitty cheap function
Rectangle GuiApp::GetRectangleText(const std::string& text, int x, int y) {
    Rectangle rect = {(float)x, (float)y, (float)MeasureText(text.c_str(), fontSize), (float)(fontSize + ((float)fontSize / 2))};

    return rect;
}

int GuiApp::ChangeStyle(int idx) {
    if(idx > styles.size() - 1) {
        std::cerr << "unpac gui error: bad style index\n";
        return 1;
    }

    currentStyleIdx = idx;
    currentStyle = styles[currentStyleIdx];

    std::string tmp = "styles";
    tmp += DIRSEP;
    tmp += currentStyle + ".rgs";

    GuiLoadStyle(tmp.c_str());

    textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));

    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

    return 0;
}
