#include "resource_manager.h"

#include <fmt/core.h>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::Textures;

Shader ResourceManager::load_shader(const char *vertex_shader_file, const char *fragment_shader_file,
                                    const char *geometry_shader_file, std::string name)
{
    shaders[name] = load_shader_from_file(vertex_shader_file, fragment_shader_file, geometry_shader_file);
    return shaders[name];
}

Shader ResourceManager::get_shader(std::string name)
{
    return shaders[name];
}

Texture ResourceManager::load_texture(const char *file, bool alpha, std::string name)
{
    Textures[name] = load_texture_from_file(file, alpha);
    return Textures[name];
}

Texture ResourceManager::get_texture(std::string name)
{
    return Textures[name];
}

void ResourceManager::clear()
{
    // Delete all shaders
    for (auto shader : shaders)
        glDeleteProgram(shader.second.id);
}

Shader ResourceManager::load_shader_from_file(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file)
{
    // Retrieve the source code from file paths
    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;

    try
    {
        // Open files
        std::ifstream vertex_shader_file_stream(vertex_shader_file);
        std::ifstream fragement_shader_file_stream(fragment_shader_file);
        std::stringstream vertex_shader_stream, fragement_shader_stream;

        // Read file's buffer contents into streams
        vertex_shader_stream << vertex_shader_file_stream.rdbuf();
        fragement_shader_stream << fragement_shader_file_stream.rdbuf();

        // Close file handlers
        vertex_shader_file_stream.close();
        fragement_shader_file_stream.close();

        // Convert stream into string
        vertex_code = vertex_shader_stream.str();
        fragment_code = fragement_shader_stream.str();

        // Load geometry shader if present
        if (geometry_shader_file != nullptr)
        {
            std::ifstream geometry_shader_file_stream(geometry_shader_file);
            std::stringstream geometry_shader_stream;
            geometry_shader_stream << geometry_shader_file_stream.rdbuf();
            geometry_shader_file_stream.close();
            geometry_code = geometry_shader_stream.str();
        }
    }
    catch (std::exception e)
    {
        fmt::print(stderr, "SHADER ERROR: Failed to read shader files\n");
    }

    const char *vertex_shader_code = vertex_code.c_str();
    const char *fragment_shader_code = fragment_code.c_str();
    const char *geometry_shader_code = geometry_code.c_str();

    // Compile shader object
    Shader shader;
    shader.compile(vertex_shader_code, fragment_shader_code, geometry_shader_file != nullptr ? geometry_shader_code : nullptr);

    return shader;
}

Texture ResourceManager::load_texture_from_file(const char *file, bool alpha)
{
    // create texture object
    Texture *texture = new Texture();
    if (alpha)
    {
        texture->internal_format = GL_RGBA;
        texture->image_format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture->generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return *texture;
}
