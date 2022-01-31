#include "TestDepthBuffer.h"

#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {
    
    TestDepthBuffer::TestDepthBuffer() :
        m_ModelRotationXYZ{ 0.0f, 0.0f,0.0f },
        m_ModelTranslationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelScalingXYZ{ 1.0f,1.0f,1.0f }
	{
        static int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;
        glEnable(GL_DEPTH_TEST);
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
             1.0f, -1.0f, -1.0f, 0.25f, 0.5f,     //              0.0f, -1.0f,  0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f,  0.5f,     //              0.0f, -1.0f,  0.0f,
            -1.0f, -1.0f,  1.0f, 0.0f,  0.25f,    //              0.0f, -1.0f,  0.0f,
             1.0f, -1.0f,  1.0f, 0.25f, 0.25f,    //              0.0f, -1.0f,  0.0f,
             1.0f, -1.0f, -1.0f, 0.25f, 0.5f    ////              0.0f, -1.0f,  0.0f,
        };//36

        m_VAO = std::make_unique<VertexArrayObject>();
        m_VAO->Bind();

        m_VertexBuffer = std::make_unique<VertexBuffer>(&data, sizeof(data));
        
        VertexBufferLayout layout;

        layout.Push<GLfloat>(3);
        //layout.Push<GLfloat>(3);
        layout.Push<GLfloat>(2);

        m_VAO->AddVertexBuffer(*m_VertexBuffer, layout);

        Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VertexShader.shader");
        Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/FragmentShader.shader");

        m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        m_Program[0]->UseProgram();
        m_Program[0]->SetUniform4f("u_color", 0.3f, 0.1f, 0.9f, 1.0f);

        Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/VertexShader_DepthBuffer.shader");
        Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/FragmentShader_DepthBuffer.shader");
        m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

        m_Proj = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 1.0f, 150.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)); 
    
        m_FrameBuffer = std::make_unique<FrameBuffer>(Attachment::DEPTH, 1600, 900);
	}

    TestDepthBuffer::~TestDepthBuffer()
	{

	}

	void TestDepthBuffer::OnUpdate(double deltaTime)
	{
        m_ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]));

        m_ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationXYZ[0]), glm::vec3(1, 0, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[1]), glm::vec3(0, 1, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[2]), glm::vec3(0, 0, 1));

        m_ModelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(m_ModelScalingXYZ[0], m_ModelScalingXYZ[1], m_ModelScalingXYZ[2]));

        m_Model = glm::mat4(1.0f) * m_ModelTranslation * m_ModelRotation * m_ModelScaling;
        m_MVP = m_Proj * m_View * m_Model;
	}

	void TestDepthBuffer::OnRender()
	{       
        m_Program[0]->UseProgram();
        m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
        
        m_FrameBuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_FrameBuffer->Unbind();
        
        m_FrameBuffer->BindTexture();
        m_Program[1]->UseProgram();
        m_Program[1]->SetUniform1i("u_texture",0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void TestDepthBuffer::OnImGuiRender()
	{
       {
            ImGui::SliderFloat3("Model translation", m_ModelTranslationXYZ, -2.0f, 2.0f, "%.1f", 2.0f);
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
