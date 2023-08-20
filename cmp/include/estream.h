#pragma once

#include <fstream>
#include <bit>

class estream: public std::fstream {
public:
    estream();
    estream(const std::string& filename, ios_base::openmode mode = ios_base::in);
    std::endian& endian();
    std::fstream& skipg(std::streampos n);
    std::fstream& skipp(std::streampos n);

    template<typename T>
    T read_int();
    float read_float();
    double read_double();

    template<typename T>
    std::fstream& write_int(const T& x);
    std::fstream& write_float(const float& x);
    std::fstream& write_double(const double& x);
private:
    std::endian endianness = std::endian::native;
};

template<typename T>
T estream::read_int() {
    T x;

    read(reinterpret_cast<char*>(&x), sizeof(x));

    if(endianness != std::endian::native)
        x = std::byteswap(x);

    return x;
}

template<typename T>
std::fstream& estream::write_int(const T& x) {
    T tx = x;

    if(endianness != std::endian::native)
        tx = std::byteswap(tx);
    
    
    write(reinterpret_cast<char*>(&tx), sizeof(tx));

    return *this;
}
