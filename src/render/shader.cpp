#include "shader.h"

#include <fmt/core.h>

Shader &Shader::use()
{
    glUseProgram(this->id);
    return *this;
}

void Shader::compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource)
{
    unsigned int vertex_shader, fragment_shader, geometry_shader;

    // Compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexSource, NULL);
    glCompileShader(vertex_shader);
    check_compile_err(vertex_shader, "VERTEX");

    // Compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentSource, NULL);
    glCompileShader(fragment_shader);
    check_compile_err(fragment_shader, "FRAGMENT");

    // Compile geometry shader if provided
    if (geometrySource != nullptr)
    {
        geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry_shader, 1, &geometrySource, NULL);
        glCompileShader(geometry_shader);
        check_compile_err(geometry_shader, "GEOMETRY");
    }

    // Create shader program and link
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader);
    glAttachShader(this->id, fragment_shader);
    if (geometrySource != nullptr)
        glAttachShader(this->id, geometry_shader);
    glLinkProgram(this->id);
    check_compile_err(this->id, "PROGRAM");

    // Delete the shaders since they are already linked
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (geometrySource != nullptr)
        glDeleteShader(geometry_shader);
}

void Shader::set_float(const char *name, float value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform1f(glGetUniformLocation(this->id, name), value);
}
void Shader::set_integer(const char *name, int value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
}
void Shader::set_vector_2f(const char *name, float x, float y, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}
void Shader::set_vector_2f(const char *name, const glm::vec2 &value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}
void Shader::set_vector_3f(const char *name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}
void Shader::set_vector_3f(const char *name, const glm::vec3 &value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}
void Shader::set_vector_4f(const char *name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}
void Shader::set_vector_4f(const char *name, const glm::vec4 &value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}
void Shader::set_matrix_4(const char *name, const glm::mat4 &matrix, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}

void Shader::check_compile_err(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            fmt::print("SHADER COMPILE-TIME ERROR: Type {}\n{}\n", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            fmt::print("SHADER LINK-TIME ERROR: Type {}\n{}\n", type, infoLog);
        }
    }
}
