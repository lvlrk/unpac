#include "app.h"
#include "guiapp.h"

int main(int argc, char **argv) {
    if(argc > 1) {
        App app(argc, argv);

        return app.Main();
    } else {
        GuiApp app;

        return app.Main();
    }

    return 0;
}
