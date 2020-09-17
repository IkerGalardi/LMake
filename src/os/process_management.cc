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

#include "process_management.hh"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include <stringtoolbox/stringtoolbox.hh>

namespace os {
    process run_process(const char* prog, const char* args) {
        pid_t pid = fork();
        auto arguments_vector = stringtoolbox::split(std::string(args), ' ');
        if(pid == 0) { // child process
            execv(prog, reinterpret_cast<char* const*>(arguments_vector.data()));
        }
        return pid;
    }

    int wait_process(process proc) {
        int exit_code = -1;
        
        int status;
        pid_t pid = static_cast<pid_t>(proc);
        waitpid(pid, &status, 0);
        if(WIFEXITED(status)) 
            return WEXITSTATUS(status);
        
        std::cerr << "[E] No exit code\n";
        return 1215752192;
    }
}