#pragma once 

#include <vector>
#include <string>

namespace utils {
    bool compile(std::string compiler, std::string flags, std::string src, std::string out);
    
    bool link(std::string linker, std::string flags, std::string obj, std::string out);

    std::vector<std::string> string_split(const std::string& str, char delimeter);

    std::string string_replace(std::string str, const std::string& from, const std::string& to);

}