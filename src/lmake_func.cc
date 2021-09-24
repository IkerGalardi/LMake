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

#include "lmake_func.hh"

#include <memory>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <stack>
#include <regex>

#include <stringtoolbox/stringtoolbox.hh>
#include <spdlog/spdlog.h>

#include "luavm.hh"
#include "os/filesystem.hh"
#include "os/process_management.hh"
#include "lmake.hh"
#include "utils.hh" 

static struct {
    struct {
        std::string compiler;
        std::string compiler_flags;
        std::string compiler_output;

        std::string linker;
        std::string linker_flags;
        std::string linker_output;
    } context;

    std::stack<std::string> been_dirs;

    std::string last_error;

    lmake::settings settings;
} lmake_data;


namespace lmake { namespace func {

    void set_settings(const settings& settings) {
        lmake_data.settings = settings;
    }

    void compatibility_version(const std::string& version) {
        auto versions = stringtoolbox::split(version, '.');
        if(versions.size() != 3) {
            spdlog::error("Incorrect formatting of version");
            std::exit(-1);
        }
        try {
            if(std::stoi(versions[0]) != LMAKE_VERSION_MAJOR) {
                spdlog::error("Major versions differ, incompatible file");
                std::exit(-1);
            } else if(std::stoi(versions[1]) < LMAKE_VERSION_MINOR) {
                spdlog::error("Minor version is smaller, some functions might not be implemented");
                std::exit(-1);
            }
        } catch(const std::exception& e) {
            spdlog::error("Incorrect formatting of version");
            std::exit(-1);
        }
    }

    void set_compiler(const std::string& compiler) {
        if(lmake_data.settings.debug)
            spdlog::info("Compiler set to: {}", compiler);

        lmake_data.context.compiler = compiler;        
    }

    void set_compiler_flags(const std::string& flags) {
        if(lmake_data.settings.debug)
            spdlog::info("Compiler flags set to: {}", flags);

        lmake_data.context.compiler_flags = flags;
    }

    void set_compiler_out(const std::string& out_regex) {
        if(lmake_data.settings.debug)
            spdlog::info("Compiler output set to: {}", out_regex);
        
        if(out_regex.find('%') == std::string::npos) {
            spdlog::error("No regex added.");
            std::exit(1);
        }

        lmake_data.context.compiler_output = out_regex;
    }

    void compile(const std::string& source_files) {
        std::vector<std::string> files = utils::string_split(source_files, ' ');
        for(int i = 0; i < files.size(); i++) {
            const std::string& compiler = lmake_data.context.compiler;
            const std::string& flags = lmake_data.context.compiler_flags;
            const std::string& out = lmake_data.context.compiler_output;

            std::filesystem::path file_path(files[i]);
            std::string file_without_path = file_path.stem().string() + file_path.extension().string();

            std::string obj_name = utils::string_replace(
                out,
                "%",
                file_without_path
            );

            /// TODO: rewrite this pls
            if(!lmake_data.settings.force_recompile) {
                if(os::file_exists(obj_name)) {
                    // Check file dates and if files exists
                    if(os::file_exists(files[i]) && !os::compare_file_dates(obj_name, files[i])) {
                        continue;
                    }
                }
            }

            if(lmake_data.settings.verbose) {
                std::cout << "[+] " << compiler + " " + files[i] + " -c " + flags + " -o " + obj_name << std::endl;
            } else {
                std::cout << "[+] Compiling " << files[i] << std::endl;
            }
            bool ok = utils::compile(
                compiler.c_str(),
                flags.c_str(),
                files[i].c_str(),
                obj_name.c_str()
            );

            if(!ok) 
                std::exit(1);
        }
    }

    void set_linker(const std::string& linker) {
        if(lmake_data.settings.debug) 
            spdlog::info("Linker set to: {}", linker);
        
        lmake_data.context.linker = linker;
    }

    void set_linker_flags(const std::string& flags) {
        if(lmake_data.settings.debug)
            spdlog::info("Linker flags set to: {}", flags);
        
        lmake_data.context.linker_flags = flags;
    }

    void set_linker_out(const std::string& out_regex) {
        if(lmake_data.settings.debug)
            spdlog::info("Linker output set to: {}");
        
        lmake_data.context.linker_output = out_regex;
    }

    void link(const std::string& object_files) {
        const std::string& linker = lmake_data.context.linker; 
        const std::string& flags = lmake_data.context.linker_flags;
        const std::string& output = lmake_data.context.linker_output;

        std::filesystem::path output_path(output);

        // Construct the args
        std::string args = flags + " " + object_files + " " + "-o " + output;

        if(lmake_data.settings.verbose) {
            std::cout << "[+] " << linker + " " + args << std::endl;
        } else  {
            std::cout << "[+] Linking " << output_path.filename().string() << std::endl;
        }

        // Execute the linker with the constructed args
        bool ok = utils::link(linker, flags, object_files, output);

        if(!ok) {
            std::exit(1);
        }
    }

