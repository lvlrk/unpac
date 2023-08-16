#pragma once

#include <string>
#include <vector>

class Archive {
public:
    class Member {
    public:
        int ReadFromFile(const std::string& filename);
        int WriteToFile(const std::string& filename);

        std::string name = "";
        std::vector<char> data;
    };

    std::vector<Member> members;
};
