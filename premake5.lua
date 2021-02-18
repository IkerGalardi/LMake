workspace "lmake"
    architecture "x86_64"
    configurations {"Debug", "Release"}
    flags {"MultiProcessorCompile"}

project "lmake"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    pchheader "pch.hh"
	pchsource "pch.cc"

    targetdir("build/")
    objdir("build/obj/")

    files {"src/**.cc", "src/**.hh"}
    includedirs {"lib/include", "src"}

    libdirs {"lib/bin"}
    links {"lua53", "dl", "spdlog"}

    filter "system:linux"
        defines { "LMAKE_OS_LINUX" }