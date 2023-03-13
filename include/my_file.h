#ifndef _MY_FILE_H
#define _MY_FILE_H

#include <string>
#include <fstream>
#include <vector>
#include "bytestream.h"

namespace ft {
    enum classification {
        DATA = 1,
        TEXT = 1 << 1,
        INFO = 1 << 2,
        ARCHIVE = 1 << 3,
        COMPRESSED = 1 << 4
    };

    enum integrity {
        BAD = 1 << 5,
        CORRUPT = 1 << 6,
        OK = 1 << 7
    };

    enum type {
        UNRECOGNIZED = 1 << 8,
        ARCV = 1 << 9
    };

    int id_file(const std::string& filename);
    int id_stream(std::ifstream& stream, const std::string& filename = "");
    int id_data(const std::vector<char>& data, const std::string& filename = "");
    int id_bytestream(bytestream bs, const std::string& filename = "");
}

#endif
