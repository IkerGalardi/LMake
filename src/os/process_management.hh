#pragma once 

#include <string>

namespace os {
#ifdef LMAKE_OS_LINUX
    using process = unsigned int;
#else
    #error no recognizable os
#endif

    process run_process(const std::string& prog, const std::string& args)
    void wait_process(process proc);    
}