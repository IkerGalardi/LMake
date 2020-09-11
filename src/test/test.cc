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

#include <iostream>

#include "os/filesystem.hh"
#include "os/process_management.hh"

#include "luavm.hh"

namespace test {
    void run() {
        std::cout << "* OS tests\n";
        test::os();
        std::cout << "* Ended OS tests\n* LuaVM tests\n";
        test::luavm();
        std::cout << "* Ended LuaVM tests\n";
    }

    void luavm() {
        const char* error_script = "//jañ";
        const char* correct_script = "test()";

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

    }
}