#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include "lzss.h"
#include "util.h"
#include "vcra.h"

int main(int argc, char **argv) {
    std::string file = "";
    std::vector<std::string> files;
    std::string arg = "";

    bool compress = false;
    bool decompress = false;
    bool create = false;
    bool extract = false;
    bool extractAll = false;
    bool list = false;
    bool append = false;
    bool remove = false;

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
                std::cout << " -c, --create       create an archive\n";
                std::cout << " -x, --extract      extract files from an archive\n";
                std::cout << " -t, --list         list files from an archive\n";
                std::cout << " -a, --append       append files to an archive\n";
                std::cout << " -r, --remove       remove files from an archive\n\n";
                std::cout << "enjoy this bangin' modding tool\n";

                return 0;
            } else if(arg == "--examples") {
                std::cout << "# will decompress myfile.lzs to myfile.arc if it is a VCRA file, else, myfile\n";
                std::cout << "unpac -d myfile.lzs\n\n";

                std::cout << "# will compress myfile.arc to myfile.lzs\n";
                std::cout << "unpac -z myfile.arc\n\n";

                std::cout << "# will compress myfile to myfile.lzs\n";
                std::cout << "unpac -z myfile\n";

                std::cout << "# unpac -c [archive] {file...} or {dir...}\n";
                std::cout << "# will create myfile.arc from file1 and file2\n";
                std::cout << "unpac -c myfile.arc file1 file2\n\n";

                std::cout << "# unpac -x [archive] {file...}\n";
                std::cout << "# extract only file1 from myfile.arc\n";
                std::cout << "unpac -x myfile.arc file1\n";
                std::cout << "# extract all files from myfile.arc\n";
                std::cout << "unpac -x myfile.arc\n";

                return 0;
            } else if(arg == "-v" || arg == "--version") {
                std::cout << "unpac-1.1\n";
                std::cout << "got them archive tools now too\n";

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
            } else if(arg == "-c" || arg == "--create") {
                if(argv[i + 1]) {
                    file = argv[++i];

                    create = true;

                    int j = i + 1;
                    while(argv[j]) {
                        if(argv[j][0] != '-') files.push_back(argv[j++]);
                        else break;
                    }
                } else goto missing_argument;
            } else if(arg == "-x" || arg == "--extract") {
                if(argv[i + 1]) {
                    file = argv[++i];

                    extract = true;

                    int j = i + 1;
                    while(argv[j]) {
                        if(argv[j][0] != '-') files.push_back(argv[j++]);
                        else break;
                    }

                    if(j == i + 1) extractAll = true;
                } else goto missing_argument;
            } else if(arg == "-t" || arg == "--list") {
                if(argv[i + 1]) {
                    file = argv[++i];

                    list = true;
                } else goto missing_argument;
            } else if(arg == "-a" || arg == "--append") {
                file = argv[++i];

                append = true;

                int j = i + 1;
                while(argv[j]) {
                    if(argv[j][0] != '-') files.push_back(argv[j++]);
                    else break;
                }
            } else if(arg == "-r" || arg == "--remove") {
                file = argv[++i];

                remove = true;

                int j = i + 1;
                while(argv[j]) {
                    if(argv[j][0] != '-') files.push_back(argv[j++]);
                    else break;
                }
            }

            i++;
        }
    } else {
        std::cerr << "Usage: unpac [-vzdcxtar]\n";
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
    if(create) {
        Vcra arc;
        Archive::Member member;

        struct stat st;
        for(const std::string& f: files) {
            if(stat(f.c_str(), &st) == -1) {
                std::cerr << "unpac error: bad stat for " << f << '\n';

                continue;
            } else {
                std::cout << f << '\n';
                if(S_ISREG(st.st_mode)) {
                    if(member.ReadFromFile(f)) goto member_read;
                    arc.members.push_back(member);
                } else if(S_ISDIR(st.st_mode)) {
                    for (std::filesystem::recursive_directory_iterator i(f), end; i != end; ++i) {
                        if (!is_directory(i->path())) {
                            if(member.ReadFromFile(std::string(i->path().filename()))) goto member_read;
                            arc.members.push_back(member);
                        }
                    }
                }
            }
        }

        arc.WriteToFile(file);
    }
    if(extract) {
        Vcra arc;

        if(arc.ReadFromFile(file)) goto vcra_read;

        if(extractAll) {
            std::string outdir = RemoveAllExtensions(file);
            mkdir(outdir.c_str(), 0755);

            for(Archive::Member& member: arc.members) {
                if(member.WriteToFile(outdir + '/' + member.name)) {
                    std::cerr << "unpac error: bad write\n";
                }
            }
        } else {
            for(const std::string& f: files) {
                for(Archive::Member& member: arc.members) {
                    if(f == member.name) {
                        if(member.WriteToFile(member.name)) {
                            std::cerr << "unpac error: bad write\n";
                        }
                    }
                }
            }
        }
    }
    if(list) {
        Vcra arc;

        if(arc.ReadFromFile(file)) goto vcra_read;

        for(const Archive::Member& member: arc.members) {
            std::cout << member.name << '\n';
        }
    }
    if(append) {
        Vcra arc;

        if(arc.ReadFromFile(file)) goto vcra_read;

        Archive::Member member;

        for(const std::string& f: files) {
            member.ReadFromFile(f);

            arc.members.push_back(member);
        }

        if(arc.WriteToFile(file)) goto vcra_write;
    }
    if(remove) {
        Vcra arc;

        if(arc.ReadFromFile(file)) goto vcra_read;

        for(int i = 0; i < arc.members.size(); i++) {
            for(const std::string& f: files) {
                if(arc.members[i].name == f) {
                    arc.members.erase(arc.members.begin() + i);
                }
            }
        }

        if(arc.WriteToFile(file)) goto vcra_write;
    }

    return 0;

// common runtime errors
open:
    std::cerr << "unpac error: cant open " << file << '\n';

    return 1;

missing_argument:
    std::cerr << "unpac error: missing argument to " << arg << '\n';

    return 1;

vcra_read:
    std::cerr << "unpac error: bad vcra read\n";

    return 1;

vcra_write:
    std::cerr << "unpac error: bad vcra write\n";

    return 1;

member_read:
    std::cerr << "unpac error: bad read\n";

    return 1;
}
