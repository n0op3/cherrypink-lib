workspace "TinyCherno"
    configurations { "Debug", "Releas" }
    location "build"

    include "Include.lua"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
