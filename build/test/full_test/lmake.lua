-- lmake_compatibility_version("0.4")

COMPILER = "/bin/gcc"
LINKER = "/bin/gcc"

function build() {
    lmake_set_compiler("/bin/gcc")
    lmake_set_compiler_flags("-O3")
    lmake_set_compiler_output("build")
    lmake_compile("src/main.c src/print.c")

    lmake_set_linker("/bin/gcc")
    lmake_set_linker_flags("")
    lmake_set_linker_output("build/app")
    lmake_link("build/main.c.o build/print.c.o")
}
