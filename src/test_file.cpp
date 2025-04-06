#include "test_file.hpp"
#include "glad/glad.h"

#include "GLFW/glfw3.h"

void tiny_cherno::init() {
    glfwInit();
    gladLoadGL();
}
