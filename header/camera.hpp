#pragma once
#include <glm/ext.hpp>

class Camera{
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), 
            glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 getViewMatrix();
};
