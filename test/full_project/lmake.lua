lmake_compatibility_version(1)

function build()
    -- Compile the source files
    src_files = lmake_find("src/**.c")
    lmake_set_compiler("/bin/gcc")
    lmake_set_compiler_flags("-O2 -ggdb")
    lmake_set_compiler_out("build/%.o")
    lmake_compile(src_files)

    -- Link the object files
    obj_files = lmake_find("build/*.o")
    lmake_set_linker("/bin/gcc")
    lmake_set_linker_flags("")
    lmake_set_linker_out("build/app")
    lmake_link(obj_files)
end

function clean()
    obj_files = lmake_find("build/*.o")
    lmake_exec("rm " .. obj_files)
end

function exec()
    lmake_exec("build/app")
end