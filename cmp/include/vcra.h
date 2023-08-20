#pragma once

#include "archive.h"

class Vcra: public Archive {
public:
    int ReadFromFile(const std::string& filename);
    int WriteToFile(const std::string& filename);
};
