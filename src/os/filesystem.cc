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
#include <filesystem>

namespace os {
    bool file_exists(const std::string& path) {
        FILE *file;
        if ((file = fopen(path.c_str(), "r"))) {
            fclose(file);
            return true;
        }
        return false;
    }

    std::shared_ptr<char> read_file(const std::string& path) {
        FILE* file_path = std::fopen(path.c_str(), "r");

        std::fseek(file_path, 0, SEEK_END);
        int length = std::ftell(file_path);
        std::fseek(file_path, 0, SEEK_SET);
        
        std::shared_ptr<char> buffer(new char[length + 1], std::default_delete<char[]>());
        std::fread(buffer.get(), length, 1, file_path);
        std::fclose(file_path);

        buffer.get()[length + 1] = '\0';

        return buffer;
    }

    bool change_dir(const std::string& dir) {
         return chdir(dir.c_str()) == 0;
    }

    std::string get_dir() {
        char* tmp = getcwd(NULL, 0);
        std::string res(tmp);
        std::free(tmp);
        return res;
    }

    bool compare_file_dates(const std::string& file_a, const std::string& file_b) {
        auto edited_a = std::filesystem::last_write_time(file_a);
        auto edited_b = std::filesystem::last_write_time(file_b);

        return edited_a < edited_b;
    }

}