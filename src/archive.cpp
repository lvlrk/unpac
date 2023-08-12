#include <fstream>
#include "archive.h"

int Archive::Member::ReadFromFile(const std::string& filename) {
    name = filename;

    std::ifstream in(name, std::ios::ate);

    data.resize(in.tellg());
    in.seekg(0, in.beg);

    in.read(reinterpret_cast<char*>(&data[0]), data.size());

    return 0;
}

int Archive::Member::WriteToFile(const std::string& filename) {
    if(IsDataBad()) return 1;

    std::ofstream out(filename);

    if(!isCompressed || compressionType == COM_NONE) {
        out.write(reinterpret_cast<char*>(&data[0]), data.size());
    } else {
        switch(compressionType) {
        }
    }

    return 0;
}

bool Archive::Member::IsDataBad() {
    return (!data.size() || !compressedData.size());
}
