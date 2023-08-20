#include "estream.h"

estream::estream() {}

estream::estream(const std::string& filename, ios_base::openmode mode) {
    open(filename, mode);
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

float estream::read_float() {
    float x;

    read(reinterpret_cast<char*>(&x), sizeof(x));

    if(endianness != std::endian::native)
        x = std::byteswap(static_cast<int>(x));

    return x;
}

double estream::read_double() {
    double x;

    read(reinterpret_cast<char*>(&x), sizeof(x));

    if(endianness != std::endian::native)
        x = std::byteswap(static_cast<long long>(x));

    return x;
}

std::fstream& estream::write_float(const float& x) {
    float tx = x;

    if(endianness != std::endian::native)
        tx = std::byteswap(static_cast<int>(tx));

    write(reinterpret_cast<char*>(&tx), sizeof(tx));

    return *this;
}

std::fstream& estream::write_double(const double& x) {
    double tx = x;

    if(endianness != std::endian::native)
        tx = std::byteswap(static_cast<long long>(tx));

    write(reinterpret_cast<char*>(&tx), sizeof(tx));

    return *this;
}
