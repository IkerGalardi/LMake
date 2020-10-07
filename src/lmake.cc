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

#include "lmake.hh"

#include <memory>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <stack>

#include <stringtoolbox/stringtoolbox.hh>

#include "luavm.hh"
#include "os/filesystem.hh"
#include "os/process_management.hh"
#include "lmake.hh"

/// TODO: std::exit() wrong, stop executing script and set last error

static struct {
    luavm vm;

    struct {
        std::string compiler;
        std::string compiler_flags;
        std::string compiler_output;

        std::string linker;
        std::string linker_flags;
        std::string linker_output;
    } context;

    std::stack<std::string> been_dirs;

    bool initialized = false;
    bool config_executed = false;

    std::string last_error;
} lmake_data;

#define DEBUG(x) std::cout << "[D] " << x << std::endl

std::string process_script(const char* file_contents, const char* containing_dir) {
    /// TODO: preprocess all the lmake_include (mimic #include of c)
    std::stringstream stream(file_contents);
    std::string res;

    std::string temp;
    while(std::getline(stream, temp)) {
        if(temp.find("lmake_include") != std::string::npos) {
            size_t bracket_left_index = temp.find("(\"");
            size_t bracket_right_index = temp.find("\")");
            std::string substring = temp.substr(bracket_left_index, bracket_right_index);
            DEBUG(substring);
        } else {
            res.append(temp);
            res.append("\n");
        }
    }

    //DEBUG(res);

    return std::string(file_contents);

}

static std::vector<std::string> string_split(const std::string& str, char delimeter) {
    std::vector<std::string> res;
    std::string temp;
    std::istringstream stream(str.c_str());

    while(std::getline(stream, temp, delimeter))
        res.push_back(temp);

    return res;
}

std::string string_replace(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

namespace lmake {
    void initialize() {
        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            auto compatibility_version = lua_tonumber(vm, -1);
            if(compatibility_version != LMAKE_COMPAT_VERSION) {
                std::cerr << "[E] Incompatible version\n";
                std::exit(0);
            }
            return 1;
        }, "lmake_compatibility_version");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler = std::string(lua_tostring(vm, -1));
            return 1;
        }, "lmake_set_compiler");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler_flags = std::string(lua_tostring(vm, -1));
            return 1;
        }, "lmake_set_compiler_flags");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler_output = std::string(lua_tostring(vm, -1));
            return 1;
        }, "lmake_set_compiler_output");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string source_files = std::string(lua_tostring(vm, -1));
    
            // Create output paths from file
            std::vector<std::string> files = string_split(source_files, ' ');
            std::vector<std::string> obj_file_names;
            std::vector<std::string> src_files;
            obj_file_names.reserve(files.size());
            for(int i = 0; i < files.size(); i++) {
                std::string& file = files[i];
                std::filesystem::path filepath(file);
                std::string filename_without_path = filepath.filename().string();
                src_files.push_back(filename_without_path);
                std::string full_obj_path = lmake_data.context.compiler_output + "/" 
                                          + string_replace(filename_without_path, "%", filename_without_path);
                obj_file_names.emplace_back(full_obj_path);
            }

            // Compile all the files
            for(int i = 0; i < files.size(); i++) {
                std::cout << "[+] Compiling " << src_files[i] << std::endl;
                std::string& compiler = lmake_data.context.compiler;
                std::string& flags = lmake_data.context.compiler_flags;

                // Run the compiler and get exit code
                std::string args =  files[i] + " -c " + flags + " -o " + obj_file_names[i] + ".o";
                //std::cout << args.c_str() << std::endl;
                os::process p = os::run_process(compiler.c_str(), args.c_str());
                int exit = os::wait_process(p);

                // if compilation gone wrong exit the program
                if(exit != 0) 
                    std::exit(1);

            }

            std::cout << std::endl;
            
            return 1;
        }, "lmake_compile");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.linker = std::string(lua_tostring(vm, -1));
            return 1;
        }, "lmake_set_linker");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.linker_flags = std::string(lua_tostring(vm, -1));
            //DEBUG(lmake_data.context.linker_flags);
            return 1;
        }, "lmake_set_linker_flags");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            DEBUG("debus desl test");
            lmake_data.context.linker_output = std::string(lua_tostring(vm, -1));
            DEBUG(lmake_data.context.linker_output);
            return 1;
        }, "lmake_set_linker_out");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string obj_files = std::string(lua_tostring(vm, -1));

            std::string& linker = lmake_data.context.linker; 
            std::string& flags = lmake_data.context.linker_flags;
            std::string& output = lmake_data.context.linker_output;

            std::filesystem::path output_path(output);
            std::cout << "[+] Linking " << output_path.filename().string() << std::endl;

            // Construct the args
            std::string args = "-o " + output + " " + obj_files + " " + flags;

            // Execute the linker with the constructed args
            os::process p = os::run_process(linker.c_str(), args.c_str());
            int exit_code = os::wait_process(p);

            /// TODO: stop executing and error
            if(exit_code != 0) {
                std::exit(1);
            }

            std::cout << std::endl;

            return 1;
        }, "lmake_link");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string dir = std::string(lua_tostring(vm, -1));
            lmake_data.been_dirs.push(os::get_dir());
            bool err = os::change_dir(dir.c_str());
            /// TODO: if error -> stop executing and set last error 
            return 1;
        }, "lmake_set_linker_output");

        lmake_data.initialized = true;
        lmake_data.config_executed = false;
    }

    bool load_from_file(const char* config_path) {
        if(!lmake_data.initialized) 
            return false;

        auto file_buffer = os::read_file(config_path);
        std::string processed = process_script(file_buffer.get(), config_path);
        return lmake::load_from_string(processed.c_str());
    }

    bool load_from_string(const char* config_string) {
        if(!lmake_data.initialized) 
            return false;

        if(!lmake_data.vm.execute_script(config_string)) {
            lmake_data.last_error = lmake_data.vm.get_last_error();
            return false;
        }

        lmake_data.config_executed = true;

        return true;
    }

    bool execute_target(const char* target) {
        if(!lmake_data.config_executed) {
            lmake_data.last_error = "No configuration executed";
            return false;
        }

        if(!lmake_data.vm.function_exists(target)) {
            lmake_data.last_error = "Specified target does not exist";
            return false;
        }

        lmake_data.vm.execute_function(target);

        return true;
    }

    std::string& get_last_error() {
        return lmake_data.last_error;
    }
}