#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <vector>

#include "renderer.h"
#include "math.hpp"

#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "texture.h"

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
    {
        std::vector<float> positions = {
            -0.5f, -0.5f, 0.0f, 0.0f,  // 0
             0.5f, -0.5f, 1.0f, 0.0f,  // 1
             0.5f,  0.5f, 1.0f, 1.0f,  // 2
            -0.5f,  0.5f, 0.0f, 1.0f,  // 3
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0,
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexBuffer vb(&positions[0], positions.size() * sizeof(decltype(positions)::value_type));
        IndexBuffer ib(&indices[0], indices.size());

        /* VertexArray is generated with default index 0 in compatibility profile but isn't in core profile
         * thus we must generate vertex array manually here
         */
        VertexArray va;
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va.addBuffer(vb, layout);

        Shader shader("res/shaders/basic.shader");
        shader.bind();
        shader.setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        Texture texture("res/textures/logo.png");
        texture.bind();
        shader.setUniform1i("u_Texture", 0);

        Renderer renderer;

        /* initial position */
        Matrix<float> pivot(1, 2, { 0.5f, 0 });

        /* initial color */
        float r = 0.8f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* transit color */
            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }
            r += increment;

            shader.bind();
            shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            /* Render here */
            renderer.clear();
            renderer.draw(va, ib, shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}