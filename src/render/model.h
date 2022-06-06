#ifndef Model_H
#define Model_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

class Model
{
public:
    Model(){};
    unsigned int vao, vbo, nbo, tbo, ebo;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> tex_coords;
    std::vector<unsigned int> indices;
    void add_vertex(float x, float y, float z);
    void add_normal(float x, float y, float z);
    void add_tex_coord(float s, float t);
    void add_vertices(const float v1[3], const float v2[3], const float v3[3], const float v4[3]);
    void add_normals(const float n1[3], const float n2[3], const float n3[3], const float n4[3]);
    void add_tex_coords(const float t1[2], const float t2[2], const float t3[2], const float t4[2]);
    void add_indices(unsigned int i1, unsigned int i2, unsigned int i3);
    void draw();
    void init();
    virtual void calculate_normals();
};

#endif