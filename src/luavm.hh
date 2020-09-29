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

#include <lua/lua.hpp>

typedef int(*luafunc)(lua_State*);

/// TODO: add documentation to functions
class luavm {
public:
    /* Constructs all the necesary things to execute lua scripts
     */
    luavm();
    
    /* Finalizes the virtual machine and cleans up heap memory
     */
    ~luavm();

    /* Adds the native function to the lua virtual machine
     * 
     * @param func: function to be executed by the virtual machine
     * @param name: the name of the function
     */
    void add_native_function(luafunc func, const char* name);

    /* Executes the script on the lua virtual machine
     * 
     * @param script: the script to be executed
     */
    bool execute_script(const char* script);

    /// TODO: errors as enums instead of strings
    /* Returns the last error given by the virtual machine
     * 
     * @return: error description
     */
    const std::string& get_last_error();
private:
    lua_State* vm;
    std::string last_error;
protected:
};