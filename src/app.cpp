#include <fstream>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include "app.h"
#include "util.h"
#include "lzss.h"
#include "vcra.h"
#include "error.h"
#include "ap.h"

App::App(int argc, char **argv):
    argc{argc},
    argv{argv} {}

int App::Main() {
    if(argc < 2) return Usage();

    Parser argParser(argc, argv);

    Opt helpOpt("help", 0);
    Opt examplesOpt("examples", 0);
    Opt versionOpt("version", 'v');
    Opt compressOpt("compress", 'z', 1);
    Opt decompressOpt("decompress", 'd', 1);
    Opt createOpt("create", 'c', AC_VARI);
    Opt extractOpt("extract", 'x', AC_VARI);
    Opt listOpt("list", 't', 1);
    Opt addOpt("add", 'a', AC_VARI);
    Opt removeOpt("remove", 'r', AC_VARI);

    argParser.opts.push_back(helpOpt);
    argParser.opts.push_back(examplesOpt);
    argParser.opts.push_back(versionOpt);
    argParser.opts.push_back(compressOpt);
    argParser.opts.push_back(decompressOpt);
    argParser.opts.push_back(createOpt);
    argParser.opts.push_back(extractOpt);
    argParser.opts.push_back(listOpt);
    argParser.opts.push_back(addOpt);
    argParser.opts.push_back(removeOpt);

    argParser.Parse();

    if(helpOpt.set) {
        Help();

        return 0;
    }
    if(examplesOpt.set) {
        Examples();

        return 0;
    }
    if(versionOpt.set) {
        Version();

        return 0;
    }
    if(compressOpt.set) {
        if(compressOpt.args.size() < compressOpt.argCount)
            return MissingArgumentError("-z");
        else {
            if(Compress(compressOpt.args[0])) return 1;
        }
    }
    if(decompressOpt.set) {
        if(decompressOpt.args.size() < decompressOpt.argCount)
            return MissingArgumentError("-d");
        else {
            if(Decompress(decompressOpt.args[0])) return 1;
        }
    }
    if(createOpt.set) {
        if(createOpt.args.size() < 1)
            return MissingArgumentError("-c");
        else {
            std::string tmp = createOpt.args[0];
            createOpt.args.erase(createOpt.args.begin());
            if(Create(tmp, createOpt.args)) return 1;
        }
    }
    if(extractOpt.set) {
        if(extractOpt.args.size() < 1)
            return MissingArgumentError("-x");
        else {
            std::string tmp = extractOpt.args[0];
            extractOpt.args.erase(extractOpt.args.begin());
            if(Extract(tmp, extractOpt.args)) return 1;
        }
    }
    if(listOpt.set) {
        if(listOpt.args.size() < listOpt.argCount)
            return MissingArgumentError("-t");
        else {
            if(List(listOpt.args[0])) return 1;
        }
    }
    if(addOpt.set) {
        if(addOpt.args.size() < 1)
            return MissingArgumentError("-a");
        else {
            std::string tmp = addOpt.args[0];
            addOpt.args.erase(addOpt.args.begin());
            if(Add(tmp, addOpt.args)) return 1;
        }
    }
    if(removeOpt.set) {
        if(removeOpt.args.size() < 1)
            return MissingArgumentError("-r");
        else {
            std::string tmp = removeOpt.args[0];
            removeOpt.args.erase(removeOpt.args.begin());
            if(Remove(tmp, removeOpt.args)) return 1;
        }
    }

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
    std::cout << "unpac-1.3\n";
    std::cout << "added epic arg parsing\n";
}
