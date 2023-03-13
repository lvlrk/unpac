#include "filename.h"
#include <iostream>
#include <fmt/core.h>
#include <algorithm>

const std::string remove_all_extensions(const std::string& path) {
    size_t idx = path.find(".");
    if(idx > path.size()) {
        return path;
    }

    std::string result;
    //std::copy(path.begin(), path.begin() + idx + 1, result.begin());
    result.assign(path, 0, idx);

    // fmt::print("{}\n", idx);

    return result;
}
