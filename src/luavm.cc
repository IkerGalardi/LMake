#include "luavm.hh"

#include <iostream>

luavm::luavm() {
    vm = luaL_newstate();

    if(vm == NULL)
        std::cout << "Error while creating the lua virtual machine\n";
}

luavm::~luavm() {
    lua_close(vm);
}

void luavm::add_native_function(luafunc func, const char* name) {
    lua_pushcfunction(vm, func);
    lua_setglobal(vm, name);
}

bool luavm::execute_script(const char* script) {
    luaL_loadstring(vm, script);
    if (lua_pcall(vm, 0, 0, 0) != LUA_OK) {
        last_error = std::string(lua_tostring(vm, -1));
        return false;
    }
    
    return true;
}

const std::string& luavm::get_last_error() {
    return last_error;
}