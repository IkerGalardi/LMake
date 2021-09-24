project "own_spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/")
    objdir("bin/spdlog_obj/")

    files {"sources/spdlog/src/**.cpp", "sources/spdlog/include/**.hh"}
    includedirs {"lib/sources/spdlog/include/"}

    defines { "SPDLOG_COMPILED_LIB" }

    filter "configurations:Debug"
        symbols "on"
        optimize "off"
    filter "configurations:Release"
        optimize "on"
        symbols "off"
        runtime "Release"

project "own_lua"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir("bin/")
    objdir("bin/lua_obj/")

    files {"sources/lua/**.c", "sources/lua/**.h"}
    includedirs {"lib/sources/lua/"}

    filter "configurations:Debug"
        symbols "on"
        optimize "off"
    filter "configurations:Release"
        optimize "on"
        symbols "off"
        runtime "Release"