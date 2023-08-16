#pragma once

#include <string>

class Brres {
public:
    int ReadFromFile(const std::string& filename);
private:
    const std::string filename;
};
