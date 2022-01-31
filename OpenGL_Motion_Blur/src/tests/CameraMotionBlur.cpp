#include "CameraMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {
    
    CameraMotionBlur::CameraMotionBlur() :
        m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelTranslationXYZ{ 0.0f,0.0f,-3.0f },
        m_ModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_PrevModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_PrevModelTranslationXYZ{ 0.0f,0.0f,-3.0f },
        m_PrevModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_CurrentTime(0),
        m_SwapInterval(1),
        m_CubeSpeed(3),
        m_MotionBlurLevel(8),
        m_MotionBlurPower(1.0f),
        m_Camera(&m_DeltaTime)
	{
        static int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_FACE);
        glFrontFace(GL_CCW);

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

        m_Sphere = make_unique<Sphere>(50,50,2,Surface::Smooth);

        m_VAO = std::make_unique<VertexArrayObject>();
        m_VAO->Bind();

        m_VertexBuffer = std::make_unique<VertexBuffer>(m_Sphere->getData(), m_Sphere->getSize());
        
        m_VAO->AddVertexBuffer(*m_VertexBuffer, m_Sphere->getLayout());

        m_IndexBuffer = make_unique<IndexBuffer>(m_Sphere->getIndecies().data(), m_Sphere->getCount());
        m_IndexBuffer->Bind();

        Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
        Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/TestModel_fragment.glsl");

        m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/CameraMotionBlur_Vertex.glsl");
        Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/CameraMotionBlur_Fragment.glsl");

        m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

        m_Texture = std::make_unique<Texture>("res/textures/earth.bmp", GL_LINEAR);
        m_Texture->Bind(0);

        m_Program[0]->UseProgram();
        m_Program[0]->SetUniform1i("u_texture", 0);

        m_CurrentTime = glfwGetTime();

        m_PrevModelTranslationXYZ[0] = sin((float)m_CurrentTime * m_CubeSpeed) * 3;
        m_PrevModelTranslationXYZ[1] = cos((float)m_CurrentTime * m_CubeSpeed) * 3;

        m_Proj = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 1.0f, 10.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

        m_ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]));

        m_ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationXYZ[0]), glm::vec3(1, 0, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[1]), glm::vec3(0, 1, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[2]), glm::vec3(0, 0, 1));

        m_ModelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(m_ModelScalingXYZ[0], m_ModelScalingXYZ[1], m_ModelScalingXYZ[2]));

        m_Model = glm::mat4(1.0f) * m_ModelTranslation * m_ModelRotation * m_ModelScaling;

        m_Proj = m_Camera.getProj();
        m_View = m_Camera.getView();
        m_PrevMVP = m_Proj * m_View;

        std::function<void(int)> w = std::bind(&Camera::setForward, &m_Camera, std::placeholders::_1);
        std::function<void(int)> s = std::bind(&Camera::setBackward, &m_Camera, std::placeholders::_1);
        std::function<void(int)> a = std::bind(&Camera::setLeft, &m_Camera, std::placeholders::_1);
        std::function<void(int)> d = std::bind(&Camera::setRight, &m_Camera, std::placeholders::_1);
        std::function<void(int)> space = std::bind(&Camera::setUp, &m_Camera, std::placeholders::_1);
        std::function<void(int)> shift = std::bind(&Camera::setDown, &m_Camera, std::placeholders::_1);
        std::function<void(int)> mouseMiddleButton = std::bind(&Camera::setMouseMiddleButton, &m_Camera, std::placeholders::_1);
        std::function<void(double, double, double, double)>  cursor = std::bind(&Camera::processMouseMovment, &m_Camera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        m_Inputs.setWSADKeyCallbacks(w, s, a, d);
        m_Inputs.setSpaceKeyCallback(space);
        m_Inputs.setShiftKeyCallback(shift);
        m_Inputs.setMouseButtonCallbacks(GLFW_MOUSE_BUTTON_3, mouseMiddleButton);
        m_Inputs.setCursorCullbuck(cursor);

        m_FrameBuffer[0] = make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT);
        m_FrameBuffer[0]->SetDepthBuffer();

        m_FrameBuffer[1] = make_unique<FrameBuffer>(Attachment::DEPTH, WINDOW_WIDTH, WINDOW_HEIGHT);

        m_FrameBuffer[0]->BindTexture(1);
        m_FrameBuffer[1]->BindTexture(2);

        m_Program[1]->UseProgram();
        m_Program[1]->SetUniform1i("u_texture", 1);
        m_Program[1]->SetUniform1i("u_depthMap", 2);
	}

    CameraMotionBlur::~CameraMotionBlur()
	{
        glfwSwapInterval(1);
        glDeleteTextures(1, &m_ColorBuffer);
        glDeleteFramebuffers(1, &m_ColorBufferFBO);
	}

	void CameraMotionBlur::OnUpdate(double deltaTime)
	{
        m_DeltaTime = deltaTime;
        m_CurrentTime = glfwGetTime();

        //m_ModelTranslationXYZ[0] = sin(m_CurrentTime * m_CubeSpeed) * 3;
        //m_ModelTranslationXYZ[1] = cos(m_CurrentTime * m_CubeSpeed) * 3;

        m_View = glm::translate(glm::mat4(1.0f), glm::vec3( sin(m_CurrentTime * m_CubeSpeed) * 3 , cos(m_CurrentTime * m_CubeSpeed),-5.0f ));

        m_ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]));

        m_ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationXYZ[0]), glm::vec3(1, 0, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[1]), glm::vec3(0, 1, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[2]), glm::vec3(0, 0, 1));

        m_ModelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(m_ModelScalingXYZ[0], m_ModelScalingXYZ[1], m_ModelScalingXYZ[2]));

        m_Model = glm::mat4(1.0f) * m_ModelTranslation * m_ModelRotation * m_ModelScaling;
        m_MVP = m_Proj * m_View * m_Model;
	}

	void CameraMotionBlur::OnRender()
	{
        m_FrameBuffer[1]->Bind();

        m_Texture->Bind(0);
        m_Program[0]->UseProgram();
        m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
        m_Program[0]->SetUniformMatrix4fv("u_Model", m_Model);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

        m_FrameBuffer[1]->Unbind();

        m_FrameBuffer[0]->Bind();

        m_Texture->Bind(0);
        m_Program[0]->UseProgram();
        m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

        m_FrameBuffer[0]->Unbind();

        m_Program[1]->UseProgram();
        m_Program[1]->SetUniformMatrix4fv("u_invMVP", glm::inverse(m_Proj * m_View));
        m_Program[1]->SetUniformMatrix4fv("u_prevMVP", m_PrevMVP);
        m_Program[1]->SetUniform1i("u_BlurLevel", m_MotionBlurLevel);
        m_Program[1]->SetUniform1f("u_BlurPower", m_MotionBlurPower);

        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        m_PrevMVP = m_Proj * m_View;
	}

	void CameraMotionBlur::OnImGuiRender()
	{
       {
            ImGui::SliderFloat3("Model rotation", m_ModelRotationXYZ, -180.0f, 180.0f, "%.0f", 1.0f);
            ImGui::SliderFloat3("Model scaling", m_ModelScalingXYZ, -3.0f, 3.0f, "%.1f", 2.0f);

            ImGui::SliderInt("Motion Blur Level", &m_MotionBlurLevel , 3, 100, "%d");
            ImGui::SliderFloat("Motion Blur Power", &m_MotionBlurPower, 1.0f, 3.0f, "%.2f", 1.0f);

            if (ImGui::RadioButton("No limit", m_SwapInterval == 0))
            {
                m_SwapInterval = 0;
                glfwSwapInterval(m_SwapInterval);
            } ImGui::SameLine();

            if (ImGui::RadioButton("60fps", m_SwapInterval == 1))
            {
                m_SwapInterval = 1;
                glfwSwapInterval(m_SwapInterval);
            } ImGui::SameLine();

            if (ImGui::RadioButton("30fps", m_SwapInterval == 2))
            {
                m_SwapInterval = 2;
                glfwSwapInterval(m_SwapInterval);
            } ImGui::SameLine();

            if (ImGui::RadioButton("20fps", m_SwapInterval == 3))
            {
                m_SwapInterval = 3;
                glfwSwapInterval(m_SwapInterval);
            } ImGui::SameLine();

            if (ImGui::RadioButton("15fps", m_SwapInterval == 4))
            {
                m_SwapInterval = 4;
                glfwSwapInterval(m_SwapInterval);
            }

            ImGui::SliderFloat("Model Speed", &m_CubeSpeed, 0.1f, 10.0f, "%.1f", 1.0f);

            if (ImGui::Button("Reset"))
            {
                m_ModelRotationXYZ[0] = m_ModelRotationXYZ[1] = m_ModelRotationXYZ[2] = 0.0f;
                m_ModelScalingXYZ[0] = m_ModelScalingXYZ[1] = m_ModelScalingXYZ[2] = 1.0f;
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
	}
}
