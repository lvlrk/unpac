#pragma once

#include <string>

int OpenError(const std::string& filename);
int ReadError(const std::string& filename);
int WriteError(const std::string& filename);
int MissingArgumentError(const std::string& option);
int VcraMemberReadError(const std::string& name);
int VcraMemberWriteError(const std::string& name);
int VcraReadError(const std::string& filename);
int VcraWriteError(const std::string& filename);
