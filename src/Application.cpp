#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>

#include "fileSystem.h"
#include "shader.h"
#include "math.hpp"
#include "wrapper.h"

#define PI 3.14159265358979323846

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
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

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
    GLCall(glGenBuffers(1, &vertexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    /* Why do we need vertex array in macOS? */
    GLCall(glGenVertexArrays(1, &vertexArray));
    GLCall(glBindVertexArray(vertexArray));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    // index buffer
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    ShaderProgramSource shaderSource = parseShader(read_text("res/shaders/basic.shader"));
    unsigned int shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int color_loc = glGetUniformLocation(shader, "u_Color"));
    ASSERT(color_loc != -1);
    GLCall(glUniform4f(color_loc, 1.0f, 0.0f, 0.0f, 1.0f));
    
    /* initial position */
    Matrix<float> pivot(1, 2, { 0.5f, 0 });

    /* initial color */
    float r = 0.8f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        /* transit position */
        for (int i = 0; i < 4; ++i)
        {
            Matrix<float> vec2 = rotateVector2(
                Matrix<float>(1, 2, { positions[2 * i], positions[2 * i + 1] }),
                Matrix<float>(1, 2, { 0, 0 }),
                PI / 180
            );

            positions[2 * i] = vec2[0][0];
            positions[2 * i + 1] = vec2[0][1];
        }

        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

        /* transit color */
        if (r > 1.0f)
        {
            increment = - 0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }
        r += increment;

        GLCall(glUniform4f(color_loc, r, 0.2f, 0.3f, 1.0f));

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
         
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