#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.hpp"

using namespace std;

ShaderProgram::ShaderProgram():m_ID((GLuint)NULL){}

ShaderProgram::~ShaderProgram(){
    glDeleteShader(vertexShader.shaderID);
    glDeleteShader(fragmentShader.shaderID);
    glDeleteProgram(m_ID);
}

ShaderProgram::Shader::Shader():shaderID((GLuint)NULL), filePath(""){}

void ShaderProgram::initShaderProgram(const char* vertexPath, const char* fragmentPath){
    // create & compile vertexShader
    vertexShader.shaderID = glCreateShader(GL_VERTEX_SHADER);
    vertexShader.filePath = vertexPath;
    if (!compileShader(vertexShader)){
        cerr << "vertexShader compile failed" << endl;
    }

    // create & compile fragmentShader
    fragmentShader.shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader.filePath = fragmentPath;
    if (!compileShader(fragmentShader)){
        cerr << "fragmentShader compile failed" << endl;
    }

    // link shaders
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader.shaderID);
    glAttachShader(m_ID, fragmentShader.shaderID);
    glLinkProgram(m_ID);

    // check for linking errors
    GLint linkResult;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE) {
        cerr << "Shader link failed" << endl;
    }
    glDeleteShader(vertexShader.shaderID);
    glDeleteShader(fragmentShader.shaderID);
}

bool ShaderProgram::compileShader(const Shader &shader){
    // open source code
    ifstream ifSource(shader.filePath);
    if (!ifSource.is_open()) return false;
    stringstream sSourceStream;
    sSourceStream << ifSource.rdbuf();
    // convert to char array
    const char* sourceCode = sSourceStream.str().c_str();
    // compile the source code
    glShaderSource(shader.shaderID, 1, (const GLchar **)&sourceCode, NULL);
    glCompileShader(shader.shaderID);
    // check the compile result
    GLint compileResult;
    glGetShaderiv(shader.shaderID, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) return false;
    return true;
}

void ShaderProgram::enable(){
    glUseProgram(m_ID);
}

void ShaderProgram::disable(){
    glUseProgram((GLuint)NULL);
}

GLuint ShaderProgram::getID(){ return m_ID; }

GLint ShaderProgram::getUniformLocation ( const char * uniformName ) const {
    GLint result = glGetUniformLocation(m_ID, (const GLchar *)uniformName);
    if (result == -1) {
#if DEBUG_INFO
        cerr << "Get Uniform Location Failed" << endl;
#endif
    }
    return result;
}

GLint ShaderProgram::getAttribLocation ( const char * attributeName ) const {
    GLint result = glGetAttribLocation(m_ID, (const GLchar *)attributeName);
    if (result == -1) {
#if DEBUG_INFO
        cerr << "Get Attribute Location Failed" << endl;
#endif
    }
    return result;
}
