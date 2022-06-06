#include "sphere.h"
#include <cmath>

Sphere::Sphere()
{
    int sub = 3;
    unsigned int vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
    std::vector<float> unitVertices = get_unit_pos_x(vertexCountPerRow);

    unsigned int k = 0, k1, k2, i1, i2; // indices
    float v1[3], v2[3], v3[3], v4[3];   // tmp vertices
    float t1[2], t2[2], t3[2], t4[2];   // texture coords
    float n[3];                         // normal vector

    // +X face
    for(unsigned int i = 0; i < vertexCountPerRow - 1; ++i)
    {
        k1 = i * vertexCountPerRow;              // index at curr row
        k2 = k1 + vertexCountPerRow;             // index at next row

        // vertical tex coords
        t1[1] = t3[1] = (float)i / (vertexCountPerRow - 1);
        t2[1] = t4[1] = (float)(i+1) / (vertexCountPerRow - 1);

        for(unsigned int j = 0; j < vertexCountPerRow - 1; ++j, ++k1, ++k2)
        {
            i1 = k1 * 3;
            i2 = k2 * 3;

            // 4 vertices of a quad
            // v1--v3
            // | / |
            // v2--v4
            v1[0] = unitVertices[i1];
            v1[1] = unitVertices[i1+1];
            v1[2] = unitVertices[i1+2];
            v2[0] = unitVertices[i2];
            v2[1] = unitVertices[i2+1];
            v2[2] = unitVertices[i2+2];
            v3[0] = unitVertices[i1+3];
            v3[1] = unitVertices[i1+4];
            v3[2] = unitVertices[i1+5];
            v4[0] = unitVertices[i2+3];
            v4[1] = unitVertices[i2+4];
            v4[2] = unitVertices[i2+5];

            // compute face nornal
            Sphere::compute_face_normal(v1, v2, v3, n);

            // compute horizontal tex coords
            t1[0] = t2[0] = (float)j / (vertexCountPerRow - 1);
            t3[0] = t4[0] = (float)(j+1) / (vertexCountPerRow - 1);

            // add 4 vertex attributes
            add_vertices(v1, v2, v3, v4);
            add_normals(n, n, n, n);
            add_tex_coords(t1, t2, t3, t4);

            // add indices of 2 triangles
            add_indices(k, k+1, k+2);
            add_indices(k+2, k+1, k+3);

            k += 4;     // next
        }
    }

    // array size and index for building next face
    unsigned int startIndex;                    // starting index for next face
    int vertexSize = (int)vertices.size();      // vertex array size of +X face
    int indexSize = (int)indices.size();        // index array size of +X face

    // build -X face by negating x and z values
    startIndex = vertices.size() / 3;
    for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        add_vertex(-vertices[i], vertices[i+1], -vertices[i+2]);
        add_tex_coord(tex_coords[j], tex_coords[j+1]);
        add_normal(-normals[i], normals[i+1], -normals[i+2]);
    }
    for(int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // // build +Y face by swapping x=>y, y=>-z, z=>-x
    // startIndex = vertices.size() / 3;
    // for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    // {
    //     add_vertex(-vertices[i+2], vertices[i], -vertices[i+1]);
    //     add_tex_coord(tex_coords[j], tex_coords[j+1]);
    //     add_normal(-normals[i+2], normals[i], -normals[i+1]);
    // }
    // for(int i = 0; i < indexSize; ++i)
    // {
    //     indices.push_back(startIndex + indices[i]);
    // }

    // // build -Y face by swapping x=>-y, y=>z, z=>-x
    // startIndex = vertices.size() / 3;
    // for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    // {
    //     add_vertex(-vertices[i+2], -vertices[i], vertices[i+1]);
    //     add_tex_coord(tex_coords[j], tex_coords[j+1]);
    //     add_normal(-normals[i+2], -normals[i], normals[i+1]);
    // }
    // for(int i = 0; i < indexSize; ++i)
    // {
    //     indices.push_back(startIndex + indices[i]);
    // }

    // // build +Z face by swapping x=>z, z=>-x
    // startIndex = vertices.size() / 3;
    // for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    // {
    //     add_vertex(-vertices[i+2], vertices[i+1], vertices[i]);
    //     add_tex_coord(tex_coords[j], tex_coords[j+1]);
    //     add_normal(-normals[i+2], normals[i+1], normals[i]);
    // }
    // for(int i = 0; i < indexSize; ++i)
    // {
    //     indices.push_back(startIndex + indices[i]);
    // }

    // // build -Z face by swapping x=>-z, z=>x
    // startIndex = vertices.size() / 3;
    // for(int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    // {
    //     add_vertex(vertices[i+2], vertices[i+1], -vertices[i]);
    //     add_tex_coord(tex_coords[j], tex_coords[j+1]);
    //     add_normal(normals[i+2], normals[i+1], -normals[i]);
    // }
    // for(int i = 0; i < indexSize; ++i)
    // {
    //     indices.push_back(startIndex + indices[i]);
    // }

    init();
};

float Sphere::compute_scale_for_length(const float v[3], float length)
{
    return length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

std::vector<float> Sphere::get_unit_pos_x(unsigned int points_per_row)
{
    std::vector<float> vertices;
    float n1[3]; // normal of longitudinal plane rotating along Y-axis
    float n2[3]; // normal of latitudinal plane rotating along Z-axis
    float v[3];  // direction vector intersecting 2 planes, n1 x n2
    float a1;    // longitudinal angle along y-axis
    float a2;    // latitudinal angle
    float scale;

    for (unsigned int i = 0; i < points_per_row; ++i)
    {
        a2 = glm::radians(45.0f - 90.0f * i / (points_per_row - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        for (unsigned int j = 0; j < points_per_row; ++j)
        {
            a1 = glm::radians(-45.0f + 90.0f * j / (points_per_row - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            scale = compute_scale_for_length(v, 1);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }
    return vertices;
}

void Sphere::compute_face_normal(const float v1[3], const float v2[3], const float v3[3], float n[3])
{
    const float EPSILON = 0.000001f;

    // default return value (0, 0, 0)
    n[0] = n[1] = n[2] = 0;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = v2[0] - v1[0];
    float ey1 = v2[1] - v1[1];
    float ez1 = v2[2] - v1[2];
    float ex2 = v3[0] - v1[0];
    float ey2 = v3[1] - v1[1];
    float ez2 = v3[2] - v1[2];

    // cross product: e1 x e2
    float nx, ny, nz;
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        n[0] = nx * lengthInv;
        n[1] = ny * lengthInv;
        n[2] = nz * lengthInv;
    }
}
