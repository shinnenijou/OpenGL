#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <chrono>

#include "fileSystem.h"
#include "shader.h"
#include "math.hpp"

constexpr int FRAMERATE = 60;
constexpr int UPDATE_INTERVAL = (int)(1000 / FRAMERATE);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1; 

    /* NSGL: The targeted version of macOS only supports forward-compatible core profile contexts for OpenGL 3.2 and above */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR" << std::endl;
    }

    float positions[] = {
        -0.5f, -0.5f,   // 0
         0.5f, -0.5f,   // 1
         0.5f,  0.5f,   // 2
        -0.5f,  0.5f,   // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    unsigned int vertexBuffer, vertexArray;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    /* Why do we need vertex array in macOS? */
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ShaderProgramSource shaderSource = parseShader(read_text("res/shaders/basic.shader"));
    unsigned int shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    auto timer = std::chrono::steady_clock::now();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        auto now = std::chrono::steady_clock::now();
        if ((std::chrono::duration_cast<std::chrono::milliseconds>(now - timer).count() > UPDATE_INTERVAL))
        {
            timer = now;

            for (int i = 0; i < 4; ++i)
            {
                Matrix<float> vec2 = rotateVector2(
                    Matrix<float>(1, 2, { positions[2 * i], positions[2 * i + 1] }),
                    Matrix<float>(1, 2, { 0, 0 }),
                    M_PI / 180
                );

                positions[2 * i] = vec2[0][0];
                positions[2 * i + 1] = vec2[0][1];
            }

            glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
         
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}