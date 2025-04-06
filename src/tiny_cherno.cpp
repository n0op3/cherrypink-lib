#include "tiny_cherno.hpp"
#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

void tiny_cherno::init() {
    spdlog::info("Loading the TinyCherno runtime!");
    glfwInit();
    gladLoadGL();
}
