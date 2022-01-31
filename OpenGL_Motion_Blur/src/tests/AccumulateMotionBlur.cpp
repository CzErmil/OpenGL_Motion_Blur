#include "AccumulateMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <functional>

namespace test {
    
    AccumulateMotionBlur::AccumulateMotionBlur() :
        m_DeltaTime(0),
        m_Camera(&m_DeltaTime),
        m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
        m_ModelTranslationXYZ{ 0.0f,0.0f,-3.0f },
        m_ModelScalingXYZ{ 1.0f,1.0f,1.0f },
        m_SwapInterval(1),
        m_CubeSpeed(3),
        m_MotionBlurLevel(10),
        m_MotionBlurPower(1),
        m_sumTime(0),
        m_Pause(0),
        m_Smooth(1),
        m_Sektors(30),
        m_Stack(30),
        m_SphereChanged(1)
	{
        static int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;

        glLineWidth(2);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_FACE);
        glFrontFace(GL_CCW);

        m_VAO = std::make_unique<VertexArrayObject>();
        m_VAO->Bind();

        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, (6 * 100 * 80 + 3 * 100 * 20) * sizeof(Vertex), GL_DYNAMIC_DRAW);

        VertexBufferLayout layout;

        layout.Push<GLfloat>(3);
        layout.Push<GLfloat>(3);
        layout.Push<GLfloat>(2);

        m_VAO->AddVertexBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = make_unique<IndexBuffer>(nullptr, (6 * 100 * 100), GL_DYNAMIC_DRAW);
        m_IndexBuffer->Bind();

        Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
        Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/TestModel_fragment.glsl");

        m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

        Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
        Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/glAccum.fragment_shader");

        m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

        Shader vertexShader3 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
        Shader fragmentShader3 = Shader(GL_FRAGMENT_SHADER, "src/shaders/RenderTexture.fragment_shader");

        m_Program[2] = std::make_unique<ShaderProgram>(vertexShader3, fragmentShader3);

        Shader vertexShader4 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_vertex.glsl");
        Shader geometryShader4 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_geometry.glsl");
        Shader fragmentShader4 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

        m_Program[3] = std::make_unique<ShaderProgram>(vertexShader4, geometryShader4, fragmentShader4);

        m_Proj = m_Camera.getProj();

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

        m_Texture = std::make_unique<Texture>("res/textures/earth.bmp", GL_LINEAR);
        m_Texture->Bind(0);

        m_Program[0]->UseProgram();
        m_Program[0]->SetUniform1i("u_texture", 0);

        m_FrameBuffer[0] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT);
        m_FrameBuffer[0]->SetDepthBuffer();

        m_FrameBuffer[1] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

    AccumulateMotionBlur::~AccumulateMotionBlur()
	{
        glfwSwapInterval(1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void AccumulateMotionBlur::OnUpdate(double deltaTime)
	{
        m_DeltaTime = deltaTime;
        m_View = m_Camera.getView();

        if (m_SphereChanged) {
            if (m_Smooth)
                m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Smooth);
            else
                m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Flat);

            m_VertexBuffer->UpdateData(m_Sphere->getVertices().data(), m_Sphere->getSize());
            m_IndexBuffer->UpdateBuffer(m_Sphere->getIndecies().data(), m_Sphere->getIndecies().size());
            m_SphereChanged = 0;
        }
	}

    void AccumulateMotionBlur::calculatePosition(double deltaTime)
    {
        m_sumTime += deltaTime * m_CubeSpeed;
        m_ModelTranslationXYZ[0] = sinf(m_sumTime)*3;
        m_ModelTranslationXYZ[1] = cosf(m_sumTime)*3;
        
        m_Sphere->setPosition(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]);
		m_Sphere->rotate(m_ModelRotationXYZ[0] * (float)deltaTime * 3, m_ModelRotationXYZ[1] * (float)deltaTime * 3, m_ModelRotationXYZ[2] * (float)deltaTime * 3);
    }

	void AccumulateMotionBlur::OnRender()
	{
        if(!m_Pause)
        {
            m_FrameBuffer[1]->Bind();
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_FrameBuffer[1]->Unbind();

            int count = m_MotionBlurLevel;
            for (int i = 0; i < count; i++)
            {
                m_FrameBuffer[0]->Bind();

                glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                if(i==0)
                    calculatePosition(-((m_DeltaTime * m_MotionBlurPower) - m_DeltaTime) / 2);

                calculatePosition(m_DeltaTime*m_MotionBlurPower / count);

                m_Model = m_Sphere->getModelMatrix();
                m_MVP = m_Proj * m_View * m_Model;

                m_FrameBuffer[0]->BindTexture(1);
                m_Texture->Bind(0);
                m_Program[0]->UseProgram();
                m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
                m_Program[0]->SetUniformMatrix4fv("u_Model", m_Model);
                m_Program[0]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

                glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

                if (m_DrawLines)
                {
                    m_Program[3]->UseProgram();
                    m_Program[3]->SetUniformMatrix4fv("u_MVP", m_MVP);
                    m_Program[3]->SetUniform3f("u_color", 0.0f, 0.0f, 0.0f);

                    glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
                }

                m_FrameBuffer[0]->Unbind();

                m_FrameBuffer[1]->Bind();

                glClear(GL_DEPTH_BUFFER_BIT);
                glBlendFunc(GL_ONE, GL_ONE);

                m_FrameBuffer[1]->BindTexture(2);
                m_Program[1]->UseProgram();
                m_Program[1]->SetUniform1i("u_texture", 1);
                m_Program[1]->SetUniform1f("u_alpha", 1.0f / count);

                glDrawArrays(GL_TRIANGLES, 0, 6);

                m_FrameBuffer[1]->Unbind();

                if (i == count-1)
                    calculatePosition(-((m_DeltaTime * m_MotionBlurPower) - m_DeltaTime) / 2);
            }
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Program[2]->UseProgram();
        m_Program[2]->SetUniform1i("u_texture", 2);
        glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void AccumulateMotionBlur::OnImGuiRender()
	{
       {
            ImGui::SliderFloat3("Model rotation", m_ModelRotationXYZ, -100.0f, 100.0f, "%.1f", 1.0f);

            ImGui::SliderInt("Motion Blur Level", &m_MotionBlurLevel , 1, 100, "%d");
            
            ImGui::SliderFloat("Motion Blur Power", &m_MotionBlurPower, 1.0f, 3.0f, "%.2f", 1.0f);

            ImGui::SliderFloat("Model Speed", &m_CubeSpeed, 0, 10, "%.1f", 1.0f);

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

            if (ImGui::SliderInt("Sphere secttors", &m_Sektors, 3, 100, "%d")) m_SphereChanged = 1;
            if (ImGui::SliderInt("Sphere stack", &m_Stack, 3, 100, "%d")) m_SphereChanged = 1;

            ImGui::Text("Draw lines?");

            if (ImGui::RadioButton("No", m_DrawLines == 0))
            {
                m_DrawLines = 0;
            } ImGui::SameLine();

            if (ImGui::RadioButton("Yes", m_DrawLines == 1))
            {
                m_DrawLines = 1;
            }

            if (ImGui::RadioButton("Smooth", m_Smooth == 1))
            {
                m_Smooth = 1;
                m_SphereChanged = 1;
            } ImGui::SameLine();

            if (ImGui::RadioButton("Flat", m_Smooth == 0))
            {
                m_Smooth = 0;
                m_SphereChanged = 1;
            }


            if (ImGui::Button("Reset"))
            {
                m_ModelRotationXYZ[0] = m_ModelRotationXYZ[1] = m_ModelRotationXYZ[2] = 0.0f;
                m_Sphere->setRotation(0, 0, 0);
            }            

            ImGui::Checkbox("Pause",&m_Pause);       

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }
	}
}
