#include <fstream>
#include <iostream>
#include "vcra.h"

int Vcra::ReadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if(!in.is_open()) return 1;

    char magic[4];
    in.read(magic, 4);
    if(!std::equal(magic, magic + 4, "VCRA")) return 1;

    int tmp;

    in.read(reinterpret_cast<char*>(&tmp), 4);
    members.resize(tmp);

    in.seekg(64, in.beg);

    for(int i = 0; i < members.size(); i++) {
        int off, size;
        in.read(reinterpret_cast<char*>(&off), 4);
        in.read(reinterpret_cast<char*>(&size), 4);

        char name[56];
        in.read(name, 56);

        members[i].name = name;

        tmp = in.tellg();
        in.seekg(off, in.beg);

        members[i].data.resize(size);
        in.read(reinterpret_cast<char*>(&members[i].data[0]), size);
        
        in.seekg(tmp, in.beg);
    }

    return 0;
}

int Vcra::WriteToFile(const std::string& filename) {
    std::ofstream out(filename);
    if(!out.is_open()) return 1;

    out.write("VCRA", 4);
    
    int tmp;
    
    tmp = members.size();
    out.write(reinterpret_cast<char*>(&tmp), 4);

    tmp = 0;
    out.write(reinterpret_cast<char*>(&tmp), 4);

    out.write(reinterpret_cast<char*>(&tmp), 4);

    out.write("\x05\x02\x07\x20", 4);

    for(int i = 0; i < 44; i++) out.put(0);

    for(int i = 0; i < members.size(); i++) {
        tmp = 0;
        out.write(reinterpret_cast<char*>(&tmp), 4);
        
        tmp = members[i].data.size();
        out.write(reinterpret_cast<char*>(&tmp), 4);

        out.write(members[i].name.c_str(), members[i].name.size());
        for(int j = 0; j < 56 - members[i].name.size(); j++) out.put(0);
    }

    int tmp2, tmp3;

    for(int i = 0; i < members.size(); i++) {
        tmp2 = out.tellp();
        out.write(reinterpret_cast<char*>(&members[i].data[0]), members[i].data.size());
        tmp3 = out.tellp();

        // math magic here
        out.seekp((64 + (64 * i)), out.beg);
        out.write(reinterpret_cast<char*>(&tmp2), 4);
        out.seekp(tmp3, out.beg);
    }

    tmp = out.tellp();

    out.seekp(8, out.beg);
    out.write(reinterpret_cast<char*>(&tmp), 4);

    return 0;
}
