//
// Created by 13973 on 25-3-16.
//

#pragma once

#include "PrecompiledHeader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <initializer_list>

extern GLuint CompileShader(const char* shaderCode, GLenum shaderType);

class Shader {
public:
    GLuint glID;

    void SetInt(const std::string &name, int value) const;
    void SetUint(const std::string &name, unsigned int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &vec) const;
    void SetVec3(const std::string &name, const glm::vec3 &vec) const;
    void SetVec4(const std::string &name, const glm::vec4 &vec) const;
    void SetMat2(const std::string &name, const glm::mat2 &mat) const;
    void SetMat3(const std::string &name, const glm::mat3 &mat) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;
};

class VertexShader : public Shader {
public:
    explicit VertexShader(const char* pFile);
    ~VertexShader();
};

class FragmentShader : public Shader {
public:
    explicit FragmentShader(const char* pFile);
    ~FragmentShader();
};

class ShaderProgram : public Shader {
public:
    ShaderProgram(const char *pVertexShaderFile, const char *pFragmentShaderFile);
};
