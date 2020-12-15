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

#include "lmake.hh"

namespace lmake { namespace func {

    /*
     * Sets up settings for internal use on all native
     * functions.
     * 
     * @param settings: new settings
     */
    void set_settings(const settings& settings);

    /*
     * Given the version like a float (major and minor version) 
     * this function checks if the given version is compatible
     * with the currently instaled one, if not just exit.
     * 
     * @param compatibility_version: version major.minor format
     */
    void compatibility_version(float compatibility_version);

    /*
     * Sets the compiler specified on the context
     * 
     * @param compiler: path to the compiler (full path only) 
     */
    void set_compiler(const std::string& compiler);

    /*
     * Sets the compiler flags specified on the context
     * 
     * @param flags: flags that are going to be passed to
     *               the compiler
     */
    void set_compiler_flags(const std::string& flags);

    /*
     * Sets the compiler output regex on the context. 
     * 
     * @param out_regex: regex that describes how the output
     *                   of the compiler should be (% substituted
     *                   by the name of the source file)
     */
    void set_compiler_out(const std::string& out_regex);

    /*
     * Compiles the specified source files using context's flags 
     * and output directory
     * 
     * @param source_files: source files paths separated by spaces
     */
    void compile(const std::string& source_files);

    /*
     * Sets the linker on the context.
     * 
     * @param linker: absolute path to the linker
     */
    void set_linker(const std::string& linker);

    /*
     * Sets the flags to be used when invoking the linker on the
     * context. 
     * 
     * @param flags: flags used when invoking the linker
     */
    void set_linker_flags(const std::string& flags);

    /*
     * Sets the linker output regex on the context.
     * 
     * @param out_regex: path to the output binary of the linker
     */
    void set_linker_out(const std::string& out_regex);

    /*
     * Links the specified object file into the finel executable
     * using context flags and output path
     * 
     * @param object_files: 
     */
    void link(const std::string& object_files);

    /*
     * Changes the working directory and updates the been dirs.
     * 
     * @param dir: directory to be changed to
     */
    void chdir(const std::string& dir);

    /*
     * Changes the working directory to the one before the current.
     */
    void last_dir();

    /*
     * Executes the another process with the given path and command
     * line arguments.
     * 
     * @param command: bash like command with path and arguments 
     *                 separated by spaces
     */
    int exec(const std::string& command);

    /*
     * Prints the given message with the "[E]" tag and exits
     * execution of the application.
     * 
     * @param msg: message to print
     */
    void error(const std::string& msg);

    /*
     * Finds all files in the given path including the regex. Regex
     * function is similar to bash's one (* = all files in given 
     * directory). 
     * 
     * @param regex: bash like regex 
     */
    std::string find(const std::string& regex);

    /*
     * Similar to find, but also searches in sub directories when 
     * "**" is used instead of single astrisc.
     * 
     * @param regex: bash like
     */
    std::string find_recursive(const std::string& regex);
} }