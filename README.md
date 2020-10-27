# LMake
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
You can contribute to the project in two ways: writing issues and contribute code. As a lmake user, if you want to report a bug or ask for a feature writing issues is the most efficient thing to do. Else, if you want to contribute code, you can create a pull request. For me to take the issues and pull requests seriously certain things need to done:

### Writing issues
Issues are the most simple thing to do if you want to contribute, as almost no prerequisites are needed. The only thing that is needed is to add a tag. As a user the only two tags that are available to you are `enhancement` and `bug`. These tags are self explanatory. 

In case of writing a bug related issue, as much information about the expected behaviour and script are needed to ease the development of a patch. 

In case of enhancements, details of implementation aren't needed, just an idea of what needs to be implemented and about how it would be implemented on the interface (flags, lmake script commands, etc...). Interface implementations are going to be discussed with the maintainer for the interface consistency reasons.

### Contributing code
For contributing code is recommended to have an issue with the tag `needs-impl`. This is made so that user level consistency can be maintained easily. It is possible to create a pull request without a previous issue, but if the maintainer sees the feature is not needed or breaks user level experience the pull request can be closed.

When trying to fix a bug you encountered as a user, it is recommended to at least create an issue so that all the bugs are tracked.
