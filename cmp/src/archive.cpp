#include <iostream>
#include <fstream>
#include "archive.h"
#include "util.h"

int Archive::Member::ReadFromFile(const std::string& filename) {
    name = GetFileFromPath(filename);
    data.clear();

    std::ifstream in(filename, std::ios::ate);

    data.resize(in.tellg());
    in.seekg(0, in.beg);

    in.read(reinterpret_cast<char*>(&data[0]), data.size());

    return 0;
}

int Archive::Member::WriteToFile(const std::string& filename) {
    std::ofstream out(filename);

    out.write(reinterpret_cast<char*>(&data[0]), data.size());

    return 0;
}
