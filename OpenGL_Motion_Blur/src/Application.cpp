#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBuffer.h"
#include "ShaderProgram.h"

int main(void)
{
    static int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW has not been inintialized\n";
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

    const float data[6]{
        -0.25, -0.25,
        0.25, -0.25,
        0.25, 0.25
    };

    VertexBuffer vertexBaffer = VertexBuffer(&data, sizeof(data));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    vertexBaffer.UnBind();

    Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/BasicVertexShader.shader");
    Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/BasicFragmentShader.shader");
    
    ShaderProgram program = ShaderProgram(vertexShader, fragmentShader);
    program.UseProgram();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}