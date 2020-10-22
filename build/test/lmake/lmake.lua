lmake_compatibility_version(1)

function build()
    lmake_set_compiler("/bin/gcc")
    lmake_set_compiler_flags("-O3")
    lmake_set_compiler_out("build/%.o")
    lmake_compile("src/main.c src/print.c")

    lmake_set_linker("/bin/gcc")
    lmake_set_linker_flags("")
    lmake_set_linker_out("build/app")
    lmake_link("build/main.c.o build/print.c.o")
end
