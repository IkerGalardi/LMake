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

#pragma once 

#include <string>

#define LMAKE_VERSION "1.0.0 DEV"
#define LMAKE_COMPAT_VERSION 1.0

namespace lmake {
    struct settings {
        bool force_recompile = false;
        bool verbose = false;
    };

    /*
     * Initializes de luavm and adds all the necesary native functions.
     * 
     * @param settings: settings that lmake will use
     */
    void initialize(const settings& settings);

    /*
     * Loads the file from the filesystem system, preprocesses it (replaces 
     * all the lmake_include...), and executes it.
     * 
     * @param config_path: path to the configuration file
     * @return: true if success, false if something went wrong
     */
    void load_from_file(std::string config_path);

    /*
     * Executes the provided string without any processing.
     * 
     * @param config_path: path to the configuration file
     * @return: true if success, false if something went wrong
     */
    void load_from_string(std::string config_string);

    /*
     * Executes the specified target function
     * 
     * @param target: name of the target
     * @return: true if success, false if something went wrong
     */
    void execute_target(std::string target);
}