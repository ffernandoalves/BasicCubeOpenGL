#pragma once
#include "debug.hpp"

#include <string>
#include <fstream>
#include <sstream>


struct ShaderProgramSource {
    std::string vertex_source;
    std::string fragment_source;
};

static ShaderProgramSource parserShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

GLuint loadShader(GLenum shader_type, const std::string& shader_source) {
    GLCall(GLuint shader_id = glCreateShader(shader_type));
    const char* source = shader_source.c_str();
    GLCall(glShaderSource(shader_id, 1, &source, nullptr));
    GLCall(glCompileShader(shader_id));

    //Verifique se a compilação foi bem-sucedida
    GLint success;
    GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success));
    if (success == GL_FALSE) {
        GLint length;
        GLCall(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
        GLsizei log_length = 0;
        int max_length = 1024;
        GLchar message[max_length];
        GLCall(glGetShaderInfoLog(shader_id, max_length, &log_length, message));
        std::cerr << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cerr << message << std::endl;
        GLCall(glDeleteShader(shader_id));
        return 0;
    }

    return shader_id;
}

GLuint createProgram(const std::string& vertex_shader_source, const std::string& fragment_shader_source) {    
    GLuint vertex_shader_id = loadShader(GL_VERTEX_SHADER, vertex_shader_source);
    GLuint fragment_shader_id = loadShader(GL_FRAGMENT_SHADER, fragment_shader_source);

    GLCall(GLuint shader_program_id = glCreateProgram());
    GLCall(glAttachShader(shader_program_id, vertex_shader_id));
    GLCall(glAttachShader(shader_program_id, fragment_shader_id));
    GLCall(glLinkProgram(shader_program_id));
    GLCall(glValidateProgram(shader_program_id));

    // Verifique se a ligação foi bem-sucedida
    GLint success;
    GLCall(glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success));
    if (success == GL_FALSE) {
        char info_log[512];
        GLCall(glGetProgramInfoLog(shader_program_id, 512, nullptr, info_log));
        std::cerr << "Erro na ligação do programa:\n" << info_log << std::endl;
        return 0;
    }
    GLCall(glDeleteShader(vertex_shader_id));
    GLCall(glDeleteShader(fragment_shader_id));

    return shader_program_id;
}