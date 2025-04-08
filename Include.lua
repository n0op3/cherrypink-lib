project "TinyChernoLib"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
    defines { "SPDLOG_COMPILED_LIB" }

    newoption {
        trigger = "x11",
        description = "Enable X11 support"
    }

    filter "system:linux"
        filter { "options:x11" }
            defines { "_GLFW_X11" }
        filter {}
    filter {}

    files {
        "src/**",
        "lib/glfw-3.4/src/**",
        "lib/glad/src/**",
        "lib/spdlog/src/**"
    }
    includedirs {
        "lib/glfw-3.4/include",
        "lib/glad/include",
        "lib/spdlog/include"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"

    filter "toolset:clang or gcc"
        buildoptions { "-Wall", "-Werror" }
