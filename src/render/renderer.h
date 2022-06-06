#ifndef RENDERER_H
#define RENDERER_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Renderer
{
public:
    static GLFWwindow *window;
    static int init();
    static GLFWwindow *get_window();
    static void render_all();
    static void terminate();

private:
    Renderer() {}
    static void render();
};

#endif