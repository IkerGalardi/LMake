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
 *    - Iñigo Gastesi
 */

#include "lmake.hh"

#include <memory>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <stack>
#include <regex>
#include <string.h>

#include <stringtoolbox/stringtoolbox.hh>
#include <spdlog/spdlog.h>

#include "luavm.hh"
#include "os/filesystem.hh"
#include "os/process_management.hh"
#include "utils.hh"
#include "lmake_func.hh"

/// TODO: std::exit() wrong, stop executing script and set last error


#define PRINT_IF(m, b) if(b) std::cout << m << std::endl

luavm vm;

namespace lmake {
    void initialize(const settings& settings) {
        lmake::func::set_settings(settings);

        lmake::func::chdir(os::get_dir());

        vm.add_native_function([](lua_State* vm) -> int {
            const char* to_include_path = lua_tostring(vm, -1);

            // Check if file exits, if not erro and exit
            if(!os::file_exists(to_include_path)) {
                spdlog::error("The files {} can't be opened.", to_include_path);
                std::exit(1);
            }

            auto to_include = os::read_file(to_include_path);
            luaL_loadstring(vm, to_include.get());
            if(lua_pcall(vm, 0, 0, 0) != LUA_OK) {
                spdlog::error("Error including file: {}", lua_tostring(vm, -1));
                std::exit(1);
            }

            return 1;
        }, "lmake_include");
        
        vm.add_native_function([](lua_State* vm) -> int {
            auto version = lua_tonumber(vm, -1);
            lmake::func::compatibility_version(version);
            return 1;
        }, "lmake_compatibility_version");

        vm.add_native_function([](lua_State* vm) -> int {
            lmake::func::set_compiler(std::string(lua_tostring(vm, -1)));
            return 1;
        }, "lmake_set_compiler");

        vm.add_native_function([](lua_State* vm) -> int {
            lmake::func::set_compiler_flags(std::string(lua_tostring(vm, -1)));
            return 1;
        }, "lmake_set_compiler_flags");

        vm.add_native_function([](lua_State* vm) -> int {
            std::string out_regex = std::string(lua_tostring(vm, -1));
            lmake::func::set_compiler_out(out_regex);
            return 1;
        }, "lmake_set_compiler_out");

        vm.add_native_function([](lua_State* vm) -> int {
            std::string source_files = std::string(lua_tostring(vm, -1));
            lmake::func::compile(source_files);
            return 1;
        }, "lmake_compile");

        vm.add_native_function([](lua_State* vm) -> int {
            lmake::func::set_linker(std::string(lua_tostring(vm, -1)));
            return 1;
        }, "lmake_set_linker");

        vm.add_native_function([](lua_State* vm) -> int {
            lmake::func::set_linker_flags(std::string(lua_tostring(vm, -1)));
            return 1;
        }, "lmake_set_linker_flags");

        vm.add_native_function([](lua_State* vm) -> int {
            lmake::func::set_linker_out(std::string(lua_tostring(vm, -1)));
            return 1;
        }, "lmake_set_linker_out");

        vm.add_native_function([](lua_State* vm) -> int {
            std::string obj_files = std::string(lua_tostring(vm, -1));
            lmake::func::link(obj_files);
            return 1;
        }, "lmake_link");

        vm.add_native_function([](lua_State* vm) -> int {
            std::string dir = std::string(lua_tostring(vm, -1));
            lmake::func::chdir(dir);
            return 1;
        }, "lmake_chdir");

        vm.add_native_function([](lua_State* vm) -> int {
            lmake::func::last_dir();
            return 1;
        }, "lmake_chdir_last");


        vm.add_native_function([](lua_State* vm) -> int {
            std::string command = lua_tostring(vm, -1);
            int exit_code = lmake::func::exec(command);
            lua_pushnumber(vm, (lua_Number)exit_code);

            return 1;
        }, "lmake_exec");
       
        vm.add_native_function([](lua_State* vm) -> int {
            std::string to_match = std::string(lua_tostring(vm, -1));

            // Checks what regex to use and calls the corresponding functions
            size_t double_pos = to_match.find("**");
            size_t single_pos = to_match.find("*");
            if(double_pos != std::string::npos) {
                std::string result = lmake::func::find_recursive(to_match);
                lua_pushstring(vm, result.c_str());
                return 1;
            } else if(single_pos != std::string::npos) {
                std::string result = lmake::func::find(to_match);
                lua_pushstring(vm, result.c_str());
                return 1;
            } else {
                spdlog::error("There is no regex in: {}", to_match);
                std::exit(1);
            }
            return 1;
        }, "lmake_find");


        vm.add_native_function([](lua_State* vm) -> int {
            std::string msg = std::string(lua_tostring(vm, -1));
            lmake::func::error(msg); 
            return 1;
        }, "lmake_error");
    }

    void load_from_file(std::string config_path) {
        auto file_buffer = os::read_file(config_path);
        lmake::load_from_string(file_buffer.get());
    }

    void load_from_string(std::string config_string) {
        if(!vm.execute_script(config_string)) {
            spdlog::error("An error has ocurred when executing script");
            std::exit(1);
        }
    }

    void execute_target(std::string target) {
        if(!vm.function_exists(target)) {
            spdlog::error("Target {} does not exist.", target);
            std::exit(1);
        }

        vm.execute_function(target);
    }
    
    void change_variable(const std::string& name, std::string& value) {
        vm.change_variable(name, value);
    }
}
