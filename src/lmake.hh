#pragma once

#include <string>

namespace lmake {    
    using build_result = int;
    using parse_result = int;

    /*
     * Builds the target specified by the lmake configuration
     * file.
     * 
     * @param lmake_config: path to the lmake config file
     * @return: the result from building the target
     */
    build_result build(const char* lmake_config);

    /*
     * Sets the absolute path to the compiler executable.
     * 
     * @param compiler: path to the compiler executable
     */
    void set_compiler(const char* compiler);

     /*
     * Gets the absolute path to the compiler executable.
     * 
     * @return: path to the compiler executable
     */
    std::string get_compiler();
    
    /*
     * Sets the compiler flags that are going to be used when 
     * compiling.
     * 
     * @flags: compiler flags
     */
    void set_compiler_flags(const char* flags);
    
    /*
     * Gets the compiler flags set before. Empty if none
     * where specified.
     * 
     * @return: compiler flags
     */
    std::string get_compiler_flags(const char* flags);
    
     /*
     * Sets the absolute path to the linker executable.
     * 
     * @return: path to the linker executable
     */
    void set_linker(const char* linker);
    
    /*
     * Gets the absolute path to the linker executable.
     * 
     * @return: path to the compiler executable
     */
    std::string get_linker();

    /*
     * Sets the linker flags that are going to be used when 
     * linking.
     * 
     * @param flags: linker flags
     */
    void set_linker_flags(const char* flags);
    
    /*
     * Gets the linker flags that are going to be used on the
     * linking phase.
     * 
     * @return: linker flags
     */
    std::string get_linker_flags();

}