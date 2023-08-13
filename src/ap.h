// AP version 0.0.1
// in-dev

#pragma once

#include <string>
#include <vector>

enum ArgCount {
    AC_NONE = 0,
    AC_VARI = -1
};

class Opt {
public:
    Opt(const std::string& nLong, char nShort, int argCount = AC_NONE);

    std::string nLong;
    char nShort;
    int argCount;
    std::vector<std::string> args;
    bool set = false;
};

class Parser {
public:
    Parser(int argc, char **argv);
    void Parse();

    std::vector<std::reference_wrapper<Opt>> opts;
private:
    enum OptType {
        OPT_LONG = 0,   // starts with '--'; ex. '--file'
        OPT_SHORT = 1,  // starts with '-'; ex. '-f'
        OPT_MSHORT = 2, // a short option with multiple codes; ex. '-cf'
        OPT_NONE = 3    // neither types
    };
    int GetOptType(const std::string& arg);
    std::string GetOpt(const std::string& arg, int optType = -1);

    // returns true if
    // all option's arguments are found
    bool ParseOptArgs(Opt& opt, int argIndex);

    int argc;
    char **argv;
    std::vector<std::string> args;
};
