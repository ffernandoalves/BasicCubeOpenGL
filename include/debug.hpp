#pragma once
#include <glad/gl.h>

#include <iostream>

#include <signal.h>

#if defined (_WIN32)
    #include <windows.h>
    #define DEBUG_BREAK __debugbreak();
#elif defined (__linux__)
    #define DEBUG_BREAK raise(SIGTRAP);
#endif

#define ASSERT(x) if (!(x)) DEBUG_BREAK;
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cerr << "[OpenGL Error] (" << std::hex << error << std::dec << ")" << 
                " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}