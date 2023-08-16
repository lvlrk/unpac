#pragma once

#include <string>
#include <vector>

class App {
public:
    App(int argc, char **argv);
    int Main();

    // option functions
    int Compress(const std::string& filename);
    int Decompress(const std::string& filename);
    int Create(const std::string& filename, const std::vector<std::string> names);
    int Extract(const std::string& filename, const std::vector<std::string>& filenames);
    int List(const std::string& filename);
    int Add(const std::string& filename, const std::vector<std::string>& filenames);
    int Remove(const std::string& filename, const std::vector<std::string>& names);
    int Usage();
    void Help();
    void Examples();
    void Version();
private:
    int argc;
    char **argv;
};
