#include "filesystem.hh"

#include <cstdio>

namespace os {
    bool file_exists(const char* path) {
        FILE *file;
        if ((file = fopen(path, "r"))) {
            fclose(file);
            return true;
        }
        return false;
    }
}