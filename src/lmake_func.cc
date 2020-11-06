#include "lmake_func.hh"

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

#define PRINT_IF(m, b) if(b) std::cout << m << std::endl

#define DEBUG(x) std::cout << "[D] " << x << std::endl

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

    bool initialized = false;
    bool config_executed = false;

    std::string last_error;

    lmake::settings settings;
} lmake_data;


namespace lmake { namespace func {

    void set_settings(const settings& settings) {
        lmake_data.settings = settings;
    }

    void compatibility_version(float compatibility_version) {
        if(compatibility_version != LMAKE_COMPAT_VERSION) {
            std::cerr << "[E] Incompatible version\n";
            std::exit(0);
        }
    }

    void set_compiler(const std::string& compiler) {
        lmake_data.context.compiler = compiler;        
    }

    void set_compiler_flags(const std::string& flags) {
        lmake_data.context.compiler_flags = flags;
    }

    void set_compiler_out(const std::string& out_regex) {
        if(out_regex.find('%') == std::string::npos) {
            std::cerr << "[E] No regex added.\n";
            std::exit(1);
        }

        lmake_data.context.compiler_output = out_regex;
    }

    void compile(const std::string& source_files) {
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

            /// TODO: rewrite this pls
            if(!lmake_data.settings.force_recompile) {
                if(os::file_exists(obj_name)) {
                    if(!os::compare_file_dates(obj_name, files[i])) {
                        continue;
                    }
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
    }

    void set_linker(const std::string& linker) {
        lmake_data.context.linker = linker;
    }

    void set_linker_flags(const std::string& flags) {
        lmake_data.context.linker_flags = flags;
    }

    void set_linker_out(const std::string& out_regex) {
        lmake_data.context.linker_output = out_regex;
    }

    void link(const std::string& object_files) {
        std::string& linker = lmake_data.context.linker; 
        std::string& flags = lmake_data.context.linker_flags;
        std::string& output = lmake_data.context.linker_output;

        std::filesystem::path output_path(output);
        std::cout << "[+] Linking " << output_path.filename().string() << std::endl;

        // Construct the args
        std::string args = "-o " + output + " " + object_files + " " + flags;

        // Execute the linker with the constructed args
        os::process p = os::run_process(linker.c_str(), args.c_str());
        int exit_code = os::wait_process(p);

        if(exit_code != 0) {
            std::exit(1);
        }
    }

    void chdir(const std::string& dir) {
        if(os::change_dir(dir.c_str())) {
            lmake_data.been_dirs.push(os::get_dir());
        } else {
            std::cerr << "[E] Specified directory can't be entered.\n";
            std::exit(1);
        }
    }

    void last_dir() {
        std::stack<std::string>& been_dirs = lmake_data.been_dirs;
            
        // Check if it can go back a directory
        if(been_dirs.empty() || been_dirs.size() == 1) {
            std::cerr << "[E] Can't go back a directory\n";
            std::exit(1);
        }

        // Take the previous directory
        been_dirs.pop();
        std::string prev_dir = been_dirs.top();
            
        // Change to the previous directory
        if(!os::change_dir(prev_dir)) {
            std::cerr << "[E] Specified directory can't be entered.\n";
            std::exit(1);
        }
    }

    int exec(const std::string command) {
        auto splited_params = utils::string_split(command, ' ');

        if(splited_params.size() == 0) {
            std::cerr << "[E] Unknown syntax.\n";
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
            std::cerr << "[E] " << splited_params[0] << " was not found\n";
        }

        std::string params;
        for(int i = 1; i < splited_params.size(); i++) {
            params.append(splited_params[i] + " ");
        }

        os::process p = os::run_process(real_prog, params);
        int exit = os::wait_process(p);
        return exit;
    }

    void error(const std::string msg) {
        std::cout << "[E] " << msg << std::endl;
        std::exit(2);
    }

} }