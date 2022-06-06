#include "camera.h"
#include <fmt/core.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ZOOM = 45;

// Camera speeds
const float MOVE_SPEED = 25;
const float LOOK_SPEED = 75;
const float ZOOM_SPEED = 25;

Camera::Camera(glm::vec3 position)
{
    this->position = position;
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    yaw = YAW;
    pitch = PITCH;
    zoom = ZOOM;
    move_speed = MOVE_SPEED;
    look_speed = LOOK_SPEED;
    zoom_speed = ZOOM_SPEED;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::process_input(float delta_time)
{
    /*
        Controls:
        W: Move forward
        A: Move left
        S: Move backward
        D: Move right
        Q: Move down
        E: Move up

        H: Look left
        J: Look down
        K: Look up
        L: Look right

        U: Zoom out (decrease zoom)
        I: Zoom in (increase zoom)
    */

    // MOVE
    float speed = move_speed * delta_time;
    if (keys[GLFW_KEY_W])
        position += front * speed;
    if (keys[GLFW_KEY_S])
        position -= front * speed;
    if (keys[GLFW_KEY_A])
        position -= right * speed;
    if (keys[GLFW_KEY_D])
        position += right * speed;
    if (keys[GLFW_KEY_E])
        position += up * speed;
    if (keys[GLFW_KEY_Q])
        position -= up * speed;

    // LOOK
    speed = look_speed * delta_time;
    if (keys[GLFW_KEY_K])
        pitch += speed;
    if (keys[GLFW_KEY_J])
        pitch -= speed;
    if (keys[GLFW_KEY_H])
        yaw -= speed;
    if (keys[GLFW_KEY_L])
        yaw += speed;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    update_camera_vectors();

    // ZOOM
    speed = zoom_speed * delta_time;
    if (keys[GLFW_KEY_I])
        zoom -= speed;
    if (keys[GLFW_KEY_U])
        zoom += speed;
    if (zoom > 179)
        zoom = 179;
    else if (zoom < 1)
        zoom = 1;

    fmt::print("x: {} .. y:{} .. z:{} .. fx: {} .. fy: {} .. fz: {} .. p: {} .. y: {}\n",
               position.x, position.y, position.z, front.x, front.y, front.z, pitch, yaw);
}
void Camera::update_camera_vectors()
{
    // calculate the new front vector
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);
    // also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, world_up)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}