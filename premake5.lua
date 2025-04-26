workspace "CherryPink"
    configurations { "Debug", "Release" }
    location "build"

    include "Include.lua"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"
