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

namespace os {
    process run_process(const std::string& prog, const std::string& args) {
        pid_t pid = fork();
        if(pid == 0) // child process
            execl(prog.c_str(), args.c_str());

        return pid;
    }

    int wait_process(process proc) {
        int exit_code = -1;
        while(true) {
            int status;
            pid_t pid = static_cast<pid_t>(proc);
            waitpid(pid, &status, 0);

            if(WIFEXITED(status)) {
                exit_code = WEXITSTATUS(status);
                break;
            }
        }

        return exit_code;
    }
}