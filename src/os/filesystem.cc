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

    std::shared_ptr<char> read_file(const char* path) {
        FILE* file_path = fopen(path, "r");
        fseek(file_path, 0, SEEK_END);
        int length = ftell(file_path);

        std::shared_ptr<char> buffer(new char[length + 1], std::default_delete<char[]>());
        std::fread(buffer.get(), 1, length, file_path);
        std::fclose(file_path);

        buffer.get()[length] = '\0';

        return buffer;
    }
}