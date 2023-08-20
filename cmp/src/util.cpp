#include "util.h"

const std::string RemoveExtension(const std::string& filename, const std::string& extension) {
    // i have no clue how this code works i stole it from stackoverflow
    return {filename, 0, filename.rfind(extension)};
}

const std::string RemoveAllExtensions(const std::string& filename) {
    return {filename, 0, filename.find('.')};
}

const std::string GetFileFromPath(const std::string& path) {
    return path.substr(path.find_last_of("/\\") + 1);
}
