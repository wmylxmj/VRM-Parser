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
extern GLuint LinkProgram(std::initializer_list<GLuint> shaderIDs);

void SetShaderUniformInt(GLuint shaderID, const std::string &name, int value);
void SetShaderUniformUint(GLuint shaderID, const std::string &name, unsigned int value);
void SetShaderUniformFloat(GLuint shaderID, const std::string &name, float value);
void SetShaderUniformVec2(GLuint shaderID, const std::string &name, const glm::vec2 &value);
void SetShaderUniformVec3(GLuint shaderID, const std::string &name, const glm::vec3 &value);
void SetShaderUniformVec4(GLuint shaderID, const std::string &name, const glm::vec4 &value);
void SetShaderUniformMat2(GLuint shaderID, const std::string &name, const glm::mat2 &value);
void SetShaderUniformMat3(GLuint shaderID, const std::string &name, const glm::mat3 &value);
void SetShaderUniformMat4(GLuint shaderID, const std::string &name, const glm::mat4 &value);



