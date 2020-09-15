-- Check if all the native functions are available
lmake_compatible_version(1)

-- Literally copy pastes the lua file
lmake_include("build/config.lua")

function check()
    lmake_check("/bin/clang")
    lmake_check("/bin/ld")
end

-- Invoked by: lmake build
function build()
    -- Compiles all the files
    lmake_set_compiler("/bin/clang")
    lmake_set_compiler_flags("-O3"))
    source_files = lmake_find_files("src/**.c")
    source_files = source files + lmake_find_files("src/**.cc")
    lmake_compile("./build/%.o", source_files)

    -- Links all the object files into an ELF
    lmake_set_linker("/bin/ld")
    lmake_set_linker_flags("-lX11")
    obj_files = lmake_find_files("./build/*.o")
    lmake_link("./build/app", obj_files)
end

-- Invoked by: lmake clear
function clear()
    -- Removes the files of the regex
    lmake_remove_files("build/*.o")
end