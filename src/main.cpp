#include <cstring>
#include <fmt/core.h>
#include <string>
#include <iostream>
#include "../include/bytestream.h"
#include <cctype>

//#define RELEASE
#define DEBUG

#ifdef RELEASE
#define VERSION 1.0

int main(int argc, char **argv) {
    const std::string usage = fmt::format("usage: {} [-f]\n"
            "     --help           print this help and exit\n"
            "     --version        print version information and exit\n"
            " -f, --file=FILE      set operation file\n"
            " -x, --extract=[DIR]  extract operation file to DIR\n", argv[0]);

    if(argc < 2) {
        std::cerr << usage;
        return 1;
    }

    std::string file = "";

    const std::string required_argument_format = "{}: requires an argument\n";
    const std::string required_arguments_format = "{}: requires arguments";

    bool extract = false;

    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "--help")) {
            std::cout << usage;
            return 0;
        } else if(!strcmp(argv[i], "--version")) {
            fmt::print("{}-{:.1f}\n", argv[0], VERSION);
            return 0;
        } else if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
            if(argv[i + 1]) {
                file = argv[i + 1];
            } else {
                std::cerr << fmt::format(required_argument_format, argv[i]);
                std::cerr << usage;
                return 1;
            }
        } else if(!strcmp(argv[i], "-x") || !strcmp(argv[i], "--extract")) {
            if(file != "") {
                std::string extract_dir = "";

                if(argv[i + 1] && argv[i + 1][0] != '-') {
                    extract_dir = argv[i + 1];
                } else {
                    extract_dir = remove_all_extensions(file);
                }


            } else {
                std::cerr << fmt::format(required_argument_format, argv[i]);
                std::cerr << usage;
                return 1;
            }
        }
    }

    return 0;
}
#endif
#ifdef DEBUG
int main() {
    bytestream bs;
    bs.from_file("m00a_t_icon01.brres");

	bs.seek(8);
	int size = bs.read_int<int>();
	fmt::print("{}\n", size);
	bs.seek(-4, bs.cur);
	bs.endianness(guess_endianness(reinterpret_cast<char*>(&size), sizeof(size)));
	//size = bs.read_int<int>();

	fmt::print("{}\n", size);
	
    return 0;
}
#endif
