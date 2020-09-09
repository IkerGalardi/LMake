#include "luavm.hh"

luavm::luavm() {
    vm = luaL_newstate();
}

luavm::~luavm() {
    lua_close(vm);
}

void luavm::add_native_function(luafunc func, const char* name) {
    lua_pushcfunction(vm, func);
    lua_setglobal(vm, name);
}

bool luavm::execute_script(const char* script) {
    bool res = luaL_dostring(vm, script);
    // TODO: get error type
    last_error = "Lua execution error";
    return res;
}

const std::string& luavm::get_last_error() {
    return last_error;
}