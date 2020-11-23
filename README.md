# LMake
First of all, this project is currently on **heavy development**. As such, this software is not meant to be runned on production. The first stable release is planned for the end of the year. If you are using this software, all bug reports and suggestions are welcome!

LuaMake or LMake is a statement based low level build system aimed to provide flexibility to modify all parts of how your binary is built. LMake is not a project generator, instead, invokes the compiler/linker when necesary.

LMake takes low level seriously, this means that you are able to modify the build commands down to the flags passed to the compiler or linker. This gives you the flexibility needed to, for example, create the build system to compile and link an entire operating system. The only thing LMake takes care off is not to recompile files that haven't been edited.

## Example lmake.lua file
```lua
lmake_compatibility_version(1)

CC = "/bin/gcc"
LD = "/bin/gcc"
OUT_DIR = "build/"

function build()
    lmake_set_compiler(CC)
    lmake_set_compiler_flags("-O3")
    lmake_set_compiler_out(OUT_DIR .. "%.o")
    lmake_compile("src/main.c src/print.c")

    lmake_set_linker(LD)
    lmake_set_linker_flags("")
    lmake_set_linker_out(OUT_DIR .. "app")
    lmake_link("build/main.c.o build/print.c.o")
end
```
## How to contribute
You can contribute to the project in two ways: writing issues and contribute code. As a lmake user, if you want to report a bug or ask for a feature writing issues is the most efficient thing to do. Else, if you want to contribute code, you can create a pull request. For more information check the [contributing](docs/Contributing.md) document.