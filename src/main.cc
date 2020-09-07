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

#include <lua.hpp>

#include "os/filesystem.hh"
#include "lmake.hh"
#include "test/test.hh"

void print_usage(const char* reason) {
    std::cout << "[+] " << reason << std::endl;
    std::cout << "\t build: builds using the LMakefile file on the directory\n";
    std::cout << "\t clean: cleans all intermediate files\n";
    std::cout << "\t help: prints this helpfull message\n";
}

int main(int argc, char** argv) {
    std::cout << "[+] Using " << LUA_VERSION << std::endl;

    if(argc <= 1 || std::strcmp(argv[1], "help") == 0) {
        print_usage("No arguments specified, you can specify one of the next:");
        std::exit(0);
    }

    if(std::strcmp(argv[1], "--run-tests") == 0) {
        test::luavm();
    }

    if(!os::file_exists("./LMakefile")) {
        std::cout << "[E] No LMakefile file found.\n";
        std::exit(1);
    }

    lmake::get()->build("./LMakefile");
}