#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp> 

#include <iostream>

#include "tests/TestModel.h"
#include "tests/AccumulateMotionBlur.h"
#include "tests/CameraMotionBlur.h"
#include "tests/PerObjectMotionBlur.h"
#include "tests/MultiLayerMotionBlur.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), message);
}

int main(void)
{
    static int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 410";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Motion Blur", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            glViewport(0,0,width,height);
            WINDOW_WIDTH = width;
            WINDOW_HEIGHT = height;
        });

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW has not been inintialized\n";
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestModel>("Test Model");
    testMenu->RegisterTest<test::AccumulateMotionBlur>("Accumulate Motion Blur");
    testMenu->RegisterTest<test::CameraMotionBlur>("Camera Motion Blur");
    testMenu->RegisterTest<test::PerObjectMotionBlur>("Per-object Motion Blur");
    testMenu->RegisterTest<test::MultiLayerMotionBlur>("Multi-Layer Motion Blur");

    double curentTime, lastTime, deltaTime;
    lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        curentTime = glfwGetTime();
        deltaTime = curentTime - lastTime;
        lastTime = curentTime;

        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime);
            currentTest->OnRender();
            ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove);
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest=testMenu;
            }
            glDisable(GL_DEBUG_OUTPUT);
            currentTest->OnImGuiRender();
            ImGui::End();
            
        }
        glDisable(GL_DEBUG_OUTPUT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEBUG_OUTPUT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete currentTest;
    if (currentTest != testMenu)
        delete testMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}