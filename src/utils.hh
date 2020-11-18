#pragma once 

#include <vector>
#include <string>

namespace utils {
    bool compile(const std::string& compiler, const std::string& flags, const std::string& src, const std::string& out);
    
    bool link(const std::string& linker, const std::string& flags, const std::string& obj, const std::string& out);

    std::vector<std::string> string_split(const std::string& str, char delimeter);

   std::string string_replace(std::string str, const std::string& from, const std::string& to);

}