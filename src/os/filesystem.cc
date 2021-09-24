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

#include <sys/mman.h>
#include <fcntl.h>

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <filesystem>

namespace os {
    std::shared_ptr<char> read_file(const std::string& path) {
        size_t file_size = std::filesystem::file_size(path);
        
        std::shared_ptr<char> buffer(new char[file_size + 1], std::default_delete<char[]>());

        // Map the file to address space
        int fd = open(path.c_str(), O_RDONLY);
        void* mapped_file_addr = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

        std::memcpy(buffer.get(), mapped_file_addr, file_size);

        munmap(mapped_file_addr, file_size);
        close(fd);

        return buffer;
    }

    std::string file_dir(const std::string& file) {
        std::string directory;
        
        // Gets the last "/" character and returns all the string before that
        const size_t last_slash_idx = file.rfind('/');
        if (std::string::npos != last_slash_idx) {
            directory = file.substr(0, last_slash_idx);
        }

        return directory;
    }
}