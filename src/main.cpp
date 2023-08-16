#include "app.h"

int main(int argc, char **argv) {
    App app(argc, argv);
    if(argc < 2) return app.Usage();

    return app.Main();
}
