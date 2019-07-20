#include "camera.hpp"

using namespace glm;

Camera::Camera(vec3 position, vec3 target, vec3 up):
    m_position(position), m_target(target){
        // calculate the right direction and new up direction
        m_front = normalize(m_position - m_target);
        m_right = normalize(cross(up, m_front));
        m_up    = normalize(cross(m_front, m_right));
}

mat4 Camera::getViewMatrix(){
    return lookAt(m_position, m_target, m_up);
}