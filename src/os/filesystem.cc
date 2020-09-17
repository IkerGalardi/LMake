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

namespace os {
    bool file_exists(const char* path) {
        FILE *file;
        if ((file = fopen(path, "r"))) {
            fclose(file);
            return true;
        }
        return false;
    }

    std::shared_ptr<char> read_file(const char* path) {
        FILE* file_path = std::fopen(path, "r");
        std::fseek(file_path, 0, SEEK_END);
        int length = ftell(file_path);

        std::shared_ptr<char> buffer(new char[length + 1], std::default_delete<char[]>());
        std::fread(buffer.get(), 1, length, file_path);
        std::fclose(file_path);

        buffer.get()[length] = '\0';

        return buffer;
    }
}