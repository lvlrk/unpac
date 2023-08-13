#include <fstream>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include "app.h"
#include "util.h"
#include "lzss.h"
#include "vcra.h"
#include "error.h"

App::App(int argc, char **argv):
    argc{argc},
argv{argv} {}

int App::Main() {
    if(argc > 1) {
        int i = 1;

        while(i < argc) {
            std::string arg = argv[i];

            if(arg == "--help") {
                Help();

                return 0;
            } else if(arg == "--examples") {
                Examples();

                return 0;
            } else if(arg == "-v" || arg == "--version") {
                Version();

                return 0;
            } else if(arg == "-z" || arg == "--compress") {
                if(argv[i + 1]) {
                    if(Compress(argv[++i])) return 1;
                } else return MissingArgumentError(arg);
            } else if(arg == "-d" || arg == "--decompress") {
                if(argv[i + 1]) {
                    if(Decompress(argv[++i])) return 1;
                } else return MissingArgumentError(arg);
            } else if(arg == "-c" || arg == "--create") {
                if(argv[i + 1]) {
                    std::string tmp = argv[++i];

                    std::vector<std::string> tmpv;

                    int j = i + 1;
                    while(argv[j]) {
                        if(argv[j][0] != '-') tmpv.push_back(argv[j++]);
                        else break;
                    }

                    if(Create(tmp, tmpv)) return 1;
                } else return MissingArgumentError(arg);
            } else if(arg == "-x" || arg == "--extract") {
                if(argv[i + 1]) {
                    std::string tmp = argv[++i];

                    std::vector<std::string> tmpv;

                    int j = i + 1;
                    while(argv[j]) {
                        if(argv[j][0] != '-') tmpv.push_back(argv[j++]);
                        else break;
                    }

                    if(Extract(tmp, tmpv)) return 1;
                } else return MissingArgumentError(arg);
            } else if(arg == "-t" || arg == "--list") {
                if(argv[i + 1]) {
                    if(List(argv[++i])) return 1;
                } else return MissingArgumentError(arg);
            } else if(arg == "-a" || arg == "--add") {
                std::string tmp = argv[++i];
                std::vector<std::string> tmpv;

                int j = i + 1;
                while(argv[j]) {
                    if(argv[j][0] != '-') tmpv.push_back(argv[j++]);
                    else break;
                }

                if(Add(tmp, tmpv)) return 1;
            } else if(arg == "-r" || arg == "--remove") {
                std::string tmp = argv[++i];
                std::vector<std::string> tmpv;

                int j = i + 1;
                while(argv[j]) {
                    if(argv[j][0] != '-') tmpv.push_back(argv[j++]);
                    else break;
                }

                if(Remove(tmp, tmpv)) return 1;
            }

            i++;
        }
    } else return Usage();

    return 0;
}

int App::Compress(const std::string& filename) {
    std::ifstream in;
    std::ofstream out;

    in.open(filename, std::ios::ate);
    if(!in.is_open()) return OpenError(filename);

    int size = in.tellg();
    in.seekg(0, in.beg);

    std::vector<char> data(size);
    in.read(reinterpret_cast<char*>(&data[0]), size);

    in.close();

    std::string outname;
    if(std::equal(&data[0], &data[0] + 4, "VCRA")) {
        outname = RemoveExtension(filename, ".arc") + ".lzs";
    } else outname = filename + ".lzs";

    out.open(outname);
    if(!out.is_open()) return OpenError(outname);

    std::vector<char> zdata(size);

    int zsize = lzss_compress(reinterpret_cast<unsigned char*>(&data[0]), size, reinterpret_cast<unsigned char*>(&zdata[0]), size);

    out.write("SSZL", 4);
    out.write("\0\0\0\0", 4);
    out.write(reinterpret_cast<char*>(&zsize), 4);
    out.write(reinterpret_cast<char*>(&size), 4);

    out.write(reinterpret_cast<char*>(&zdata[0]), zsize);

    out.close();

    return 0;
}

int App::Decompress(const std::string& filename) {
    std::ifstream in;
    std::ofstream out;

    in.open(filename);
    if(!in.is_open()) return OpenError(filename);

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

    std::string outname;
    if(std::equal(&data[0], &data[0] + 4, "VCRA")) {
        outname = RemoveExtension(filename, ".lzs") + ".arc";
    } else outname = RemoveExtension(filename, ".lzs");

    out.open(outname);
    if(!out.is_open()) return OpenError(outname);

    out.write(reinterpret_cast<char*>(&data[0]), size);

    out.close();

    return 0;
}

