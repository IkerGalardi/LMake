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

#include <iostream>

#include "os/filesystem.hh"

lmake* lmake::instance = nullptr;

lmake* lmake::get() {
    if(!instance) 
        instance = new lmake();

    return instance;
}

bool lmake::build(const std::string config_path) {
    auto buffer = os::read_file(config_path.c_str());

    // TODO: setup native methods for the luavm
    
    if(!vm.execute_script(buffer.get())) {
        std::string err = vm.get_last_error();
        std::cout << "[E] Lua error\n";
        std::cout << "\t" << err << std::endl;

        return false;
    }

    return true;
}