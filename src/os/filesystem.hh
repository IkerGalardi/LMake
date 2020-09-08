#pragma once 

#include <memory>

namespace os {

    /*
     * Check if the file exists on the filesystem
     * 
     * @param path: path to the file
     */
    bool file_exists(const char* path);

    /*
     * Reads the file and returns the buffer
     * 
     * @param path: path to the file
     */
    std::shared_ptr<char> read_file(const char* path);
}