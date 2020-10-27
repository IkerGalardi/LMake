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
#include "utils.hh"

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

#define PRINT_IF(m, b) if(b) std::cout << m << std::endl

void print_context() {
    std::cout << "CONTEXT\n";
    std::cout << "  Compiler  = " << lmake_data.context.compiler << std::endl;
    std::cout << "     Flags  = " << lmake_data.context.compiler_flags << std::endl;
    std::cout << "     Output = " << lmake_data.context.compiler_output << std::endl;

    std::cout << "  Linker = " << lmake_data.context.linker << std::endl;
    std::cout << "     Flags = " << lmake_data.context.linker_flags << std::endl;
    std::cout << "     Output = " << lmake_data.context.linker_output << std::endl;

    std::cout << "END CONTEXT\n";
}

#define DEBUG(x) std::cout << "[D] " << x << std::endl

static std::string process_script(std::string file_contents, std::string containing_dir) {
    /// TODO: preprocess all the lmake_include (mimic #include of c)
    std::stringstream stream(file_contents);
    std::string res;

    std::string temp;
    while(std::getline(stream, temp)) {
        if(temp.find("lmake_include") != std::string::npos) {
            // Get the parameter passed to lmake_include command
            size_t bracket_left_index = temp.find("(\"");
            size_t bracket_right_index = temp.find("\")");
            std::string substring = temp.substr(bracket_left_index, bracket_right_index);

            // Check if file exists, if not, throw an error and quit
            if(!os::file_exists(substring.c_str())) {
                /// TODO: maybe print the line in which the file is trying to be included
                std::cerr << "[E] The file " << substring << " can't be opened.\n"; 
                std::exit(1);
            }

            // Read the file to the postprocessed script contents
            auto file_contents = os::read_file(substring);
            res.append("\n");
            res.append(std::string(file_contents.get()));
            res.append("\n");
        } else {
            res.append(temp);
            res.append("\n");
        }
    }

    return std::string(file_contents);
}

namespace lmake {
    void initialize() {
        std::string current_dir = os::get_dir();
        lmake_data.been_dirs.push(current_dir);

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
            PRINT_IF("Compiler set to " << lmake_data.context.compiler, false);
            return 1;
        }, "lmake_set_compiler");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler_flags = std::string(lua_tostring(vm, -1));
            PRINT_IF("Compiler flags set to " << lmake_data.context.compiler_flags, false);
            return 1;
        }, "lmake_set_compiler_flags");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string out_regex = std::string(lua_tostring(vm, -1));
            
            if(out_regex.find('%') == std::string::npos) {
                std::cerr << "[E] No regex added.\n";
                std::exit(1);
            }

            lmake_data.context.compiler_output = out_regex;

            PRINT_IF("Compiler out set to " << lmake_data.context.compiler_output, false);
            return 1;
        }, "lmake_set_compiler_out");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string source_files = std::string(lua_tostring(vm, -1));
            PRINT_IF("FILES = " << source_files, false);
    
            // Compile the file
            std::vector<std::string> files = utils::string_split(source_files, ' ');
            for(int i = 0; i < files.size(); i++) {
                std::string& compiler = lmake_data.context.compiler;
                std::string& flags = lmake_data.context.compiler_flags;
                std::string& out = lmake_data.context.compiler_output;

                std::filesystem::path file_path(files[i]);
                std::string file_without_path = file_path.stem().string() + file_path.extension().string();

                std::string obj_name = utils::string_replace(
                    out,
                    "%",
                    file_without_path
                );

                if(os::file_exists(obj_name)) {
                    if(!os::compare_file_dates(obj_name, files[0])) {
                        continue;
                    }
                }

                std::cout << "[+] Compiling " << files[i] << std::endl;
                bool ok = utils::compile(
                    compiler.c_str(),
                    flags.c_str(),
                    files[i].c_str(),
                    obj_name.c_str()
                );

                if(!ok) 
                    std::exit(1);
            }

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
            lmake_data.context.linker_output = std::string(lua_tostring(vm, -1));
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

            return 1;
        }, "lmake_link");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string dir = std::string(lua_tostring(vm, -1));

            if(os::change_dir(dir.c_str())) {
                lmake_data.been_dirs.push(os::get_dir());
            } else {
                std::cerr << "[E] Specified directory can't be entered.\n";
                std::exit(1);
            }

            return 1;
        }, "lmake_chdir");

        lmake_data.initialized = true;
        lmake_data.config_executed = false;
    }

    bool load_from_file(std::string config_path) {
        if(!lmake_data.initialized) 
            return false;

        auto file_buffer = os::read_file(config_path);
        std::string processed = process_script(file_buffer.get(), config_path);
        return lmake::load_from_string(processed.c_str());
    }

    bool load_from_string(std::string config_string) {
        if(!lmake_data.initialized) 
            return false;

        if(!lmake_data.vm.execute_script(config_string)) {
            lmake_data.last_error = lmake_data.vm.get_last_error();
            return false;
        }

        lmake_data.config_executed = true;

        return true;
    }

    bool execute_target(std::string target) {
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
