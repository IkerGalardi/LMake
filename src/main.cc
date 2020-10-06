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

#define LMAKE_CONFIG_PATH "./LMakefile"

void print_usage(const char* reason) {
    std::cout << "[+] " << reason << std::endl;
    std::cout << "\t build: builds using the LMakefile file on the directory\n";
    std::cout << "\t clean: cleans all intermediate files\n";
    std::cout << "\t help: prints this helpfull message\n";
}

int main(int argc, char** argv) {

    if(argc <= 1 || std::strcmp(argv[1], "--help") == 0) {
        print_usage("No arguments specified, you can specify one of the next:");
        std::exit(0);
    }
    
    // Executes all the tests
    if(std::strcmp(argv[1], "--test") == 0) {
        if(std::strcmp(argv[2], "all") == 0) {
            std::cout << "[+] Starting tests...\n";
            test::run();
            std::exit(0);
        } else if(std::strcmp(argv[2], "lmake") == 0) {
            std::cout << "[+] Starting lmake test...\n";
            test::lmake();
            std::exit(0);
        }
    } else if(std::strcmp(argv[1], "--version") == 0 || std::strcmp(argv[1], "-v") == 0) {
        std::cout << "[+] Lua version   " << LUA_VERSION_MAJOR "." LUA_VERSION_MINOR << std::endl;
        std::cout << "[+] LMake version " LMAKE_VERSION "\n";
        std::exit(0);
    }

    if(!os::file_exists(LMAKE_CONFIG_PATH)) {
        std::cerr << "[E] No LMakefile file found.\n";
        std::exit(1);
    }

    lmake::initialize();

    if(!lmake::load_from_file(LMAKE_CONFIG_PATH)) {
        std::cerr << "[E] " << lmake::get_last_error() << std::endl;
        std::exit(3);
    }
    
}