    void chdir(const std::string& dir) {
        if(lmake_data.settings.debug)
            spdlog::info("Changing directory to: {}", dir);
        
        if(os::change_dir(dir.c_str())) {
            lmake_data.been_dirs.push(os::get_dir());
        } else {
            spdlog::error("Specified directory can't be entered.");
            std::exit(1);
        }
    }

    void last_dir() {
        if(lmake_data.settings.debug)
            spdlog::info("Changing to last working directory");
        
        std::stack<std::string>& been_dirs = lmake_data.been_dirs;
            
        // Check if it can go back a directory
        if(been_dirs.empty() || been_dirs.size() == 1) {
            spdlog::error("Can't go back a directory");
            std::exit(1);
        }

        // Take the previous directory
        been_dirs.pop();
        std::string prev_dir = been_dirs.top();
            
        // Change to the previous directory
        if(!os::change_dir(prev_dir)) {
            spdlog::error("Specified directory can't be entered.");
            std::exit(1);
        }
    }

    int exec(const std::string& command) {
        auto splited_params = utils::string_split(command, ' ');

        if(splited_params.size() == 0) {
            spdlog::error("Unknown syntax.");
            std::exit(1);
        }

        /// TODO: maybe look inside PATH instead of manual checks
        std::string real_prog;
        if(os::file_exists(splited_params[0])) {
            real_prog = splited_params[0];
        } else if(os::file_exists("/bin/" + splited_params[0])) {
            real_prog = "/bin/" + splited_params[0];
        } else if(os::file_exists("/usr/bin/" + splited_params[0])) {
            real_prog = "/usr/bin/" + splited_params[0];
        } else {
            spdlog::error("%s was not found.", splited_params[0].c_str());
        }

        std::string params;
        for(int i = 1; i < splited_params.size(); i++) {
            params.append(splited_params[i] + " ");
        }

        if(lmake_data.settings.verbose) {
            std::cout << "[+] " << command << std::endl;
        } else {
            std::cout << "[+] Executing " << splited_params[0] << std::endl;
        }
        os::process p = os::run_process(real_prog, params);
        int exit = os::wait_process(p);
        return exit;
    }

    void error(const std::string& msg) {
        spdlog::error(msg.c_str());
        std::exit(2);
    }

    std::string find(const std::string& regex) {
        if(lmake_data.settings.debug)
            spdlog::info("Finding with regex: {}", regex);
        
        const std::string template_regex_complete = "^%[a-zA-Z0-9_.]*?$"; // % by left part, ? by right part

        size_t single_pos = regex.find("*");

        // Gets the left and right parts of the "*" to search in the necessary files
        std::string left_part = regex.substr(0, single_pos);
        std::string right_part = regex.substr(single_pos + 1, regex.size() - single_pos);

        // Builds the C++ regex from the custom one
        auto regex_complete = utils::string_replace(
            template_regex_complete,
            "%",
            left_part
        );
        regex_complete = utils::string_replace(
            regex_complete,
            "?",
            right_part
        );
        regex_complete = utils::string_replace(
            regex_complete,
            ".",
            "\\."
        );

        const std::regex regex_obj(regex_complete);
        std::smatch match;
        std::string path = os::file_dir(regex);

        // When path is returned empty that means that is the current path
        // if not specified by "." a filesystem exception is thrown
        if(path.empty()) {
            path = "./";
        }

        /// TODO: error checking from directory iterator??
        std::string result;
        std::filesystem::directory_iterator dir_iterator(path);
        for(auto& dir_entry : dir_iterator) {
            if(dir_entry.is_directory()) 
                continue;

            // Check if the regex matches, if its true add it to the result list
            const auto& entry_string = dir_entry.path().string();
            if(std::regex_search(entry_string, match, regex_obj))
                result.append(entry_string + " ");
        }

        return result;
    }

    std::string find_recursive(const std::string& regex) {
        if(lmake_data.settings.debug)
            spdlog::info("Finding recursivelly with regex {}", regex);
        
        size_t double_index = regex.find("**");
        std::string left_part = regex.substr(0, double_index);
        std::string right_part = regex.substr(double_index + 2, regex.size() - double_index);

        // Find recursivelly the files
        std::string result = "";
        auto files = os::list_dir(left_part);
        for(std::string& file : files) {
            if(std::filesystem::is_directory(file)) {
                std::string new_regex = file + "/**" + right_part;
                result.append(find_recursive(new_regex));
            }
        }

        // Find the files in the base directory
        auto new_regex = utils::string_replace(
            regex,
            "**",
            "*"
        );
        result.append(find(new_regex));
        return result;
    }
} }