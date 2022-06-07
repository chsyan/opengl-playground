#include "renderer.h"
#include "resource_manager.h"
#include <fmt/std.h>
#include "camera.h"
#include "cube.h"
#include "sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLFWwindow *Renderer::window;
float prev_time, delta_time; // Used to calculate delta time between render frames
unsigned int cube_vao, light_vao;
Cube *cube;
Sphere *sphere;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(0.0f, 0.0f, -2.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(0.0f, -3.0f, 3.0f)};

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
glm::vec3 light_position(0.0f, 1.0f, 3.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char *description)
{
    fmt::print(stderr, "GLFW error: {}\n", description);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    {
        fmt::print("Escape key pressed\n");
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            camera.keys[key] = true;
        else if (action == GLFW_RELEASE)
            camera.keys[key] = false;
    }
}

void resize_callback(GLFWwindow *window, int width, int height) {}

int Renderer::init()
{
    // Init OpenGL library
    if (!glfwInit())
        return -1;

    // OpenGL hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disable window decorations

    // Create a window
    window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set the window as the current context
    glfwMakeContextCurrent(window);

    // Init glad
    gladLoadGL(glfwGetProcAddress);

    // Set callbacks
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Window resizing
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetWindowAspectRatio(window, 16, 9);

    // Set render parameters
    glEnable(GL_DEPTH_TEST); // Enable depth test
    glDepthFunc(GL_LESS);    // Accept fragment if it closer to the camera than the former one

    // Optimizing: Don't draw the back of every face
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // AA: 4 is normal, 16 for screenshots :)
    glfwWindowHint(GLFW_SAMPLES, 16);
    glEnable(GL_MULTISAMPLE);

    // Wtf does this do again?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Load shaders
    ResourceManager::load_shader("../data/shaders/default.vs", "../data/shaders/default.fs", nullptr, "default");
    ResourceManager::load_shader("../data/shaders/light.vs", "../data/shaders/light.fs", nullptr, "light");
    ResourceManager::load_texture("../data/textures/awesomeface.png", true, "face");
    ResourceManager::load_texture("../data/textures/container.jpg", true, "container");
    ResourceManager::load_texture("../data/textures/grid512.png", false, "grid");
    ResourceManager::load_texture("../data/textures/white.jpg", false, "white");

    cube = new Cube();
    sphere = new Sphere();

    return 1;
}

GLFWwindow *Renderer::get_window()
{
    return window;
}

void Renderer::render_all()
{
    float curr_time = static_cast<float>(glfwGetTime());
    delta_time = curr_time - prev_time;
    prev_time = curr_time;

    // Clear the screen to black and render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int screen_width, screen_height;
    glfwGetWindowSize(window, &screen_width, &screen_height);

    // get matrix's uniform location and set matrix
    Shader default_shader = ResourceManager::get_shader("default");
    Shader light_shader = ResourceManager::get_shader("light");
    Texture face_tex = ResourceManager::get_texture("face");
    Texture container_tex = ResourceManager::get_texture("container");
    Texture container_tex2 = ResourceManager::get_texture("container2");
    Texture grid_tex = ResourceManager::get_texture("grid");
    Texture white_tex = ResourceManager::get_texture("white");
    glActiveTexture(GL_TEXTURE0);
    white_tex.bind();

    glm::vec3 light_color;
    light_color.x = 1;
    light_color.y = 1;
    light_color.z = 1;
    // http://devernay.free.fr/cours/opengl/materials.html
    glm::vec3 ambient_color = light_color * glm::vec3(0.05f, 0.05f, 0.05f); // decrease influence
    glm::vec3 diffuse_color = light_color * glm::vec3(0.5f, 0.5f, 0.5f);    // decrease influence
    glm::vec3 specular_color(0.5, 0.5, 0.5);                                // decrease influence
    // Render the regular cube
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    glm::mat4 view = camera.get_view_matrix();
    default_shader.use();
    default_shader.set_vector_3f("dir_light.direction", -0.2, 0.6, -0.3);
    default_shader.set_vector_3f("dir_light.ambient", ambient_color);
    default_shader.set_vector_3f("dir_light.diffuse", diffuse_color);
    default_shader.set_vector_3f("dir_light.specular", specular_color);
    default_shader.set_vector_3f("point_light.ambient", ambient_color);
    default_shader.set_vector_3f("point_light.diffuse", diffuse_color);
    default_shader.set_vector_3f("point_light.specular", specular_color);
    default_shader.set_vector_3f("point_light.position", light_position);
    default_shader.set_float("point_light.constant", 1.0f);
    default_shader.set_float("point_light.linear", 0.045f);
    default_shader.set_float("point_light.quadratic", 0.0075f);
    default_shader.set_vector_3f("spot_light.position", camera.position);
    default_shader.set_vector_3f("spot_light.direction", camera.front);
    default_shader.set_vector_3f("spot_light.ambient", 0.0f, 0.0f, 0.0f);
    default_shader.set_vector_3f("spot_light.diffuse", 1.0f, 1.0f, 1.0f);
    default_shader.set_vector_3f("spot_light.specular", 1.0f, 1.0f, 1.0f);
    default_shader.set_float("spot_light.constant", 1.0f);
    default_shader.set_float("spot_light.linear", 0.09f);
    default_shader.set_float("spot_light.quadratic", 0.032f);
    default_shader.set_float("spot_light.cutoff", glm::cos(glm::radians(12.5f)));
    default_shader.set_float("spot_light.outer_cutoff", glm::cos(glm::radians(15.0f)));
    default_shader.set_vector_3f("material.ambient", ambient_color);
    default_shader.set_vector_3f("material.diffuse", diffuse_color);
    default_shader.set_vector_3f("material.specular", specular_color);
    default_shader.set_float("material.shininess", 0.4 * 128);
    default_shader.set_matrix_4("projection", projection);
    default_shader.set_matrix_4("view", view);
    default_shader.set_vector_3f("view_position", camera.position);
    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
        default_shader.set_matrix_4("model", model);
        cube->draw();
    }

    // Render the light cube
    light_position.x = sin(glfwGetTime() * 1.5f) * 4.0f;
    light_position.z = cos(glfwGetTime() * 1.5f) * 4.0f;
    light_position.y = sin(glfwGetTime() * 1.5f) * 6.0f;
    glm::mat4 model(1.0f);
    model = glm::translate(model, light_position);
    model = glm::scale(model, glm::vec3(0.2f));
    light_shader.use();
    light_shader.set_matrix_4("projection", projection);
    light_shader.set_matrix_4("view", view);
    light_shader.set_matrix_4("model", model);
    cube->draw();

    // Process input
    glfwPollEvents();
    camera.process_input(delta_time);

    // FLip buffers and draw
    glfwSwapBuffers(window);
}

void Renderer::terminate()
{
    glfwTerminate();
}