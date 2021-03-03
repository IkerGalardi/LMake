# LMake
First of all, this project is currently on **heavy development**. If you are using this software, all bug reports and suggestions are welcome!

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
## Getting started
For compiling and installing lmake just clone the repository and compile it using make. You can do that with the next commands
```bash
git clone https://github.com/IkerGalardi/LMake
cd LMake
```
After that, if you want to install just execute the command `sudo ./install.sh`, else if you want to simply compile by doing `make`.

## How to contribute
You can contribute to the project in two ways: writing issues and contribute code. As a lmake user, if you want to report a bug or ask for a feature writing issues is the most efficient thing to do. Else, if you want to contribute code, you can create a pull request. For more information check the [contributing](docs/Contributing.md) document.

## Versioning scheme
This project uses the semver versioning system, to learn more about the versioning check [this](docs/Versions.md) document.
