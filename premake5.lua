workspace "lmake"
    architecture "x86_64"
    configurations {"Debug", "Release"}
    flags {"MultiProcessorCompile"}

project "lmake"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("build/")
    objdir("build/obj/")

    files {"src/**.cc", "src/**.hh"}
    includedirs {"lib/include"}

    libdirs {"lib/bin"}
    links {"lua53"}

    filter "system:linux"
        defines { "LMAKE_OS_LINUX" }