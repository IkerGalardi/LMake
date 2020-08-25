#include "lmake.hh"

#include <string>

namespace lmake {

    struct compile_context {
        std::string compiler_path;
        std::string compiler_flags;
    };
    struct link_context {
        std::string linker_path;
        std::string linker_flags;
    };

    static compile_context current_compile_context;
    static link_context current_link_context;

    build_result build(const char* lmake_config) {

    }

    void set_compiler(const char* compiler) {
        current_compile_context.compiler_path = std::string(compiler);
    }

    std::string get_compiler() {
        return current_compile_context.compiler_path;
    }
    
    void set_compiler_flags(const char* flags) {
        current_compile_context.compiler_flags = std::string(flags);
    }
    
    std::string get_compiler_flags(const char* flags) {
        return current_compile_context.compiler_flags;
    }
    
    void set_linker(const char* linker) {
        current_link_context.linker_path = std::string(linker);
    }
    
    std::string get_linker() {
        return current_link_context.linker_path;
    }

    void set_linker_flags(const char* flags) {
        current_link_context.linker_flags = std::string(flags);
    }
    
    std::string get_linker_flags() {
        return current_link_context.linker_flags;
    }
}