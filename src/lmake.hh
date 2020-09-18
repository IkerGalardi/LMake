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
#include <vector>

#include "luavm.hh"

/// TODO: convert this into a namespace with separated functions
/// TODO: add documentation
class lmake { 
public:
    lmake(const lmake&) = delete;

    static lmake* get();

    bool build(const std::string config_path);
    inline std::string& get_last_error() { return last_error; }
private:
    /// TODO: implement constructor (add native functions)
    lmake() = default;
    ~lmake() = default;

    static lmake* instance;
    
    luavm vm;

    struct {
        std::string compiler;
        std::string compiler_flags;
    } context;

    /// TODO: implement an error enum instead of strings
    std::string last_error;

    friend void lmake_set_compiler(const char* comp);
    friend void lmake_set_compiler_flags(const char* flags);
    friend bool lmake_compile(const char* res_name, const char* obj_files);
protected:
};