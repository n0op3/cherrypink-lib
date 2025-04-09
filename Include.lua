libdirs {
    "lib/glfw-3.4/build/src"
}

links { "glfw3" }

project "GLFW"
    kind "StaticLib"
    language "C"
    local glfwDir = path.join(_SCRIPT_DIR, "lib", "glfw-3.4")

    prebuildcommands {
        "{CHDIR} " .. glfwDir .. "; {MKDIR} build; {CHDIR} build; cmake .. && make"
    }

project "TinyChernoLib"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
    defines { "SPDLOG_COMPILED_LIB" }

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
