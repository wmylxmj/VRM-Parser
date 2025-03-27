//
// Created by 13973 on 25-3-16.
//

#include "Shader.h"

#include <memory>

GLuint CompileShader(const char* shaderCode, const GLenum shaderType) {
    const GLuint glID = glCreateShader(shaderType);
    glShaderSource(glID, 1, &shaderCode, nullptr);
    glCompileShader(glID);

    GLint success;
    glGetShaderiv(glID, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLint logLength;
        glGetShaderiv(glID, GL_INFO_LOG_LENGTH, &logLength);

        auto* infoLog = static_cast<GLchar *>(alloca(logLength * sizeof(GLchar)));
        glGetShaderInfoLog(glID, logLength, nullptr, infoLog);
        std::cout << infoLog << std::endl;
    }

    return glID;
}

GLuint LinkProgram(const std::initializer_list<GLuint> shaderIDs) {
    const GLuint program = glCreateProgram();
    for(const GLuint shader : shaderIDs) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        auto* infoLog = static_cast<GLchar *>(alloca(logLength * sizeof(GLchar)));
        glGetProgramInfoLog(program, logLength, nullptr, infoLog);
        std::cout << infoLog << std::endl;
    }

    return program;
}


void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(glID, name.c_str()), value);
}

void Shader::SetUint(const std::string &name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(glID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(glID, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &vec) const {
    glUniform2fv(glGetUniformLocation(glID, name.c_str()), 1, &vec[0]);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &vec) const {
    glUniform3fv(glGetUniformLocation(glID, name.c_str()), 1, &vec[0]);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &vec) const {
    glUniform4fv(glGetUniformLocation(glID, name.c_str()), 1, &vec[0]);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(glID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(glID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(glID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
