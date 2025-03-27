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


void SetShaderUniformInt(const GLuint shaderID, const std::string &name, const int value) {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void SetShaderUniformUint(const GLuint shaderID, const std::string &name, const unsigned int value) {
    glUniform1ui(glGetUniformLocation(shaderID, name.c_str()), value);
}

void SetShaderUniformFloat(const GLuint shaderID, const std::string &name, const float value) {
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void SetShaderUniformVec2(const GLuint shaderID, const std::string &name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}

void SetShaderUniformVec3(const GLuint shaderID, const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}

void SetShaderUniformVec4(const GLuint shaderID, const std::string &name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}

void SetShaderUniformMat4(const GLuint shaderID, const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

