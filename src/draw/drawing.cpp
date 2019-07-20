#include <drawing.hpp>

using namespace std;
using namespace glm;

Drawing::Drawing(){}

Drawing::~Drawing(){}

void Drawing::init(const char* vertexPath, const char* fragmentPath){
    m_shader.initShaderProgram(vertexPath, fragmentPath);
}

void Drawing::setUniformPos(std::string name){
    GLint pos = m_shader.getUniformLocation(name.c_str());
    if(pos != GL_INVALID_INDEX ) m_uniformMap[name] = pos; 
}

void Drawing::uploadUniform(string name, mat4 matval){
    auto it = m_uniformMap.find(name);
    if ( it == m_uniformMap.end() ) return;
    GLint id = it->second;
    m_shader.enable();
    {
        glUniformMatrix4fv( id, 1, GL_FALSE, value_ptr( matval ) );
    }
    m_shader.disable();
}

void Drawing::uploadUniform(string name, glm::vec3 vecval){
    auto it = m_uniformMap.find(name);
    if ( it == m_uniformMap.end() ) return;
    GLint id = it->second;
    m_shader.enable();
    {
        glUniform3f( id, vecval[0], vecval[1], vecval[2] ); 
    }
    m_shader.disable();
}

void Drawing::draw(){
    m_shader.enable();
    {
        glBindVertexArray( m_vao ); 
        glDrawElements(m_mode, m_size, m_type, 0);  
    }
    m_shader.disable();
}