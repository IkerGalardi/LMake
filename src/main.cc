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

#include "pch.hh"

#include <iostream>
#include <cstring>

#include <lua/lua.hpp>
#include <spdlog/spdlog.h>

#include "os/filesystem.hh"
#include "lmake.hh"
#include "test/test.hh"

#define LMAKE_CONFIG_PATH "./lmake.lua"

int main(int argc, char** argv) {
    spdlog::set_pattern("%^[%l] %v%$");

    // Checks if "--help" flag is passed, if it is information is printed
    if(argc <= 1 || std::strcmp(argv[1], "--help") == 0) {
        std::cout << "[+] Usage: lmake <target> <flags>\n";
        std::cout << "    · target: lua function to be executed.\n";
        std::cout << "    · flags: aditional flags.\n";
        std::cout << "[+] Usage: lmake --help\n";
        std::cout << "    Shows this very helpful message\n";
        std::cout << "[+] Usage: lmake --version\n";
        std::cout << "    Shows lmake and luavm versions\n";
        std::exit(0);
    }
    
    // Executes all the tests
    if(std::strcmp(argv[1], "--internal-test") == 0) {
        test::run();
        std::exit(0);
    } else if(std::strcmp(argv[1], "--version") == 0 || std::strcmp(argv[1], "-v") == 0) {
        std::cout << "[+] Lua version   " << LUA_VERSION_MAJOR "." LUA_VERSION_MINOR << std::endl;
        std::cout << "[+] LMake version " LMAKE_VERSION "\n";
        std::exit(0);
    }

    // Setups the settings to be passed to lmake
    lmake::settings settings;
    for(int i = 0; i < argc; i++) {
        std::string argvi(argv[i]);
        if(argvi.find("--") != std::string::npos) {
            if(std::string(argv[i]) == std::string("--recompile")) {
                settings.force_recompile = true;
            } else if(std::string(argv[i]) == std::string("--verbose")) {
                settings.verbose = true;
            } else if(std::string(argv[i]) == std::string("--debug")) {
                settings.verbose = true;
                settings.debug = true;
            } else {
                spdlog::error("Unknown flag passed");
                std::exit(1);
            } 
        }
    }

    // Checks if configuration file exists
    if(!os::file_exists(LMAKE_CONFIG_PATH)) {
        spdlog::error("No lmake.lua file found.");
        std::exit(1);
    }


    // Initializes lmake and loads the configuration file
    lmake::initialize(settings);
    lmake::load_from_file(LMAKE_CONFIG_PATH);

    // Checks if variables need to be changed and changes 
    // variable values if needed
    for (int i = 0; i < argc; i++) {
        std::string argvi = std::string(argv[i]);
        size_t equals_index = argvi.find("=");
        if(equals_index != std::string::npos) {
            // Split the left and right parts of the equals 
            std::string left_part = argvi.substr(0, equals_index);
            std::string right_part = argvi.substr(equals_index + 1, argvi.size() - equals_index);

            // Changes the left part named variable to value right_part
            lmake::change_variable(left_part, right_part);
        }
    }
    

    // Executes the target given by the command line arguments
    // if no target is specified an error message is printed
    if(argc >= 1) {
        lmake::execute_target(argv[1]);
    } else {
        spdlog::error("No target specified.");
        std::exit(1);
    }
    
}