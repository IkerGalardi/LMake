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

    void set_settings(const settings& settings);

    void compatibility_version(float compatibility_version);

    void set_compiler(const std::string& compiler);

    void set_compiler_flags(const std::string& flags);

    void set_compiler_out(const std::string& out_regex);

    void compile(const std::string& source_files);

    void set_linker(const std::string& linker);

    void set_linker_flags(const std::string& flags);

    void set_linker_out(const std::string& out_regex);

    void link(const std::string& object_files);

    void chdir(const std::string& dir);

    void last_dir();

    int exec(const std::string& command);

    void error(const std::string& msg);

    char* find(const std::string& regex);

    std::string find_recursive(const std::string& regex);
} }