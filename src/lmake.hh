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

#define LMAKE_VERSION "0.6 DEV"
#define LMAKE_COMPAT_VERSION 1.0

namespace lmake {
    /*
     * Initializes de luavm and adds all the necesary native functions.
     */
    void initialize();

    /*
     * Loads the file from the filesystem system, preprocesses it (replaces 
     * all the lmake_include...), and executes it.
     * 
     * @param config_path: path to the configuration file
     * @return: true if success, false if something went wrong
     */
    bool load_from_file(const char* config_path);

    /*
     * Executes the provided string without any processing.
     * 
     * @param config_path: path to the configuration file
     * @return: true if success, false if something went wrong
     */
    bool load_from_string(const char* config_string);

    /*
     * Executes the specified target function
     * 
     * @param target: name of the target
     * @return: true if success, false if something went wrong
     */
    bool execute_target(const char* target);

    /*
     * Returns the last error that occured.
     * 
     * @return: string explaining the error
     */
    std::string& get_last_error();
}