int App::Create(const std::string& filename, const std::vector<std::string> names) {
    Vcra arc;
    Archive::Member member;

    struct stat st;
    for(const std::string& name: names) {
        if(stat(name.c_str(), &st) == -1) {
            std::cerr << "unpac error: bad stat for " << name << '\n';

            continue;
        } else {
            if(S_ISREG(st.st_mode)) {
                if(member.ReadFromFile(name))
                    return VcraMemberReadError(name);

                arc.members.push_back(member);
            } else if(S_ISDIR(st.st_mode)) {
                for (std::filesystem::recursive_directory_iterator i(name), end; i != end; ++i) {
                    if (!is_directory(i->path())) {
                        if(member.ReadFromFile(std::string(i->path())))
                            return VcraMemberReadError(std::string(i->path()));

                        arc.members.push_back(member);
                    }
                }
            }
        }
    }

    if(arc.WriteToFile(filename)) return VcraWriteError(filename);

    return 0;
}

int App::Extract(const std::string& filename, const std::vector<std::string>& filenames) {
    Vcra arc;

    if(arc.ReadFromFile(filename)) return VcraReadError(filename);

    if(filenames.size() == 0) {
        std::string outdir = RemoveAllExtensions(filename);
        mkdir(outdir.c_str(), 0755);

        for(Archive::Member& member: arc.members) {
            std::string tmp = outdir + '/' + member.name;
            if(member.WriteToFile(tmp))
                return VcraMemberWriteError(tmp);
        }
    } else {
        for(const std::string& file: filenames) {
            for(Archive::Member& member: arc.members) {
                if(file == member.name) {
                    if(member.WriteToFile(member.name))
                        return VcraMemberWriteError(member.name);
                }
            }
        }
    }

    return 0;
}

int App::List(const std::string& filename) {
    Vcra arc;

    if(arc.ReadFromFile(filename)) return VcraReadError(filename);

    for(const Archive::Member& member: arc.members) {
        std::cout << member.name << '\n';
    }

    return 0;
}

int App::Add(const std::string& filename, const std::vector<std::string>& filenames) {
    Vcra arc;

    if(arc.ReadFromFile(filename)) return VcraReadError(filename);

    Archive::Member member;

    for(const std::string& file: filenames) {
        if(member.ReadFromFile(file))
            return VcraMemberReadError(file);

        bool found = false;
        for(int i = 0; i < arc.members.size(); i++) {
            if(arc.members[i].name == file) {
                found = true;
                if(arc.members[i].ReadFromFile(file))
                    return VcraMemberReadError(file);
            }
        }
        if(!found) arc.members.push_back(member);
    }

    if(arc.WriteToFile(filename)) VcraWriteError(filename);

    return 0;
}

int App::Remove(const std::string& filename, const std::vector<std::string>& names) {
    Vcra arc;

    if(arc.ReadFromFile(filename)) return VcraReadError(filename);

    for(int i = 0; i < arc.members.size(); i++) {
        for(const std::string& name: names) {
            if(arc.members[i].name == name) {
                arc.members.erase(arc.members.begin() + i);
            }
        }
    }

    if(arc.WriteToFile(filename)) return VcraWriteError(filename);

    return 0;
}

int App::Usage() {
    std::cerr << "Usage: unpac [-vzdcxtar]\n";
    std::cerr << "       unpac --help\n";
    std::cerr << "       unpac --examples\n";

    return 1;
}

void App::Help() {
    std::cout << "Usage: unpac [option...] [file...]\n\n";
    std::cout << "     --help         print this help and exit\n";
    std::cout << "     --examples     print common usage examples and exit\n";
    std::cout << " -v, --version      output version information and exit\n";
    std::cout << " -z, --compress     compress file to lzs\n";
    std::cout << " -d, --decompress   decompress file from lzs\n";
    std::cout << " -c, --create       create an archive\n";
    std::cout << " -x, --extract      extract files from an archive\n";
    std::cout << " -t, --list         list files from an archive\n";
    std::cout << " -a, --add          add files to an archive\n";
    std::cout << " -r, --remove       remove files from an archive\n\n";
    std::cout << "enjoy this bangin' modding tool\n";
}

void App::Examples() {
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
}

void App::Version() {
    std::cout << "unpac-1.2\n";
    std::cout << "made them archive tools better\n";
}
