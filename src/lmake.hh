#pragma once 

#include <string>

#include "luavm.hh"

class lmake {
public:
    lmake(const lmake&) = delete;

    static lmake* get();

    bool build(const std::string config_path);
private:
    lmake() = default;
    ~lmake() = default;

    static lmake* instance;
    
    luavm vm;
protected:
};