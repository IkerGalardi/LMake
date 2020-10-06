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

bool luavm::function_exists(const char* fn_name) {
    lua_getglobal(vm, fn_name);
    if(lua_isfunction(vm, -1)) {
        return true;
    }

    return false;
}

void luavm::execute_function(const char* fn_name) {
    lua_getglobal(vm, fn_name);
    lua_pcall(vm, 0, 0, 0);
}


const std::string& luavm::get_last_error() {
    return last_error;
}