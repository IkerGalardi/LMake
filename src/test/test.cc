#include "test.hh"

#include <iostream>

#include "os/filesystem.hh"
#include "os/process_management.hh"

#include "luavm.hh"

namespace test {
    void run() {
        test::os();
        test::luavm();
    }

    void luavm() {
        std::cout << "Normal execution test\n";

        ::luavm vm;
        vm.execute_script("build/test/test_script.lua");
    }

    void os() {

    }
}