includedirs {
    "src",
    "lib/glfw-3.4/include",
    "lib/glad/include",
    "lib/spdlog/include",
    "lib/glm",
    "lib/assimp/include",
    "lib/stb/include",
    "build/assimp/include",
}

filter "system:windows" -- I have no clue why Visual Studio won't compile without this, frick you, Microsoft
        buildoptions { "/utf-8" }
        defines { "_UNICODE", "UNICODE" }

project "glfw3"
    kind "StaticLib"
    language "C"

project "CherryPink"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
    defines { "SPDLOG_COMPILED_LIB" }

    -- We need to compile these manually, as they are not Premake projects
    local glfwDir = path.join(_SCRIPT_DIR, "lib", "glfw-3.4")
    local assimpDir = path.join(_SCRIPT_DIR, "lib", "assimp")

    prebuildcommands {
        "cmake -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -S " .. glfwDir .. " glfw",
        "cmake --build glfw --config %{cfg.buildcfg}",
        "cmake -DBUILD_SHARED_LIBS=OFF -DASSIMP_NO_EXPORT=ON -DASSIMP_INSTALL=OFF -DASSIMP_BUILD_TESTS=OFF -DASSIMP_BUILD_ZLIB=ON -DASSIMP_BUILD_MINIZIP=ON -S " .. assimpDir  .. " assimp",
        "cmake --build assimp --config %{cfg.buildcfg}",
    }

    filter "system:windows"
        prebuildcommands {
            "{COPYFILE} glfw/src/%{cfg.buildcfg}/*glfw* bin/%{cfg.buildcfg}",
            "{COPYFILE} assimp/lib/%{cfg.buildcfg}/*assimp* bin/%{cfg.buildcfg}",
            "{COPYFILE} assimp/contrib/zlib/%{cfg.buildcfg}/*zlibstatic* bin/%{cfg.buildcfg}",
        }
    filter {}

    filter "system:not windows"
        prebuildcommands {
            "{COPYFILE} glfw/src/*glfw3* bin/%{cfg.buildcfg}",
            "{COPYFILE} assimp/lib/*assimp* bin/%{cfg.buildcfg}",
            "{COPYFILE} assimp/contrib/zlib/*zlibstatic* bin/%{cfg.buildcfg}",
        }
    filter {}

    files {
        "src/**",
        "lib/glad/src/**",
        "lib/spdlog/src/**",
        "lib/glm/glm"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "on"

    filter "toolset:clang or gcc"
        buildoptions { "-Wall", "-Werror" }
