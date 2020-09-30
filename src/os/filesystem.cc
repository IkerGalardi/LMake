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

#include "filesystem.hh"

#include <cstdio>
#include <unistd.h>
#include <iostream>

namespace os {
    bool file_exists(const char* path) {
        FILE *file;
        if ((file = fopen(path, "r"))) {
            fclose(file);
            return true;
        }
        return false;
    }

    char* read_file(const char* path) {
        FILE* file_path = std::fopen(path, "r");
        std::fseek(file_path, 0, SEEK_END);
        int length = std::ftell(file_path);
        std::fseek(file_path, 0, SEEK_SET);
        char* buffer = new char[length + 1];
        std::fread(buffer, length, 1, file_path);
        std::fclose(file_path);

        buffer[length + 1] = '\0';

        return buffer;
    }

    bool change_dir(const char* dir) {
         return chdir(dir) == 0;
    }
}