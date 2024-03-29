#include "AccumulateMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <functional>

namespace test {

	AccumulateMotionBlur::AccumulateMotionBlur()
	{
		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/DirLight_fragment.glsl");

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

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniform1i("u_texture", 0);

		m_FrameBuffer[0] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[0]->SetDepthBuffer();

		m_FrameBuffer[1] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	AccumulateMotionBlur::~AccumulateMotionBlur()
	{
		glfwSwapInterval(1);

		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void AccumulateMotionBlur::OnUpdate(double deltaTime)
	{
		m_DeltaTime = deltaTime;

		if (m_SphereChanged)
		{
			UpdateSphere();
		}
	}

	void AccumulateMotionBlur::calculatePosition(double deltaTime)
	{
		ObjectTest::calculatePosition(deltaTime);

		if (m_PredefinedCameraMovement)
		{
			PredefinedCameraMovement(deltaTime);
		}

		m_Sphere->setPosition(m_ModelTranslation[0], m_ModelTranslation[1], m_ModelTranslation[2]);
		m_Sphere->setRotation(m_ModelRotation[0], m_ModelRotation[1], m_ModelRotation[2]);
		m_Sphere->setScale(m_ModelScale[0], m_ModelScale[1], m_ModelScale[2]);

		m_View = m_Camera.getView();
		m_Model = m_Sphere->getModelMatrix();

		m_MVP = m_Proj * m_View * m_Model;
	}

	void AccumulateMotionBlur::OnRender()
	{
		m_FrameBuffer[1]->Bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		m_FrameBuffer[1]->Unbind();

		int count = m_MotionBlurLevel;
		for (int i = 1; i <= count; i++)
		{
			m_FrameBuffer[0]->Bind();

			glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			auto dt = m_DependendOnFPS ? m_DeltaTime : m_ConstTime;

			if (i == 1)
			{
				calculatePosition(dt - (dt * m_MotionBlurPower));
			}

			calculatePosition(dt * m_MotionBlurPower / count);

			m_FrameBuffer[0]->BindTexture(1);
			m_Texture->Bind(0);
			m_Program[0]->UseProgram();
			m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
			m_Program[0]->SetUniformMatrix4fv("u_Model", m_Model);

			m_Program[0]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

			m_Program[0]->SetUniform1i("u_texture", 0);

			m_Program[0]->SetUniform3fv("material.specular", m_Material.specular);
			m_Program[0]->SetUniform1f("material.shininess", m_Material.shininess);

			m_Program[0]->SetUniform3fv("light.direction", m_Light.direction);
			m_Program[0]->SetUniform3fv("light.ambient", m_Light.ambient);
			m_Program[0]->SetUniform3fv("light.diffuse", m_Light.diffuse);
			m_Program[0]->SetUniform3fv("light.specular", m_Light.specular);

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

			glBlendFunc(GL_ONE, GL_ONE);

			m_FrameBuffer[1]->BindTexture(2);
			m_Program[1]->UseProgram();
			m_Program[1]->SetUniform1i("u_texture", 1);
			m_Program[1]->SetUniform1f("u_alpha", (double)i * 2 / (double)(count * count + count));

			glDrawArrays(GL_TRIANGLES, 0, 6);

			m_FrameBuffer[1]->Unbind();
		}

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Program[2]->UseProgram();
		m_Program[2]->SetUniform1i("u_texture", 2);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void AccumulateMotionBlur::OnImGuiRender()
	{
		ObjectTest::OnImGuiRender();

		ObjectTest::ImGuiShowMotionBlurSettings();
	}
}
