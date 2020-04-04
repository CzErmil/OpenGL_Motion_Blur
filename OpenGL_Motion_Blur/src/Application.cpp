#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp> 

#include <iostream>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Texture.h"

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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

    const float data[]{
        -1.0f, -1.0f,  1.0f, 0.0f,  0.5f,   ////1    WALLS     0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f, 0.25f, 0.75f,  //              0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f,  0.75f,  //              0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f,  0.5f,   //              0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f, 0.25f, 0.5f,   //              0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f, 0.25f, 0.75f,  //              0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f, 0.25f, 0.5f,     //2              1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f, 0.5f,  0.75f,    //              1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f, 0.25f, 0.75f,    //              1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f, 0.25f, 0.5f,     //              1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f, 0.5f,  0.5f,     //              1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f, 0.5f,  0.75f,    //              1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f, 0.5f,  0.5f,     //3              0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, 0.75f, 0.75f,    //              0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f, 0.5f,  0.75f,    //              0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f, 0.5f,  0.5f,     //              0.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, 0.75f, 0.5f,     //              0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, 0.75f, 0.75f,    //              0.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, 0.75f, 0.5f,     //4             -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, 1.0f,  0.75f,    //             -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, 0.75f, 0.75f,    //             -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, 0.75f, 0.5f,     //             -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f, 1.0f,  0.5f,     //             -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, 1.0f,  0.75f,  ////             -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, 0.0f,  0.75f,   ////    TOP       0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f, 0.25f, 1.0f,    //              0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, 0.0f,  1.0f,    //              0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, 0.0f,  0.75f,     //              0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f, 0.25f, 0.75f,     //              0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f, 0.25f, 1.0f,  ////              0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f, 0.0f,  0.25f,   ////    BOTTOM    0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f, 0.25f, 0.5f,    //              0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, 0.0f,  0.5f,    //              0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f, 0.0f,  0.25f,     //              0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f, 0.25f, 0.25f,     //              0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f, 0.25f, 0.5f  ////              0.0f, -1.0f,  0.0f,
    };//36

   /*const float data[]{
       -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
       -1.0f,  1.0f, 0.0f, 1.0f,
       -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
    };*/

    VertexArrayObject vao;
    vao.Bind();

    VertexBuffer vertexBaffer = VertexBuffer(&data, sizeof(data));

    VertexBufferLayout layout;

    layout.Push<GLfloat>(3);
    //layout.Push<GLfloat>(3);
    layout.Push<GLfloat>(2);

    vao.AddVertexBuffer(vertexBaffer, layout);

    Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VertexShader_texture.shader");
    Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/FragmentShader_texture.shader");
    
    ShaderProgram program = ShaderProgram(vertexShader, fragmentShader);
    program.UseProgram();

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 1.0f, 150.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model_translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 model_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(1, 0, 0));
    model_rotation = glm::rotate(model_rotation, glm::radians(45.0f), glm::vec3(0, 1, 0));
    glm::mat4 model_scaling = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));

    model = model * model_translation *model_rotation* model_scaling;

    glm::mat4 MVP = projection * view * model;

    Texture texture = Texture("res/textures/grass.png");
    texture.Bind(0);

    program.SetUniformMatrix4fv("u_MVP", MVP);
    program.SetUniform4f("u_color", 0.3f, 0.1f, 0.9f, 1.0f);
    program.SetUniform1i("u_texture", 0);
    
        while (GLenum error = glGetError())
        {
            std::cout << "OPENGL ERROR: " << error << std::endl;
        }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}