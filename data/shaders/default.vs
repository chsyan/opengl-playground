#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transform_normal;

out VS_OUT
{
    vec3 normal;
    vec3 frag_position;
    vec2 tex_coord;
} vs_out;

void main()
{
    vs_out.normal = mat3(transpose(inverse(model))) * normal;
    vs_out.frag_position = vec3(model * vec4(position, 1.0));
    vs_out.tex_coord = tex_coord;
    gl_Position = projection * view * vec4(vs_out.frag_position, 1.0);
}
