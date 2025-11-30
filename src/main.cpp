<<<<<<< HEAD
// Clock Simulation - OpenGL + GLFW (no GLAD)
// Uses legacy immediate-mode for simplicity to avoid a GL loader.

=======
#include <glad/glad.h>
#include <GLFW/glfw3.h>
>>>>>>> b499d98 (Update)
#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
<<<<<<< HEAD
#include <chrono>

#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

// Returns local time components
static void get_local_time(int& outHour, int& outMinute, int& outSecond) {
    using clock = std::chrono::system_clock;
    auto now = clock::to_time_t(clock::now());
    std::tm localTm{};
#if defined(_WIN32)
    localtime_s(&localTm, &now);
#else
    localtime_r(&now, &localTm);
#endif
    outHour = localTm.tm_hour % 12;
    outMinute = localTm.tm_min;
    outSecond = localTm.tm_sec;
}

static void setup_ortho() {
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Ortho in NDC-like space
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // Request an OpenGL context compatible with fixed-function (no core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Clock", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    const int segments = 240;
    const float radius = 0.85f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.06f, 0.07f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        setup_ortho();

        // Clock face (circle)
        glColor3f(0.9f, 0.9f, 0.9f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float t = (float)i / (float)segments * 2.0f * 3.1415926535f;
            glVertex2f(radius * std::cos(t), radius * std::sin(t));
        }
        glEnd();

        // Ticks (minute + hour)
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_LINES);
        for (int i = 0; i < 60; ++i) {
            float t = (float)i / 60.0f * 2.0f * 3.1415926535f;
            float outer = radius;
            float inner = (i % 5 == 0) ? radius - 0.08f : radius - 0.04f;
            float xo = outer * std::cos(t), yo = outer * std::sin(t);
            float xi = inner * std::cos(t), yi = inner * std::sin(t);
            glVertex2f(xi, yi);
            glVertex2f(xo, yo);
        }
        glEnd();

        // Time -> angles (radians)
        int h, m, s;
        get_local_time(h, m, s);
        float secAngle  = (float(s) / 60.0f) * 2.0f * 3.1415926535f;
        float minAngle  = ((float(m) + float(s)/60.0f) / 60.0f) * 2.0f * 3.1415926535f;
        float hourAngle = ((float(h) + float(m)/60.0f) / 12.0f) * 2.0f * 3.1415926535f;

        auto drawHand = [&](float angle, float len, float r, float g, float b, float thickness) {
            glColor3f(r, g, b);
            glLineWidth(thickness);
            float x1 = len * std::sin(angle);
            float y1 = len * std::cos(angle);
            glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(x1, y1);
            glEnd();
        };

        // Hands
        drawHand(hourAngle, 0.50f, 0.95f, 0.95f, 0.95f, 4.0f);
        drawHand(minAngle,  0.70f, 0.95f, 0.95f, 0.95f, 3.0f);
        drawHand(secAngle,  0.78f, 0.90f, 0.25f, 0.25f, 2.0f);

        // Center cap
        glColor3f(0.95f, 0.95f, 0.95f);
        glBegin(GL_LINE_LOOP);
        const int capSegs = 30; const float capR = 0.015f;
        for (int i = 0; i < capSegs; ++i) {
            float t = (float)i / (float)capSegs * 2.0f * 3.1415926535f;
            glVertex2f(capR * std::cos(t), capR * std::sin(t));
        }
        glEnd();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
=======
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
>>>>>>> b499d98 (Update)
}
