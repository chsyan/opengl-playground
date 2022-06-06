#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>


class Shader
{
public:
    unsigned int id; // Id to reference this shader object

    // Constructor
    Shader() { }

    // Set the current shader as active (Use this shader)
    Shader  &use();

    // Compiles the shader from given source GLSL source code
    // Optional: geometry_source
    void    compile(const char *vertex_source, const char *fragment_source, const char *geometry_source = nullptr);

    // Util funcs
    void    set_float    (const char *name, float value, bool use_shader = false);
    void    set_integer  (const char *name, int value, bool use_shader = false);
    void    set_vector_2f (const char *name, float x, float y, bool use_shader = false);
    void    set_vector_2f (const char *name, const glm::vec2 &value, bool use_shader = false);
    void    set_vector_3f (const char *name, float x, float y, float z, bool use_shader = false);
    void    set_vector_3f (const char *name, const glm::vec3 &value, bool use_shader = false);
    void    set_vector_4f (const char *name, float x, float y, float z, float w, bool use_shader = false);
    void    set_vector_4f (const char *name, const glm::vec4 &value, bool use_shader = false);
    void    set_matrix_4  (const char *name, const glm::mat4 &matrix, bool use_shader = false);
private:

    // Check for compilation errors and print logs
    void    check_compile_err(unsigned int object, std::string type); 
};

#endif

