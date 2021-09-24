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
    includedirs {"lib/sources/", "lib/sources/spdlog/include/", "src"}

    libdirs {"lib/bin"}
    links {"lua53", "dl", "spdlog", "pthread"}

    defines { "SPDLOG_COMPILED_LIB" }

    filter "configurations:Debug"
        symbols "on"
        optimize "off"
    filter "configurations:Release"
        optimize "on"
        symbols "off"
        runtime "Release"