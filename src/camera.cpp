#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position),
      m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_world_up(up),
      m_yaw(yaw),
      m_pitch(pitch),
      m_movement_speed(2.5f),
      m_mouse_sensitivity(0.1f)
{
    update();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_position(glm::vec3(posX, posY, posZ)),
      m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_world_up(glm::vec3(upX, upY, upZ)),
      m_yaw(yaw), m_pitch(pitch), m_movement_speed(2.5f),
      m_mouse_sensitivity(0.1f)
{
    update();
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::move(Direction direction, float deltaTime)
{
    float velocity = m_movement_speed * deltaTime;
    if (direction == FORWARD)
        m_position += m_front * velocity;
    if (direction == BACKWARD)
        m_position -= m_front * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void Camera::process_mouse(float xoffset, float yoffset)
{
    xoffset *= m_mouse_sensitivity;
    yoffset *= m_mouse_sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    update();
}

void Camera::update()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
