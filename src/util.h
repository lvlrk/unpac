#pragma once

#include <string>

#if (_WIN32 || _WIN64)
#include <unistd.h>

#define mkdir(name) _mkdir
#define DIRSEP "\\"
#else
#include <sys/stat.h>

#define mkdir(name) mkdir(name, 0755)
#define DIRSEP "/"
#endif

const std::string RemoveExtension(const std::string& filename, const std::string& extension);
const std::string RemoveAllExtensions(const std::string& filename);
const std::string GetFileFromPath(const std::string& path);
