#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#ifdef RELEASE

#define GL_CHECK_ERRORS(x) x;

#else

#define GL_CHECK_ERRORS(x) { \
    x; \
    GLenum GL_CHECK_ERRORS_errorCode; \
    while ((GL_CHECK_ERRORS_errorCode = glGetError()) != GL_NO_ERROR) \
    { \
        std::string GL_CHECK_ERRORS_errorString; \
        switch (errorCode) \
        { \
            case GL_INVALID_ENUM:                  GL_CHECK_ERRORS_errorString = "INVALID_ENUM"; break; \
            case GL_INVALID_VALUE:                 GL_CHECK_ERRORS_errorString = "INVALID_VALUE"; break; \
            case GL_INVALID_OPERATION:             GL_CHECK_ERRORS_errorString = "INVALID_OPERATION"; break; \
            case GL_STACK_OVERFLOW:                GL_CHECK_ERRORS_errorString = "STACK_OVERFLOW"; break; \
            case GL_STACK_UNDERFLOW:               GL_CHECK_ERRORS_errorString = "STACK_UNDERFLOW"; break; \
            case GL_OUT_OF_MEMORY:                 GL_CHECK_ERRORS_errorString = "OUT_OF_MEMORY"; break; \
            case GL_INVALID_FRAMEBUFFER_OPERATION: GL_CHECK_ERRORS_errorString = "INVALID_FRAMEBUFFER_OPERATION"; break; \
        } \
        std::cerr << "GL Error: " << GL_CHECK_ERRORS_errorString << ", File: " << __FILE__ << ", Line: " << __LINE__ << std::endl; \
    } \
}

#endif


