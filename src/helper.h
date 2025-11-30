#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>

GLuint createShaderProgram(const char* vsSource, const char* fsSource);

GLuint createLineVAO(float x1, float y1, float x2, float y2);
GLuint createPolylineVAO(const std::vector<float>& points);