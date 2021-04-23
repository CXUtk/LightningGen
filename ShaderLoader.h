#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>


GLuint loadVertexFragmantShader(const std::string& vertexPath, const std::string& fragmentPath);

GLuint loadVertexGeometryShader(const std::string& vertexPath, const std::string& geometryPath, const char* const* feedBackValues, int numValues);