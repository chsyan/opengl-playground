#include "cube.h"
#include <fmt/core.h>

static std::vector<float> vertex_data = {
    // Back face
    -0.5f, -0.5f, -0.5f, // Bottom-left
     0.5f,  0.5f, -0.5f, // top-right
     0.5f, -0.5f, -0.5f, // bottom-right         
     0.5f,  0.5f, -0.5f, // top-right
    -0.5f, -0.5f, -0.5f, // bottom-left
    -0.5f,  0.5f, -0.5f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f, // bottom-left
     0.5f, -0.5f,  0.5f, // bottom-right
     0.5f,  0.5f,  0.5f, // top-right
     0.5f,  0.5f,  0.5f, // top-right
    -0.5f,  0.5f,  0.5f, // top-left
    -0.5f, -0.5f,  0.5f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f, // top-right
    -0.5f,  0.5f, -0.5f, // top-left
    -0.5f, -0.5f, -0.5f, // bottom-left
    -0.5f, -0.5f, -0.5f, // bottom-left
    -0.5f, -0.5f,  0.5f, // bottom-right
    -0.5f,  0.5f,  0.5f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f, // top-left
     0.5f, -0.5f, -0.5f, // bottom-right
     0.5f,  0.5f, -0.5f, // top-right         
     0.5f, -0.5f, -0.5f, // bottom-right
     0.5f,  0.5f,  0.5f, // top-left
     0.5f, -0.5f,  0.5f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f, // top-right
     0.5f, -0.5f, -0.5f, // top-left
     0.5f, -0.5f,  0.5f, // bottom-left
     0.5f, -0.5f,  0.5f, // bottom-left
    -0.5f, -0.5f,  0.5f, // bottom-right
    -0.5f, -0.5f, -0.5f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f, // top-left
     0.5f,  0.5f,  0.5f, // bottom-right
     0.5f,  0.5f, -0.5f, // top-right     
     0.5f,  0.5f,  0.5f, // bottom-right
    -0.5f,  0.5f, -0.5f, // top-left
    -0.5f,  0.5f,  0.5f, // bottom-left        
};

static std::vector<unsigned int> index_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                               19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

static std::vector<float> tex_data = {
    0.0f, 0.0f, // Bottom-left
    1.0f, 1.0f, // top-right
    1.0f, 0.0f, // bottom-right         
    1.0f, 1.0f, // top-right
    0.0f, 0.0f, // bottom-left
    0.0f, 1.0f, // top-left
    
    0.0f, 0.0f, // bottom-left
    1.0f, 0.0f, // bottom-right
    1.0f, 1.0f, // top-right
    1.0f, 1.0f, // top-right
    0.0f, 1.0f, // top-left
    0.0f, 0.0f, // bottom-left
    
    1.0f, 0.0f, // top-right
    1.0f, 1.0f, // top-left
    0.0f, 1.0f, // bottom-left
    0.0f, 1.0f, // bottom-left
    0.0f, 0.0f, // bottom-right
    1.0f, 0.0f, // top-right
    
    1.0f, 0.0f, // top-left
    0.0f, 1.0f, // bottom-right
    1.0f, 1.0f, // top-right         
    0.0f, 1.0f, // bottom-right
    1.0f, 0.0f, // top-left
    0.0f, 0.0f, // bottom-left     
    
    0.0f, 1.0f, // top-right
    1.0f, 1.0f, // top-left
    1.0f, 0.0f, // bottom-left
    1.0f, 0.0f, // bottom-left
    0.0f, 0.0f, // bottom-right
    0.0f, 1.0f, // top-right
    
    0.0f, 1.0f, // top-left
    1.0f, 0.0f, // bottom-right
    1.0f, 1.0f, // top-right     
    1.0f, 0.0f, // bottom-right
    0.0f, 1.0f, // top-left
    0.0f, 0.0f  // bottom-left        
};

Cube::Cube()
{
    vertices = vertex_data;
    indices = index_data;
    tex_coords = tex_data;
    calculate_normals();
    init();
};