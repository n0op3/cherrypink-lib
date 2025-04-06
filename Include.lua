project "TinyChernoLib"
    kind "StaticLib"
    language "C++"
    targetdir "bin"

    files { "src/**", "lib/glfw-3.4/src/**", "lib/glad/src/**" }
    includedirs { "lib/glfw-3.4/include", "lib/glad/include" }
