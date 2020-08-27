#include "lmake.hh"

lmake* lmake::instance = nullptr;

lmake* lmake::get() {
    if(!instance) 
        instance = new lmake();

    return instance;
}

bool lmake::build(const std::string config_path) {
    // Reads the configuration file from the filesystem
    FILE* config_file = fopen(config_path.c_str(), "r");
    fseek(config_file, 0, SEEK_END);
    int length = ftell(config_file);
    char* config = static_cast<char*>(std::malloc(length + 1));
    fread(config, 1, length, config_file);
    config[length] = '\0';

    // TODO: setup native methods for the luavm
    // TODO: run the script

    std::free(config);
}