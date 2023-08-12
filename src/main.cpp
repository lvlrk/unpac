#include <iostream>
#include <fstream>
#include <vector>
#include "lzss.h"
#include "util.h"

int main(int argc, char **argv) {
    std::string file = "";
    std::string arg = "";

    bool compress = false;
    bool decompress = false;

    std::ofstream out;
    std::ifstream in;

    if(argc > 1) {
        int i = 1;

        while(i < argc) {
            arg = argv[i];

            if(arg == "--help") {
                std::cout << "Usage: unpac [option...] [file...]\n\n";
                std::cout << "     --help         print this help and exit\n";
                std::cout << "     --examples     print common usage examples and exit\n";
                std::cout << " -v, --version      output version information and exit\n";
                std::cout << " -z, --compress     compress file to lzs\n";
                std::cout << " -d, --decompress   decompress file from lzs\n";
                std::cout << "enjoy this bangin' modding tool\n";

                return 0;
            } else if(arg == "--examples") {
                std::cout << "# will decompress myfile.lzs to myfile.arc if it is a VCRA file, else, myfile\n";
                std::cout << "unpac -d myfile.lzs\n\n";

                std::cout << "# will compress myfile.arc to myfile.lzs\n";
                std::cout << "unpac -z myfile.arc\n\n";

                std::cout << "# will compress myfile to myfile.lzs\n";
                std::cout << "unpac -z myfile\n";

                return 0;
            } else if(arg == "-v" || arg == "--version") {
                std::cout << "unpac-1.0\n";
                std::cout << "got them compression tools\n";

                return 0;
            } else if(arg == "-z" || arg == "--compress") {
                if(argv[i + 1]) {
                    file = argv[++i];

                    compress = true;
                } else goto missing_argument;
            } else if(arg == "-d" || arg == "--decompress") {
                if(argv[i + 1]) {
                    file = argv[++i];

                    decompress = true;
                } else goto missing_argument;
            }

            i++;
        }
    } else {
        std::cerr << "Usage: unpac [-vzd]\n";
        std::cerr << "       unpac --help\n";
        std::cerr << "       unpac --examples\n";

        return 1;
    }

    if(compress) {
        in.open(file, std::ios::ate);
        if(!in.is_open()) goto open;

        int size = in.tellg();
        in.seekg(0, in.beg);

        std::vector<char> data(size);
        in.read(reinterpret_cast<char*>(&data[0]), size);

        in.close();

        if(std::equal(&data[0], &data[0] + 4, "VCRA")) {
            out.open(RemoveExtension(file, ".arc") + ".lzs");
        } else out.open(file + ".lzs");

        if(!out.is_open()) goto open;

        std::vector<char> zdata(size);

        int zsize = lzss_compress(reinterpret_cast<unsigned char*>(&data[0]), size, reinterpret_cast<unsigned char*>(&zdata[0]), size);

        out.write("SSZL", 4);
        out.write("\0\0\0\0", 4);
        out.write(reinterpret_cast<char*>(&zsize), 4);
        out.write(reinterpret_cast<char*>(&size), 4);

        out.write(reinterpret_cast<char*>(&zdata[0]), zsize);

        out.close();
    }
    if(decompress) {
        in.open(file);
        if(!in.is_open()) goto open;

        char magic[4];
        in.read(magic, 4);
        if(!std::equal(magic, magic + 4, "SSZL")) {
            std::cerr << "unpac error: bad lzs format\n";

            return 1;
        }

        // skip padding
        in.seekg(4, in.cur);

        int zsize, size;
        in.read(reinterpret_cast<char*>(&zsize), 4);
        in.read(reinterpret_cast<char*>(&size), 4);

        std::vector<char> zdata(zsize);
        in.read(reinterpret_cast<char*>(&zdata[0]), zsize);

        in.close();

        std::vector<char> data(size);
        unlzss(reinterpret_cast<unsigned char*>(&zdata[0]), zsize, reinterpret_cast<unsigned char*>(&data[0]), size);

        if(std::equal(&data[0], &data[0] + 4, "VCRA")) {
            out.open(RemoveExtension(file, ".lzs") + ".arc");
        } else out.open(RemoveExtension(file, ".lzs"));

        out.write(reinterpret_cast<char*>(&data[0]), size);

        out.close();
    }

    return 0;

// common runtime errors
open:
    std::cerr << "unpac error: cant open " << file << '\n';

    return 1;

missing_argument:
    std::cerr << "unpac error: missing argument to " << arg << '\n';

    return 1;
}
