#include <iostream>
#include <fmt/core.h>
#include "ap.h"

Opt::Opt(const std::string& nLong, char nShort, int argCount):
    nLong{nLong},
    nShort{nShort},
argCount{argCount} {}

Parser::Parser(int argc, char **argv) {
    args.resize(argc - 1);

    for(int i = 1; i < argc; i++) args.push_back(argv[i]);
}

void Parser::Parse() {
    for(int i = 0; i < args.size(); i++) {
        std::string cGetOpt = GetOpt(args[i]);
        int cGetOptType = GetOptType(args[i]);

        for(Opt& cOpt: opts) {
            // skip if opt has been set
            if(cOpt.set) continue;

            switch(cGetOptType) {
            case OPT_LONG:
                if(cGetOpt == cOpt.nLong) {
                    cOpt.set = true;

                    ParseOptArgs(cOpt, i);
                }

                break;
            case OPT_SHORT:
                if(cGetOpt[0] == cOpt.nShort) {
                    cOpt.set = true;

                    ParseOptArgs(cOpt, i);
                }

                break;
            case OPT_MSHORT:
                for(char c: cGetOpt) {
                    if(c == cOpt.nShort) cOpt.set = true;

                    ParseOptArgs(cOpt, i);
                }

                break;
            }
        }
    }
}

int Parser::GetOptType(const std::string& arg) {
    if(arg[0] == '-' && arg[1] == '-') return OPT_LONG;
    else if(arg[0] == '-' && arg[1] != '-') {
        if(arg.size() == 2) return OPT_SHORT;
        else if(arg.size() > 2) return OPT_MSHORT;
    }

    return OPT_NONE;
}

std::string Parser::GetOpt(const std::string& arg, int optType) {
    int myOptType;

    if(optType == -1) myOptType = GetOptType(arg);
    else myOptType = optType;

    switch(myOptType) {
    case OPT_LONG:
        return arg.substr(2);

        break;
    case OPT_SHORT:
    case OPT_MSHORT:
        return arg.substr(1);

        break;
    default:
        return arg;
    }
}

bool Parser::ParseOptArgs(Opt& opt, int argIndex) {
    if(opt.argCount != AC_NONE) {
        if(opt.argCount == AC_VARI) {
            int i = argIndex + 1; // args index; skip over current arg

            while(GetOptType(args[i]) == OPT_NONE) {
                opt.args.push_back(args[i]);
                if(i >= args.size() - 1) break;
                else i++;
            }

            return true;
        } else {
            int i = argIndex + 1; // args index; skip over current arg
            int j = 0;

            if(i == opt.argCount + 1) return false;

            while(GetOptType(args[i]) == OPT_NONE) {
                opt.args.push_back(args[i]);
                if(i >= opt.argCount) break;
                else {i++; j++;}
            }

            if(i == opt.argCount - 1) return true;
            else return false;
        }
    }

    return true;
}
