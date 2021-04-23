#include "ShaderLoader.h"
#include <iostream>
#include <sstream>
#include <fstream>

static char infoLog[512];

GLuint compileVertexShader(const std::string& shaderCode) {
    GLuint id = glCreateShader(GL_VERTEX_SHADER);
    const char* code = shaderCode.c_str();
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        throw std::exception("Failed to compile vertex shader");
    }
    return id;
}

GLuint compileGeometryShader(const std::string& shaderCode) {
    GLuint id = glCreateShader(GL_GEOMETRY_SHADER);
    const char* code = shaderCode.c_str();
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        throw std::exception("Failed to compile vertex shader");
    }
    return id;
}

GLuint compileFragmentShader(const std::string& shaderCode) {
    GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
    const char* code = shaderCode.c_str();
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        throw std::exception("Failed to compile fragment shader");
    }
    return id;
}

GLuint loadVertexFragmantShader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::ifstream vertexFile, fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertexCode, fragmentCode;
    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vSS, fSS;
        vSS << vertexFile.rdbuf();
        fSS << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();

        vertexCode = vSS.str(), fragmentCode = fSS.str();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Failed to load shaders" << std::endl;
        throw std::exception(e.what());
    }
    int vertex = compileVertexShader(vertexCode);
    int fragment = compileFragmentShader(fragmentCode);

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        throw std::exception("Failed to link the shader program");
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return id;
}

GLuint loadVertexGeometryShader(const std::string& vertexPath, const std::string& geometryPath, const char* const* feedBackValues, int numValues) {
    std::ifstream vertexFile, geometryFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertexCode, geometryCode;
    try {
        vertexFile.open(vertexPath);
        geometryFile.open(geometryPath);

        std::stringstream vSS, fSS, gSS;
        vSS << vertexFile.rdbuf();
        gSS << geometryFile.rdbuf();
        vertexFile.close();
        geometryFile.close();

        vertexCode = vSS.str(), geometryCode = gSS.str();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Failed to load shaders" << std::endl;
        throw std::exception(e.what());
    }
    int vertex = compileVertexShader(vertexCode);
    int geometry = compileGeometryShader(geometryCode);

    GLuint id = glCreateProgram();
    glTransformFeedbackVaryings(id, numValues, feedBackValues, GL_INTERLEAVED_ATTRIBS);
    glAttachShader(id, vertex);
    glAttachShader(id, geometry);
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        throw std::exception("Failed to link the shader program");
    }

    glDeleteShader(vertex);
    glDeleteShader(geometry);
    return id;
}
