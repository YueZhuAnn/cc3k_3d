#pragma once
#include <glad/glad.h>
#include <string>

class ShaderProgram{
    struct Shader {
        GLuint shaderID;
        std::string filePath;
        Shader();
    };
    GLuint m_ID;
    Shader vertexShader;
    Shader fragmentShader;
    bool compileShader(const Shader &shader);
public:
    ShaderProgram();
    ~ShaderProgram();
    void initShaderProgram(const char* vertexPath, const char* fragmentPath);
    GLuint getID();
    GLint getAttribLocation( const char * attributeName ) const;
    GLint getUniformLocation( const char * uniformName ) const;
    void enable();
    void disable();
};
