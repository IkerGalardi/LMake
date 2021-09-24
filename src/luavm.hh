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
#include <memory>

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

/* 
 * Function signature of native functions
 */
typedef int(*luafunc)(lua_State*);

class luavm {
public:
    /* 
     * Constructs all the necesary things to execute lua scripts
     */
    luavm();
    
    /* 
     * Finalizes the virtual machine and cleans up heap memory
     */
    ~luavm();

    /* 
     * Adds the native function to the lua virtual machine
     * 
     * @param func: function to be executed by the virtual machine
     * @param name: the name of the function
     */
    void add_native_function(luafunc func, std::string name);

    /* 
     * Executes the script on the lua virtual machine
     * 
     * @param script: the script to be executed
     */
    bool execute_script(std::string script);

    /* 
     * Checks if the function exists
     * 
     * @param fn_name: name of the functions
     * @return: true if the function exists, false if not
     */
    bool function_exists(std::string fn_name);
    
    /* 
     * Executes the script on the lua virtual machine
     * 
     * @param fn_name: name of the function to be executed
     */
    void execute_function(std::string fn_name);

    /*
     * Searches variable 'name' and changes the value on the 
     * lua stack to 'value'.
     * 
     * @param name: name of the lua variable
     * @param value: new value of variable
     */
    void change_variable(const std::string& name, const std::string& value);

    /// TODO: errors as enums instead of strings
    /* 
     * Returns the last error given by the virtual machine
     * 
     * @return: error description
     */
    std::string get_last_error();
private:
    lua_State* vm;
    std::string last_error;
    std::string script;
protected:
};