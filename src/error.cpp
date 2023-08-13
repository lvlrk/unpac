#include <iostream>
#include "error.h"

int OpenError(const std::string& filename) {
    std::cerr << "unpac error: io open error '" << filename << "'\n";
    return 1;
}

int ReadError(const std::string& filename) {
    std::cerr << "unpac error: input/read error '" << filename << "'\n";
    return 1;
}

int WriteError(const std::string& filename) {
    std::cerr << "unpac error: output/write error '" << filename << "'\n";
    return 1;
}

int MissingArgumentError(const std::string& option) {
    std::cerr << "unpac error: missing argument for " << option << '\n';
    return 1;
}

int VcraMemberReadError(const std::string& name) {
    std::cerr << "unpac error: vcra member read error '" << name << "'\n";
    return 1;
}

int VcraMemberWriteError(const std::string& name) {
    std::cerr << "unpac error: vcra member write error '" << name << "'\n";
    return 1;
}

int VcraReadError(const std::string& filename) {
    std::cerr << "unpac error: vcra read error '" << filename << "'\n";
    return 1;
}

int VcraWriteError(const std::string& filename) {
    std::cerr << "unpac error: vcra write error '" << filename << "'\n";
    return 1;
}
