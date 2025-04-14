local glfwDir = path.join(_SCRIPT_DIR, "lib", "glfw-3.4")

filter "system:windows" -- I have no clue why Visual Studio won't compile without this, frick you, Microsoft
        buildoptions { "/utf-8" }
        defines { "_UNICODE", "UNICODE" }

project "glfw3"
    kind "StaticLib"
    language "C"

project "TinyChernoLib"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
    defines { "SPDLOG_COMPILED_LIB" }

    -- We need to compile GLFW manually, as it is not a Premake project
    prebuildcommands {
        "cmake -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -S " .. glfwDir .. " .",
        "cmake --build . --config %{cfg.buildcfg}",
    }

    filter "system:windows"
        prebuildcommands {
            "{COPYFILE} src/%{cfg.buildcfg}/*glfw* bin/%{cfg.buildcfg}"
        }
    filter {}

    filter "system:not windows"
        prebuildcommands {
            "{COPYFILE} src/*glfw3* bin/%{cfg.buildcfg}"
        }
    filter {}

    links { "glfw3" }

    files {
        "src/**",
        "lib/glad/src/**",
        "lib/spdlog/src/**"
    }

    includedirs {
        "src",
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
