#include "estream.h"

estream::estream(const char *filename) {
    open(filename);
}

std::endian& estream::endian() {
    return endianness;
}

std::fstream& estream::skipg(std::streampos n) {
    seekg(n, cur);

    return *this;
}

std::fstream& estream::skipp(std::streampos n) {
    seekp(n, cur);

    return *this;
}
