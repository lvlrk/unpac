#pragma once

#include <vector>
#include <string>
#include "util.h"
#include "raygui.h"

class GuiApp {
public:
    GuiApp();
    int Main();
private:
    /* private member functions */
    Rectangle GetRectangleText(const std::string& text, int x, int y);
    int ChangeStyle(int idx);

    /* variables */
    std::vector<std::string> styles = {"ashes", "cyber", "lavanda",
        "bluish", "dark", "sunny", "candy", "enefete", "terminal",
        "cherry", "jungle"};

    /* all uninitialized variables are initialized in constructor */
    const int width = 800;
    const int height = 600;
    std::string title = "unpac";

    int defaultStyleIdx = 1;
    int currentStyleIdx = defaultStyleIdx;
    std::string currentStyle = styles[currentStyleIdx];

    const int fontSize = 30;
    Color textColor;

    bool dropped = false;

    FilePathList droppedFiles;
    std::vector<std::string> inputFiles;
};
