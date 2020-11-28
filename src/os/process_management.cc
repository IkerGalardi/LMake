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
#include <cstring>
#include <sstream>

#include <stringtoolbox/stringtoolbox.hh>

#include "filesystem.hh"

static std::vector<char*> string_split_null_terminated(const std::string& str, char delimeter) {
    std::vector<char*> res;
    std::string temp;
    std::istringstream stream(str.c_str());

    while(std::getline(stream, temp, delimeter)) {
        //res.push_back(std::malloc((temp.size() + 1) * sizeof(char)));
        void* mem = std::calloc(temp.size() + 1, sizeof(char));
        res.push_back(static_cast<char*>(mem));
        std::strcpy(res[res.size() - 1], temp.c_str());
    }

    res.push_back(static_cast<char*>(nullptr));

    return res;
}

namespace os {
    process run_process(std::string prog, std::string args) {
        pid_t pid = fork();

        if(!os::file_exists(prog)) {
            std::cerr << "[E] Process "  << prog << " doesnt exist.\n";
            std::exit(1);
        }

        if(pid == 0) { // child process
            std::string temp = std::string(prog) + " " + args;
            auto args = string_split_null_terminated(temp, ' ');

            int err;
            if(prog.find("/") != std::string::npos) {
                err = execv(prog.c_str(), args.data());
            } else {
                err = execvp(prog.c_str(), args.data());
            }

            std::cerr << "[E] Cannot execute process\n";
            std::exit(5);
        }
        return pid;
    }

    int wait_process(process proc) {
        int status;
        int exited; 

        do {
            waitpid(proc, &status, WUNTRACED);
            exited = WIFEXITED(status);
        } while(!exited);

        return WEXITSTATUS(status);
    }
}