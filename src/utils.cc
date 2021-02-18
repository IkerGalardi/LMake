#include "pch.hh"

#include "utils.hh"

#include <iostream>
#include <string>
#include <sstream>

#include "os/process_management.hh"

namespace utils {

    bool compile(const std::string& compiler, const std::string& flags, const std::string& src, const std::string& out) {
        std::string args = std::string(src) + " -c " + flags + " -o " + out;

        os::process p = os::run_process(compiler, args.c_str());
        int exit = os::wait_process(p);
        return exit == 0;
    }
    
    bool link(const std::string& linker, const std::string& flags, const std::string& obj, const std::string& out) {
        std::string args = flags + " " + obj + " " + "-o " + out;
        os::process p = os::run_process(linker, args);
        int exit_code = os::wait_process(p);
        return exit_code == 0;
    }

    std::vector<std::string> string_split(const std::string& str, char delimeter) {
        std::vector<std::string> res;
        std::string temp;
        std::istringstream stream(str.c_str());

        while(std::getline(stream, temp, delimeter))
            res.push_back(temp);

        return res;
    }

    std::string string_replace(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
}