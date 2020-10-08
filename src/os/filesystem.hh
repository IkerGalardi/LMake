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

#include <memory>

namespace os {

    /*
     * Check if the file exists on the filesystem
     * 
     * @param path: path to the file
     */
    bool file_exists(std::string path);

    /*
     * Reads the file and returns the buffer
     * 
     * @param path: path to the file
     */
    std::shared_ptr<char> read_file(std::string path);

    /*
     * Changes the directory of the process
     * 
     * @param path: path to the directory
     */
    bool change_dir(std::string dir);

    /*
     * Gets the working directory
     * 
     * @return: working directory path
     */
    std::string get_dir();
}