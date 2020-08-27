#pragma once

#include <string>

#include "lua.hpp"

typedef int(*luafunc)(lua_State*);

class luavm {
public:
    luavm();
    ~luavm();

    void add_native_function(luafunc func, const char* name);
    bool execute_script(const std::string& script);
private:
    lua_State* vm;
protected:
};