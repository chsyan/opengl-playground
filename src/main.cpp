#define GLFW_INCLUDE_NONE // Ensure there will be no header conflicts between glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <fmt/core.h>

#include "render/renderer.h"

int main(int argc, char *argv[])
{
    if (!Renderer::init())
    {
        fmt::print("Failed to initialize renderer");
        exit(EXIT_FAILURE);
    }

    GLFWwindow *window = Renderer::get_window();

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        Renderer::render_all();
    }

    // Terminate
    Renderer::terminate();
    exit(EXIT_SUCCESS);
}