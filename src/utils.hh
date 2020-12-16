#pragma once 

#include <vector>
#include <string>

namespace utils {
    /*
     * Invokes the compiler process given the necessary flags
     * 
     * @param compiler: path to the compiler (can be inside PATH)
     * @param flags: flags to be passed to the compiler
     * @param src: relative path to the source file
     * @param out: relative path to the output file
     * 
     * @return: true if compilation succeded, false if failed
     */
    bool compile(const std::string& compiler, const std::string& flags, const std::string& src, const std::string& out);
  
    /*
     * Invokes the linker given the necessary flags
     * 
     * @param linker: path to the linker (can be inside PATH)
     * @param flags: flags to be passed to the compiler
     * @param obj: list of path to object files separated with spaces
     * @param out: path to the output linked binary
     *
     *  @return: true if linking succeded, false if failed
     */
    bool link(const std::string& linker, const std::string& flags, const std::string& obj, const std::string& out);
   
    /*
     * Splits a string by the given delimeter character
     * 
     * @param str: string to be splited
     * @param delimeter: character to split the string
     * 
     * @return: vector of the splitted strings
     */
    std::vector<std::string> string_split(const std::string& str, char delimeter);
    
    /*
     * Replaces a given string by another.
     * 
     * @param str: given string 
     * @param from: substring to be replaced
     * @param to: substring to be inserted
     * 
     * @return: str with from replaced by to
     */
    std::string string_replace(std::string str, const std::string& from, const std::string& to);

}