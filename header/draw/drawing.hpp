#pragma once
#include <glm/ext.hpp>
#include <string>
#include <map>
#include "shader.hpp"

class Drawing{
protected:
    ShaderProgram m_shader;
    // uniforms
    std::map<std::string, GLint> m_uniformMap;
    //buffer
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    //draw type
    GLenum m_mode = 0;
    GLsizei m_size = 0;
    GLenum m_type = 0;

    void setUniformPos(std::string name);
public:
    Drawing();
    ~Drawing();
    void init(const char* vertexPath, const char* fragmentPath);
    virtual void registerUniform() = 0;
    virtual void uploadModel() = 0;
    void uploadUniform(std::string name, glm::mat4 matval);
    void uploadUniform(std::string name, glm::vec3 vecval);
    void draw();
};