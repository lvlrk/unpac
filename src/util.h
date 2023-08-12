#pragma once

#include <string>

const std::string RemoveExtension(const std::string& filename, const std::string& extension);
const std::string RemoveAllExtensions(const std::string& filename);
const std::string GetFileFromPath(const std::string& path);
