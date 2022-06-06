#include "model.h"
#include <fmt/core.h>

void Model::init()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(tex_coords[0]), tex_coords.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertices[0]), (void *)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(normals[0]), (void *)0);

    // tex coords
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(tex_coords[0]), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::draw()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);
    glBindVertexArray(0);
}

void Model::calculate_normals()
{
    for (int i = 0; i < vertices.size(); i += 9)
    {
        glm::vec3 a(vertices[i], vertices[i + 1], vertices[i + 2]);
        glm::vec3 b(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
        glm::vec3 c(vertices[i + 6], vertices[i + 7], vertices[i + 8]);
        glm::vec3 tri_norm = glm::cross(b - a, c - a);

        for (int j = 0; j < 3; j++)
        {
            normals.push_back(tri_norm.x);
            normals.push_back(tri_norm.y);
            normals.push_back(tri_norm.z);
        }
    }
}

void Model::add_vertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(x);
}
void Model::add_normal(float x, float y, float z)
{
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}
void Model::add_tex_coord(float s, float t)
{
    tex_coords.push_back(s);
    tex_coords.push_back(t);
}
void Model::add_indices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

void Model::add_vertices(const float v1[3], const float v2[3], const float v3[3], const float v4[3])
{
    vertices.insert(vertices.end(), v1, v1 + 3); // v1
    vertices.insert(vertices.end(), v2, v2 + 3); // v2
    vertices.insert(vertices.end(), v3, v3 + 3); // v3
    vertices.insert(vertices.end(), v4, v4 + 3); // v4
}
void Model::add_normals(const float n1[3], const float n2[3], const float n3[3], const float n4[3])
{
    normals.insert(normals.end(), n1, n1 + 3); // n1
    normals.insert(normals.end(), n2, n2 + 3); // n2
    normals.insert(normals.end(), n3, n3 + 3); // n3
    normals.insert(normals.end(), n4, n4 + 3); // n4
}
void Model::add_tex_coords(const float t1[2], const float t2[2], const float t3[2], const float t4[2])
{
    tex_coords.insert(tex_coords.end(), t1, t1 + 2); // t1
    tex_coords.insert(tex_coords.end(), t2, t2 + 2); // t2
    tex_coords.insert(tex_coords.end(), t3, t3 + 2); // t3
    tex_coords.insert(tex_coords.end(), t4, t4 + 2); // t4
}