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

#include <stringtoolbox/stringtoolbox.hh>

namespace os {
    process run_process(const char* prog, const char* args) {
        pid_t pid = fork();
        auto arguments_vector = stringtoolbox::split(std::string(args), ' ');
        
        // Transform the std::vector of std::strings to a std::vector of char*
        std::vector<char*> arguments;
        arguments.reserve(arguments_vector.size());
        for(int i = 0; i < arguments_vector.size(); i++) {
            size_t str_size = arguments_vector[i].size();
            arguments[i] = (char*)std::calloc(str_size, sizeof(char));
            std::strcpy(arguments[i], arguments_vector[i].c_str());
        }
        arguments.emplace_back(nullptr);

        if(pid == 0) { // child process
            int err = execvp(prog, arguments.data());
            std::cerr << "[E] Cannot execute process\n";
            std::cout << errno << " " << err << std::endl;
            std::exit(5);
        }
        return static_cast<process>(pid);
    }

    int wait_process(process proc) {
        int exit_code = -1;
        
        int status;
        pid_t pid = static_cast<pid_t>(proc);
        waitpid(pid, &status, 0);
        if(WIFEXITED(status)) 
            return WEXITSTATUS(status);
        
        std::cerr << "[E] No exit code\n";
        std::exit(0);
        return 1215752192;
    }
}