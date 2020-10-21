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

#include "test.hh"

#include <unistd.h>
#include <wait.h>

#include <iostream>
#include <vector>

#include "os/filesystem.hh"
#include "os/process_management.hh"
#include "luavm.hh"
#include "lmake.hh"

#define RUN_TEST(name, func) std::cout << "* " << name << std::endl; func(); std::cout << "* Ended " << name << std::endl

namespace test {
    void run() {
        RUN_TEST("OS", test::os);
        RUN_TEST("LuaVM", test::luavm);
        RUN_TEST("Compilation", test::compilation);
    }

    void luavm() {
        std::string error_script = "//jañ";
        std::string correct_script = "test()";

        std::cout << "Adding native function\n";
        ::luavm vm;
        vm.add_native_function([](lua_State* vm) {
            std::cout << "Test string\n";
            return 1;
        }, "test");

        std::cout << "Executing script\n";
        if(!vm.execute_script(correct_script)) {
            std::cout << "Error while executing script\n";
            std::cout << vm.get_last_error() << std::endl;
        }
    }

    void os() {
        std::cout << "Running return_0\n";
        os::process p = os::run_process("./build/test/os/return_0", "aaa");
        int exit_code = os::wait_process(p);
        std::cout << "Exit code: " << exit_code << std::endl;
    
        std::cout << "Running return_1\n";
        p = os::run_process("./build/test/os/return_1", "bbb");
        exit_code = os::wait_process(p);
        std::cout << "Exit code: " << exit_code << std::endl;

        std::cout << "Running args\n";
        p = os::run_process("./build/test/os/args", "arg1 arg2 arg3");
        exit_code = os::wait_process(p);
        std::cout << "Exit code: " << exit_code << std::endl;

        std::cout << "Reading install.sh...\n";
        auto file = os::read_file("install.sh"); 
        std::cout << file << std::endl;
    }

    void compilation() {
        os::process p = os::run_process("/bin/gcc", "-c ./build/test/os/correct.c -o ./build/test/os/correct.o");
        std::cout << "Exit code: " << os::wait_process(p) << std::endl;
    }
}