#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include "helper.h"

static const float PI = 3.14159265359f;

GLuint createShaderProgram(const char* vs_source, const char* fs_source);
GLuint createLineVAO(float x1, float y1, float x2, float y2);
GLuint createPolylineVAO(const std::vector<float>& data);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(800, 800, "Modern OpenGL Clock", nullptr, nullptr);
    glfwMakeContextCurrent(w);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    // ------- Load shaders -------
    const char* vs =
        "#version 330 core\n"
        "layout(location=0) in vec2 aPos;"
        "uniform mat4 uTransform;"
        "uniform vec3 uColor;"
        "out vec3 vColor;"
        "void main(){ gl_Position = uTransform * vec4(aPos,0,1); vColor=uColor; }";

    const char* fs =
        "#version 330 core\n"
        "in vec3 vColor;"
        "out vec4 FragColor;"
        "void main(){ FragColor = vec4(vColor,1); }";

    GLuint shader = createShaderProgram(vs, fs);
    glUseProgram(shader);

    GLint locTransform = glGetUniformLocation(shader, "uTransform");
    GLint locColor = glGetUniformLocation(shader, "uColor");

    // --------- Build clock circle ----------
    std::vector<float> circle;
    for (int i=0; i<360; i++) {
        float a = i * PI / 180.0f;
        circle.push_back(cos(a)*0.8f);
        circle.push_back(sin(a)*0.8f);
    }
    GLuint circleVAO = createPolylineVAO(circle);

    // --------- Tick marks ----------
    std::vector<GLuint> tickVAOs;
    for (int i=0; i<60; i++) {
        float a = i * PI/30.0f;
        float r1 = (i % 5 == 0 ? 0.70f : 0.75f);
        float r2 = 0.8f;

        float x1 = cos(a)*r1, y1 = sin(a)*r1;
        float x2 = cos(a)*r2, y2 = sin(a)*r2;

        tickVAOs.push_back(createLineVAO(x1,y1,x2,y2));
    }

    glLineWidth(3.0f);

    while (!glfwWindowShouldClose(w)) {
        glClearColor(0.07f,0.07f,0.1f,1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Identity matrix
        float mat[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
        glUniformMatrix4fv(locTransform, 1, GL_FALSE, mat);

        // ------------ Draw circle ------------
        glUniform3f(locColor, 0.9f, 0.9f, 0.9f);
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_LINE_LOOP, 0, 360);

        // ------------ Draw tick marks --------
        glLineWidth(2.0f);
        for (int i=0; i<60; i++) {
            if (i % 5 == 0) glUniform3f(locColor, 1.0f, 1.0f, 1.0f);
            else            glUniform3f(locColor, 0.7f, 0.7f, 0.7f);

            glBindVertexArray(tickVAOs[i]);
            glDrawArrays(GL_LINES, 0, 2);
        }

        // ------------ Time ------------
        std::time_t t = std::time(nullptr);
        std::tm lt = *std::localtime(&t);
        float sec  = lt.tm_sec;
        float min  = lt.tm_min + sec/60.0f;
        float hour = lt.tm_hour % 12 + min/60.0f;

        // ------------ Hand angles ------------
        float ang_s = sec  * PI/30.0f - PI/2;
        float ang_m = min  * PI/30.0f - PI/2;
        float ang_h = hour * PI/6.0f  - PI/2;

        // ------------ Draw hands ------------
        auto drawHand = [&](float angle, float length, float r, float g, float b, float w){
            float x = cos(angle)*length;
            float y = -sin(angle)*length;

            GLuint vao = createLineVAO(0,0,x,y);
            glLineWidth(w);
            glUniform3f(locColor,r,g,b);
            glBindVertexArray(vao);
            glDrawArrays(GL_LINES,0,2);
            glDeleteVertexArrays(1,&vao);
        };

        drawHand(ang_h, 0.45f, 1,1,1, 6);       // hour hand
        drawHand(ang_m, 0.65f, 1,1,1, 4);       // minute hand
        drawHand(ang_s, 0.75f, 1,0,0, 2);       // second hand

        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwDestroyWindow(w);
    glfwTerminate();
}
