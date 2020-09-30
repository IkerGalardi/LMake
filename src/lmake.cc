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

#include <stringtoolbox/stringtoolbox.hh>

#include "luavm.hh"
#include "os/filesystem.hh"
#include "os/process_management.hh"

#define DEBUG(x) std::cout << "[D] " << x << std::endl

static struct {
    luavm vm;

    struct {
        std::string compiler;
        std::string compiler_flags;
        std::string compiler_output;

        std::string linker;
        std::string linker_flags;
    } context;

    bool initialized = false;

    std::string last_error;
} lmake_data;

std::string process_script(const char* file_contents, const char* containing_dir) {
    /// TODO: preprocess all the lmake_include (mimic #include of c)    
    return std::string(file_contents);
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
        DEBUG("Adding native functions");
        /// TODO: add native functions
        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler = std::string(lua_tostring(vm, -1));
            DEBUG(lmake_data.context.compiler);
            return 1;
        }, "lmake_set_compiler");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler_flags = std::string(lua_tostring(vm, -1));
            DEBUG(lmake_data.context.compiler_flags);
            return 1;
        }, "lmake_set_compiler_flags");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler_output = std::string(lua_tostring(vm, -1));
            DEBUG(lmake_data.context.compiler_output);
            return 1;
        }, "lmake_set_compiler_output");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string source_files = std::string(lua_tostring(vm, -1));
            DEBUG(source_files);
    
            // Create output paths from file
            std::vector<std::string> files = stringtoolbox::split(source_files, ' ');
            std::vector<std::string> obj_file_names;
            std::vector<std::string> src_files;
            obj_file_names.reserve(files.size());
            for(int i = 0; i < files.size(); i++) {
                std::string& file = files[i];
                std::filesystem::path filepath(file);
                std::string filename_without_path = filepath.filename().string();
                src_files.push_back(filename_without_path);
                std::string full_obj_path = lmake_data.context.compiler_output
                                          + string_replace(filename_without_path, "%", filename_without_path);
                obj_file_names.emplace_back(full_obj_path);
            }

            // Compile all the files
            for(int i = 0; i < files.size(); i++) {
                std::cout << "[+] Compiling " << src_files[i] << std::endl;
                std::string& compiler = lmake_data.context.compiler;
                std::string& flags = lmake_data.context.compiler_flags;

                // Run the compiler and get exit code
                std::string args = flags + "-o " + obj_file_names[i];
                os::process p = os::run_process(compiler.c_str(), args.c_str());
                int exit = os::wait_process(p);

                // if compilation gone wrong exit the program
                if(exit == 0) 
                    std::exit(1);
            }
            
            return 1;
        }, "lmake_compile");
        DEBUG("Native functions added");

        lmake_data.initialized = true;
    }

    bool build_from_file(const char* config_path) {
        if(!lmake_data.initialized) 
            return false;

        DEBUG("Processing script");
        auto file_buffer = os::read_file(config_path);
        std::string processed = process_script(file_buffer.get(), config_path);
        DEBUG(processed);
        return lmake::build_from_string(processed.c_str());
    }

    bool build_from_string(const char* config_string) {
        if(!lmake_data.initialized) 
            return false;

        if(!lmake_data.vm.execute_script(config_string)) {
            lmake_data.last_error = lmake_data.vm.get_last_error();
            return false;
        }

        return true;
    }

    std::string& get_last_error() {
        return lmake_data.last_error;
    }
}