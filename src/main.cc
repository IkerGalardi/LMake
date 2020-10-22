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
        std::cout << "[+] Starting tests...\n";
        test::run();
        std::exit(0);
    } else if(std::strcmp(argv[1], "--version") == 0 || std::strcmp(argv[1], "-v") == 0) {
        std::cout << "[+] Lua version   " << LUA_VERSION_MAJOR "." LUA_VERSION_MINOR << std::endl;
        std::cout << "[+] LMake version " LMAKE_VERSION "\n";
        std::exit(0);
    }

    if(!os::file_exists(LMAKE_CONFIG_PATH)) {
        std::cerr << "[E] No lmake.lua file found.\n";
        std::exit(1);
    }

    lmake::initialize();

    if(!lmake::load_from_file(LMAKE_CONFIG_PATH)) {
        std::cerr << "[E] " << lmake::get_last_error() << std::endl;
        std::exit(3);
    }

    if(argc >= 1) {
        lmake::execute_target(argv[1]);
    } else {
        std::cout << "[I] No target specified\n";
        std::exit(1);
    }
    
}