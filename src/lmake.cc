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

#include "luavm.hh"
#include "os/filesystem.hh"

static struct {
    luavm vm;

    struct {
        std::string compiler;
        std::string compiler_flags;

        std::string linker;
        std::string linker_flags;
    } context;

    bool initialized = false;

    std::string last_error;
} lmake_data;

std::string process_script(const char* file_contents, const char* containing_dir) {
    /// TODO: preprocess all the lmake_include (mimic #include of c)    
    return std::string(file_contents);
}

namespace lmake {
    void initialize() {
        /// TODO: add native functions
        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler = std::string(lua_tostring(vm, -1));
            return 1;
        }, "lmake_set_compiler");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            lmake_data.context.compiler_flags = std::string(lua_tostring(vm, -1));
            return 1;
        }, "lmake_set_compiler_flags");

        lmake_data.vm.add_native_function([](lua_State* vm) -> int {
            std::string name_regex = std::string(lua_tostring(vm, -1));
            std::string source_files = std::string(lua_tostring(vm, -2));

            /// TODO: name_regex: substitute % with the source name
            /// TODO: compile all the source files

            return 1;
        }, "lmake_compile");

        lmake_data.initialized = true;
    }

    bool build_from_file(const char* config_path) {
        if(!lmake_data.initialized) 
            return false;

        auto file_buffer = os::read_file(config_path);
        std::string processed = process_script(file_buffer.get(), config_path);

        return lmake::build_from_string(processed.c_str());
    }

    bool build_from_string(const char* config_string) {
        if(!lmake_data.initialized) 
            return false;

        if(!lmake_data.vm.execute_script(config_string)) {
            lmake_data.last_error = lmake_data.vm.get_last_error();
            return false;
        }

        return true;
    }

    std::string& get_last_error() {
        return lmake_data.last_error;
    }
}