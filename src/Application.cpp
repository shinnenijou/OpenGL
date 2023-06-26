
#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

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

        Texture texture("res/textures/logo.png");
        shader.setUniform1i("u_Texture", 0);
        shader.setUniformMat4f("u_MVP", proj);

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            shader.bind();
            texture.bind();

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