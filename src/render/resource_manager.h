#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/gl.h>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
    // Resources
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> Textures;

    // loads a shader program from file
    // GLSL source code of vertex, fragment, and optional geometry
    static Shader load_shader(const char *vertex_shader_file, const char *fragment_shader_file,
                              const char *geometry_shader_file, std::string name);

    static Shader get_shader(std::string name);
    static Texture load_texture(const char *file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture get_texture(std::string name);
    // Clear all allocated resources
    static void clear();

private:
    // Private constructor, all member and functions are static
    ResourceManager() {}

    // Loads a shader from file
    static Shader load_shader_from_file(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file = nullptr);

    static Texture load_texture_from_file(const char *file, bool alpha);
};

#endif
