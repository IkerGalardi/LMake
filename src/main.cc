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

#include <iostream>
#include <cstring>

#include <lua/lua.hpp>

#include "os/filesystem.hh"
#include "lmake.hh"
#include "test/test.hh"
#include "debug.hh"

#define LMAKE_CONFIG_PATH "./lmake.lua"

int main(int argc, char** argv) {
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

    if(!os::file_exists(LMAKE_CONFIG_PATH)) {
        ERROR("No lmake.lua file found.");
        std::exit(1);
    }

    lmake::settings settings;
    for(int i = 0; i < argc; i++) {
        if(std::string(argv[i]) == std::string("--recompile")) {
            settings.force_recompile = true;
        } else if(std::string(argv[i]) == std::string("--verbose")) {
            settings.verbose = true;
        } else if(std::string(argv[i]) == std::string("--debug")) {
            settings.verbose = true;
            settings.debug = true;
        } 
    }

    lmake::initialize(settings);
    lmake::load_from_file(LMAKE_CONFIG_PATH);

    for (int i = 0; i < argc; i++) {
        std::string argvi = std::string(argv[i]);
        size_t equals_index = argvi.find("=");
        if(equals_index != std::string::npos) {
            std::string left_part = argvi.substr(0, equals_index);
            std::string right_part = argvi.substr(equals_index + 1, argvi.size() - equals_index);

            lmake::change_variable(left_part, right_part);
        }
    }
    

    if(argc >= 1) {
        lmake::execute_target(argv[1]);
    } else {
        ERROR("No target specified.");
        std::exit(1);
    }
    
}