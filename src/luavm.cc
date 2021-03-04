/*
 * This project is provided under the GNU GPL v2 license, more information can 
 * found on https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    - Iker Galardi
 */

#include "luavm.hh"

#include <iostream>
#include <cstring>

#include <spdlog/spdlog.h>

luavm::luavm() {
    vm = luaL_newstate();
    luaL_openlibs(vm);

    if(vm == NULL)
        std::cout << "Error while creating the lua virtual machine\n";
}

luavm::~luavm() {
    lua_close(vm);
}

void luavm::add_native_function(luafunc func, std::string name) {
    lua_pushcfunction(vm, func);
    lua_setglobal(vm, name.c_str());
}

/// TODO: why return anything if always is true what you return
bool luavm::execute_script(std::string script) {
    this->script = script;

    // Loads the script into lua virtual machine and tries to execute,
    // if an error has ocurred last_error saves the lua execution error.
    luaL_loadstring(vm, script.c_str());
    if (lua_pcall(vm, 0, 0, 0) != LUA_OK) {
        auto execution_error = std::string(lua_tostring(vm, -1));
        spdlog::error("Script error: {}", execution_error);
        std::exit(-1);
    }
    
    return true;
}

bool luavm::function_exists(std::string fn_name) {
    // Selects the given function name in the stack and checks if 
    // its a function.
    lua_getglobal(vm, fn_name.c_str());
    if(lua_isfunction(vm, -1)) {
        return true;
    }

    return false;
}

void luavm::execute_function(std::string fn_name) {
    // Selects the given function and calls it
    lua_getglobal(vm, fn_name.c_str());
    if(lua_pcall(vm, 0, 0, 0) != LUA_OK) {
        auto execution_error = std::string(lua_tostring(vm, -1));
        spdlog::error("Script error: {}", execution_error);
        std::exit(-1);
    }
}

void luavm::change_variable(const std::string& name, const std::string& value) {
    // Selects the given variable
    lua_getglobal(vm, name.c_str());

    // Removes the value from the stack
    lua_remove(vm, 1);

    // Pushes the new value and sets name to that
    // stack index
    lua_pushstring(vm, value.c_str());
    lua_setglobal(vm, name.c_str());
}

std::string luavm::get_last_error() {
    return std::string("[LVM] ") + last_error;
}