project "TinyChernoLib"
    kind "StaticLib"
    language "C++"
    targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
    defines { "SPDLOG_COMPILED_LIB" }

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
