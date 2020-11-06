#pragma once

#include "lmake.hh"

namespace lmake { namespace func {

    void set_settings(const settings& settings);

    void compatibility_version(float compatibility_version);

    void set_compiler(const std::string& compiler);

    void set_compiler_flags(const std::string& flags);

    void set_compiler_out(const std::string& out_regex);

    void compile(const std::string& source_files);

    void set_linker(const std::string& linker);

    void set_linker_flags(const std::string& flags);

    void set_linker_out(const std::string& out_regex);

    void link(const std::string& object_files);

    void chdir(const std::string& dir);

    void last_dir();

    int exec(const std::string command);

    void error(const std::string msg);

} }