libdirs {
    "lib/glfw-3.4/build/src"
}

links { "glfw3" }

filter "system:windows" -- I have no clue why Visual Studio won't compile without this, frick you, Microsoft
        buildoptions { "/utf-8" }
        defines { "_UNICODE", "UNICODE" }

project "GLFW"
    kind "StaticLib"
    language "C"
    local glfwDir = path.join(_SCRIPT_DIR, "lib", "glfw-3.4")

    -- We need to compile GLFW manually, as it is not a Premake project
    prebuildcommands {
         "{CHDIR} " .. glfwDir,
         "cmake -S . -B build -A x64 && cmake --build build --config %{cfg.buildcfg}"
    }

project "TinyChernoLib"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
    defines { "SPDLOG_COMPILED_LIB" }

    dependson { "GLFW" }

    files {
        "src/**",
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
