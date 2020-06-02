#include "TestMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {
    
    TestMotionBlur::TestMotionBlur() :
        m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelTranslationXYZ{ 0.0f,0.0f,-3.0f },
        m_ModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_PrevModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_PrevModelTranslationXYZ{ 0.0f,0.0f,-3.0f },
        m_PrevModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_CurrentTime(0),
        m_SwapInterval(1),
        m_CubeSpeed(3),
        m_MotionBlurLevel(8)
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

        m_VAO = std::make_unique<VertexArrayObject>();
        m_VAO->Bind();

        m_VertexBaffer = std::make_unique<VertexBuffer>(&data, sizeof(data));
        
        VertexBufferLayout layout;

        layout.Push<GLfloat>(3);
        //layout.Push<GLfloat>(3);
        layout.Push<GLfloat>(2);

        m_VAO->AddVertexBuffer(*m_VertexBaffer, layout);

        Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/TestMotionBlur_Vector.shader");
        Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/TestMotionBlur_Fragment.shader");

        m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
        Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/RenderTexture.fragment_shader");

        m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

        m_Proj = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 1.0f, 150.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)); 

        m_Texture = std::make_unique<Texture>("res/textures/grass.png");
        m_Texture->Bind(0);

        m_Program[0]->UseProgram();
        m_Program[0]->SetUniform1i("u_texture", 0);

        m_CurrentTime = glfwGetTime();

        m_PrevModelTranslationXYZ[0] = sin(m_CurrentTime * m_CubeSpeed) * 3;
        m_PrevModelTranslationXYZ[1] = cos(m_CurrentTime * m_CubeSpeed) * 3;

        /*m_ColorBufferFBO = 0;
        glGenFramebuffers(1, &m_ColorBufferFBO);

        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &m_ColorBuffer);

        glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1600, 900, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, m_ColorBufferFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);

        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1600, 900);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	}

    TestMotionBlur::~TestMotionBlur()
	{
        glfwSwapInterval(1);
        glDeleteTextures(1, &m_ColorBuffer);
        glDeleteFramebuffers(1, &m_ColorBufferFBO);
	}

	void TestMotionBlur::OnUpdate(float deltaTime)
	{

	}

    float split(float a, float b, int iteration, int count)
    {
        return b + (iteration* (a-b) / count);
    }

	void TestMotionBlur::OnRender()
	{
        m_CurrentTime = glfwGetTime();
        
        m_ModelTranslationXYZ[0] = sin(m_CurrentTime * m_CubeSpeed)*3;
        m_ModelTranslationXYZ[1] = cos(m_CurrentTime * m_CubeSpeed)*3;

        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);
        //glBindFramebuffer(GL_FRAMEBUFFER, m_ColorBufferFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int count = m_MotionBlurLevel;
        for (int i = 0; i < count; i++)
        {
            m_ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(
                split(m_ModelTranslationXYZ[0], m_PrevModelTranslationXYZ[0], i, count), 
                split(m_ModelTranslationXYZ[1], m_PrevModelTranslationXYZ[1], i, count), 
                split(m_ModelTranslationXYZ[2], m_PrevModelTranslationXYZ[2], i, count)));

            m_ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(split(m_ModelRotationXYZ[0], m_PrevModelRotationXYZ[0], i, count)), glm::vec3(1, 0, 0));
            m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(split(m_ModelRotationXYZ[1], m_PrevModelRotationXYZ[1], i, count)), glm::vec3(0, 1, 0));
            m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(split(m_ModelRotationXYZ[2], m_PrevModelRotationXYZ[2], i, count)), glm::vec3(0, 0, 1));

            m_ModelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(
                split(m_ModelScalingXYZ[0], m_PrevModelScalingXYZ[0], i, count),
                split(m_ModelScalingXYZ[1], m_PrevModelScalingXYZ[1], i, count),
                split(m_ModelScalingXYZ[2], m_PrevModelScalingXYZ[2], i, count)));

            m_Model = glm::mat4(1.0f) * m_ModelTranslation * m_ModelRotation * m_ModelScaling;
            m_MVP = m_Proj * m_View * m_Model;

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
            m_Texture->Bind(0);
            m_Program[0]->UseProgram();
            m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
            m_Program[0]->SetUniform1f("u_alpha", 0.05f + 0.6f / glm::abs(count/2-i));
            m_Program[0]->SetUniform1i("u_texture", 0);
            //m_Program[0]->SetUniform1i("u_accumulateTexture", 1);

            glClear(GL_DEPTH_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
       /* glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
        m_Program[1]->UseProgram();
        m_Program[1]->SetUniform1i("u_texture", 1);
        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);*/


        m_PrevModelTranslationXYZ[0] = m_ModelTranslationXYZ[0];
        m_PrevModelTranslationXYZ[1] = m_ModelTranslationXYZ[1];
        m_PrevModelTranslationXYZ[2] = m_ModelTranslationXYZ[2];

        m_PrevModelRotationXYZ[0] = m_ModelRotationXYZ[0];
        m_PrevModelRotationXYZ[1] = m_ModelRotationXYZ[1];
        m_PrevModelRotationXYZ[2] = m_ModelRotationXYZ[2];
        
        m_PrevModelScalingXYZ[0] = m_ModelScalingXYZ[0];
        m_PrevModelScalingXYZ[1] = m_ModelScalingXYZ[1];
        m_PrevModelScalingXYZ[2] = m_ModelScalingXYZ[2];
	}

	void TestMotionBlur::OnImGuiRender()
	{
       {
            ImGui::SliderFloat3("Model rotation", m_ModelRotationXYZ, -180.0f, 180.0f, "%.0f", 1.0f);
            ImGui::SliderFloat3("Model scaling", m_ModelScalingXYZ, -3.0f, 3.0f, "%.1f", 2.0f);

            ImGui::SliderInt("Motion Blur Level", &m_MotionBlurLevel , 3, 100, "%d");
            
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

            ImGui::SliderFloat("Cube Speed", &m_CubeSpeed, 0.1, 10, "%.1f", 2.0f);

            if (ImGui::Button("Reset"))
            {
                m_ModelRotationXYZ[0] = m_ModelRotationXYZ[1] = m_ModelRotationXYZ[2] = 0.0f;
                m_ModelScalingXYZ[0] = m_ModelScalingXYZ[1] = m_ModelScalingXYZ[2] = 1.0f;
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
	}
}