#ifndef SPHERE_H
#define SPHERE_H

#include "model.h"

class Sphere : public Model
{
public:
    Sphere();

private:
    std::vector<float> get_unit_pos_x(unsigned int vertex_counts_per_row);
    float compute_scale_for_length(const float v[3], float length);
    void compute_face_normal(const float v1[3], const float v2[3], const float v3[3], float n[3]);
};

#endif