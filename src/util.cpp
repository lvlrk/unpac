#include "util.h"

const std::string RemoveExtension(const std::string& filename, const std::string& extension) {
    // i have no clue how this code works i stole it from stackoverflow
    return {filename, 0, filename.rfind(extension)};
}
