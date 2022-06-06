#version 330 core

struct Material {
    vec3 specular;
    sampler2D diffuse;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient, diffuse, specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
};

struct SpotLight {
    vec3 position, direction;
    float cutoff, outer_cutoff;
    vec3 ambient, diffuse, specular;
    float constant, linear, quadratic;
};

in VS_OUT {
    vec3 normal;
    vec3 frag_position;
    vec2 tex_coord;
} fs_in;

out vec4 color;

#define MAX_POINT_LIGHTS 10

uniform vec3 view_position;
uniform Material material;
uniform PointLight point_light;
uniform DirectionalLight dir_light;
uniform PointLight point_lights[10];
uniform SpotLight spot_light;

// function prototypes
vec3 calc_dir_light(DirectionalLight light, vec3 normal, vec3 view_direction, vec3 texture_diffuse, Material material);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_direction, vec3 texture_diffuse, Material material);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_direction, vec3 texture_diffuse, Material material);

void main() {
    // Texture
    vec3 tex_diffuse = texture(material.diffuse, fs_in.tex_coord).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 view_direction = normalize(view_position - fs_in.frag_position);
    vec3 light_model = calc_point_light(point_light, normal, view_direction, tex_diffuse, material);
    light_model += calc_dir_light(dir_light, normal, view_direction, tex_diffuse, material);
    light_model += calc_spot_light(spot_light, normal, view_direction, tex_diffuse, material);

    color = vec4(light_model, 1.0);
}

vec3 calc_dir_light(DirectionalLight light, vec3 normal, vec3 view_direction, vec3 texture_diffuse, Material material) {
    vec3 light_direction = normalize(-light.direction);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float diff = max(dot(normal, light_direction), 0.0);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture_diffuse;
    vec3 diffuse = light.diffuse * diff * texture_diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return ambient + diffuse + specular;
}
vec3 calc_point_light(PointLight light, vec3 normal, vec3 view_direction, vec3 texture_diffuse, Material material) {
    float dist = length(light.position - fs_in.frag_position);
    vec3 light_direction = normalize(light.position - fs_in.frag_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float diff = max(dot(normal, light_direction), 0.0);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    vec3 ambient = light.ambient * texture_diffuse;
    vec3 diffuse = light.diffuse * diff * texture_diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular) * attenuation;
}
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 view_direction, vec3 texture_diffuse, Material material) {
    float dist = length(light.position - fs_in.frag_position);
    vec3 light_direction = normalize(light.position - fs_in.frag_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float diff = max(dot(normal, light_direction), 0.0);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    float theta = dot(light_direction, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * texture_diffuse;
    vec3 diffuse = light.diffuse * diff * texture_diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular) * attenuation * intensity;
}