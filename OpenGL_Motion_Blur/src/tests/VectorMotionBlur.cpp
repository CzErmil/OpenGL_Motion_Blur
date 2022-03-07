#include "VectorMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {
    
    VectorMotionBlur::VectorMotionBlur() :
        m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelTranslationXYZ{ 0.0f,0.0f,-3.0f },
        m_ModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_CurrentTime(0),
        m_MotionBlurLevel(8),
        m_BlurVector{ 0.0, 0.0 }
	{
        int WINDOW_WIDTH, WINDOW_HEIGHT;
        glfwGetWindowSize(glfwGetCurrentContext(), &WINDOW_WIDTH, &WINDOW_HEIGHT);
        glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_FALSE);

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

        m_VertexBuffer = std::make_unique<VertexBuffer>(&data, sizeof(data));
        
        VertexBufferLayout layout;

        layout.Push<GLfloat>(3);
        layout.Push<GLfloat>(2);

        m_VAO->AddVertexBuffer(*m_VertexBuffer, layout);

        Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VertexShader_texture.shader");
        Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/FragmentShader_texture.shader");

        m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture_VectroMotionBlur.vertex_shader");
        Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/RenderTexture_VectroMotionBlur.fragment_shader");

        m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

        m_Proj = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 1.0f, 150.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

        m_ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]));

        m_ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationXYZ[0]), glm::vec3(1, 0, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[1]), glm::vec3(0, 1, 0));
        m_ModelRotation = glm::rotate(m_ModelRotation, glm::radians(m_ModelRotationXYZ[2]), glm::vec3(0, 0, 1));

        m_ModelScaling = glm::scale(glm::mat4(1.0f), glm::vec3(m_ModelScalingXYZ[0], m_ModelScalingXYZ[1], m_ModelScalingXYZ[2]));

        m_Model = glm::mat4(1.0f) * m_ModelTranslation * m_ModelRotation * m_ModelScaling;
        m_MVP = m_Proj * m_View * m_Model;

        m_Texture = std::make_unique<Texture>("res/textures/grass.png");
        m_Texture->Bind(0);

        m_Program[0]->UseProgram();
        m_Program[0]->SetUniform4f("u_color", 0.3f, 0.1f, 0.9f, 1.0f);
        m_Program[0]->SetUniform1i("u_texture", 0);

        m_CurrentTime = glfwGetTime();

        m_ColorBufferFBO = 0;
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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

    VectorMotionBlur::~VectorMotionBlur()
	{
        glfwSwapInterval(1);

        glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);

        glDeleteTextures(1, &m_ColorBuffer);
        glDeleteFramebuffers(1, &m_ColorBufferFBO);
	}

	void VectorMotionBlur::OnUpdate(double deltaTime)
	{

	}

	void VectorMotionBlur::OnRender()
	{
        m_CurrentTime = glfwGetTime();
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_ColorBufferFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
            m_Texture->Bind(0);
            m_Program[0]->UseProgram();
            m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
            m_Program[0]->SetUniform1i("u_texture", 0);

            glClear(GL_DEPTH_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
        m_Program[1]->UseProgram();
        m_Program[1]->SetUniform1i("u_texture", 1);
        m_Program[1]->SetUniform1i("u_BlurLevel", m_MotionBlurLevel);
        m_Program[1]->SetUniform2f("u_BlurVector", m_BlurVector[0], m_BlurVector[1]);

        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void VectorMotionBlur::OnImGuiRender()
	{
       {
            ImGui::SliderInt("Motion Blur Level", &m_MotionBlurLevel, 3, 100, "%d");

            ImGui::SliderFloat2("Motion Blur Vector", m_BlurVector, -0.5f, 0.5f, "%.2f", 1.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
       }
	}
}
