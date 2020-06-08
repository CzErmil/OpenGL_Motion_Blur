#include "TestModel.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "functional"

namespace test {
    
    TestModel::TestModel() :
        m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelTranslationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_DeltaTime(0),
        m_Camera(&m_DeltaTime)
	{
        static int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;

        const float data[]{
            -1.0f, -1.0f,  1.0f, 0.0f,  0.5f,   ////1    WALLS    0.0f,  0.0f,  1.0f,
             1.0f,  1.0f,  1.0f, 0.25f, 0.75f,    //              0.0f,  0.0f,  1.0f,
            -1.0f,  1.0f,  1.0f, 0.0f,  0.75f,    //              0.0f,  0.0f,  1.0f,
            -1.0f, -1.0f,  1.0f, 0.0f,  0.5f,     //              0.0f,  0.0f,  1.0f,
             1.0f, -1.0f,  1.0f, 0.25f, 0.5f,     //              0.0f,  0.0f,  1.0f,
             1.0f,  1.0f,  1.0f, 0.25f, 0.75f,    //              0.0f,  0.0f,  1.0f,
             1.0f, -1.0f,  1.0f, 0.25f, 0.5f,     //2             1.0f,  0.0f,  0.0f,
             1.0f,  1.0f, -1.0f, 0.5f,  0.75f,    //              1.0f,  0.0f,  0.0f,
             1.0f,  1.0f,  1.0f, 0.25f, 0.75f,    //              1.0f,  0.0f,  0.0f,
             1.0f, -1.0f,  1.0f, 0.25f, 0.5f,     //              1.0f,  0.0f,  0.0f,
             1.0f, -1.0f, -1.0f, 0.5f,  0.5f,     //              1.0f,  0.0f,  0.0f,
             1.0f,  1.0f, -1.0f, 0.5f,  0.75f,    //              1.0f,  0.0f,  0.0f,
             1.0f, -1.0f, -1.0f, 0.5f,  0.5f,     //3             0.0f,  0.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, 0.75f, 0.75f,    //              0.0f,  0.0f, -1.0f,
             1.0f,  1.0f, -1.0f, 0.5f,  0.75f,    //              0.0f,  0.0f, -1.0f,
             1.0f, -1.0f, -1.0f, 0.5f,  0.5f,     //              0.0f,  0.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, 0.75f, 0.5f,     //              0.0f,  0.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, 0.75f, 0.75f,    //              0.0f,  0.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, 0.75f, 0.5f,     //4            -1.0f,  0.0f,  0.0f,
            -1.0f,  1.0f,  1.0f, 1.0f,  0.75f,    //             -1.0f,  0.0f,  0.0f,
            -1.0f,  1.0f, -1.0f, 0.75f, 0.75f,    //             -1.0f,  0.0f,  0.0f,
            -1.0f, -1.0f, -1.0f, 0.75f, 0.5f,     //             -1.0f,  0.0f,  0.0f,
            -1.0f, -1.0f,  1.0f, 1.0f,  0.5f,     //             -1.0f,  0.0f,  0.0f,
            -1.0f,  1.0f,  1.0f, 1.0f,  0.75f,  ////             -1.0f,  0.0f,  0.0f,
            -1.0f,  1.0f,  1.0f, 0.0f,  0.75f,  ////    TOP       0.0f,  1.0f,  0.0f,
             1.0f,  1.0f, -1.0f, 0.25f, 1.0f,     //              0.0f,  1.0f,  0.0f,
            -1.0f,  1.0f, -1.0f, 0.0f,  1.0f,     //              0.0f,  1.0f,  0.0f,
            -1.0f,  1.0f,  1.0f, 0.0f,  0.75f,    //              0.0f,  1.0f,  0.0f,
             1.0f,  1.0f,  1.0f, 0.25f, 0.75f,    //              0.0f,  1.0f,  0.0f,
             1.0f,  1.0f, -1.0f, 0.25f, 1.0f,   ////              0.0f,  1.0f,  0.0f,
            -1.0f, -1.0f,  1.0f, 0.0f,  0.25f,  ////    BOTTOM    0.0f, -1.0f,  0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f,  0.5f,     //              0.0f, -1.0f,  0.0f,
             1.0f, -1.0f, -1.0f, 0.25f, 0.5f,     //              0.0f, -1.0f,  0.0f,
            -1.0f, -1.0f,  1.0f, 0.0f,  0.25f,    //              0.0f, -1.0f,  0.0f,
             1.0f, -1.0f, -1.0f, 0.25f, 0.5f,     //              0.0f, -1.0f,  0.0f,
             1.0f, -1.0f,  1.0f, 0.25f, 0.25f   ////              0.0f, -1.0f,  0.0f,
        };//36

        m_VAO = std::make_unique<VertexArrayObject>();
        m_VAO->Bind();

        m_VertexBaffer = std::make_unique<VertexBuffer>(&data, sizeof(data));
        
        VertexBufferLayout layout;

        layout.Push<GLfloat>(3);
        //layout.Push<GLfloat>(3);
        layout.Push<GLfloat>(2);

        m_VAO->AddVertexBuffer(*m_VertexBaffer, layout);

        Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VertexShader_texture.shader");
        Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/FragmentShader_texture.shader");

        m_Program = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        m_Program->UseProgram();

        m_Texture = std::make_unique<Texture>("res/textures/grass.png");
        m_Texture->Bind(0);

        m_Program->SetUniform4f("u_color", 0.3f, 0.1f, 0.9f, 1.0f);
        m_Program->SetUniform1i("u_texture", 0);

        m_Proj = m_Camera.getProj();

        m_LastTime = glfwGetTime();
        
        std::function<void(int)> w = std::bind(&Camera::setForward, &m_Camera, std::placeholders::_1);
        std::function<void(int)> s = std::bind(&Camera::setBackward, &m_Camera, std::placeholders::_1);
        std::function<void(int)> a = std::bind(&Camera::setLeft, &m_Camera, std::placeholders::_1);
        std::function<void(int)> d = std::bind(&Camera::setRight, &m_Camera, std::placeholders::_1);
        std::function<void(int)> mouseMiddleButton = std::bind(&Camera::setMouseMiddleButton, &m_Camera, std::placeholders::_1);
        std::function<void(double, double, double, double)>  cursor = std::bind(&Camera::processMouseMovment, &m_Camera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        m_Inputs.setWSADKeyCallbacks(w, s, a, d);
        m_Inputs.setMouseButtonCallbacks(GLFW_MOUSE_BUTTON_3,mouseMiddleButton);
        m_Inputs.setCursorCullbuck(cursor);
	}

    TestModel::~TestModel()
	{

	}

	void TestModel::OnUpdate(float deltaTime)
	{
        m_CurentTime = glfwGetTime();
        m_DeltaTime = m_CurentTime - m_LastTime;
        m_LastTime = m_CurentTime;

        m_View = m_Camera.getView();

        m_ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]));

        m_ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationXYZ[0]), glm::vec3(1, 0, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[1]), glm::vec3(0, 1, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[2]), glm::vec3(0, 0, 1));

        m_ModelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(m_ModelScalingXYZ[0], m_ModelScalingXYZ[1], m_ModelScalingXYZ[2]));

        m_Model = glm::mat4(1.0f) * m_ModelTranslation * m_ModelRotation * m_ModelScaling;
        m_MVP = m_Proj * m_View * m_Model;
	}

	void TestModel::OnRender()
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Program->SetUniformMatrix4fv("u_MVP", m_MVP);

        glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void TestModel::OnImGuiRender()
	{
       {
            ImGui::SliderFloat3("Model translation", m_ModelTranslationXYZ, -10.0f, 10.0f, "%.1f", 2.0f);
            ImGui::SliderFloat3("Model rotation", m_ModelRotationXYZ, -180.0f, 180.0f, "%.0f", 1.0f);
            ImGui::SliderFloat3("Model scaling", m_ModelScalingXYZ, -3.0f, 3.0f, "%.1f", 2.0f);
            if (ImGui::Button("Reset"))
            {
                m_ModelRotationXYZ[0] = m_ModelRotationXYZ[1] = m_ModelRotationXYZ[2] = 0.0f;
                m_ModelTranslationXYZ[0] = m_ModelTranslationXYZ[1] = m_ModelTranslationXYZ[2] = 0.0f;
                m_ModelScalingXYZ[0] = m_ModelScalingXYZ[1] = m_ModelScalingXYZ[2] = 1.0f;
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
	}
}
