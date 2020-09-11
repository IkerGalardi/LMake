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

lmake* lmake::instance = nullptr;

lmake* lmake::get() {
    if(!instance) 
        instance = new lmake();

    return instance;
}

bool lmake::build(const std::string config_path) {
    // Reads the configuration file from the filesystem
    FILE* config_file = fopen(config_path.c_str(), "r");
    fseek(config_file, 0, SEEK_END);
    int length = ftell(config_file);
    char* config = static_cast<char*>(std::malloc(length + 1));
    fread(config, 1, length, config_file);
    config[length] = '\0';

    // TODO: setup native methods for the luavm
    // TODO: run the script

    std::free(config);

    return true;
}