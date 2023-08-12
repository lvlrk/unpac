#pragma once

#include <string>
#include <vector>

class Archive {
public:
    class Member {
    public:
        enum CompressionType {
            COM_NONE = 0,
            COM_LZSS = 1,
            COM_LZSS0 = 2,
            COM_ZLIB = 3
        };

        int ReadFromFile(const std::string& filename);
        int WriteToFile(const std::string& filename);

        std::string name = "";
        std::vector<char> data;

        bool isCompressed = false;
        int compressionType = COM_NONE;
        std::vector<char> compressedData;
    private:
        bool IsDataBad();
    };
};
