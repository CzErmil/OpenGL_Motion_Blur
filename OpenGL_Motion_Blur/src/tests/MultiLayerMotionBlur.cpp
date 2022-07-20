#include "MultiLayerMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "functional"

#include <string>

namespace test {

	MultiLayerMotionBlur::MultiLayerMotionBlur() :
		m_FrameBufferLayers(MAX_FRAME_LAYERS)
	{
		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/MultiLayerMotionBlur_vertex.glsl");
		Shader geometryShader = Shader(GL_GEOMETRY_SHADER, "src/shaders/MultiLayerMotionBlur_geometry.glsl");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/MultiLayerMotionBlur_fragment.glsl");
		
		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, geometryShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_MultiLayers_vertex.glsl");
		Shader geometryShader2 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_MultiLayers_geometry.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, geometryShader2, fragmentShader2);

		Shader vertexShader3 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
		Shader fragmentShader3 = Shader(GL_FRAGMENT_SHADER, "src/shaders/RenderArrayTexture.glsl");

		m_Program[2] = std::make_unique<ShaderProgram>(vertexShader3, fragmentShader3);

		m_Texture = std::make_unique<Texture>("res/textures/earth.bmp", GL_LINEAR);
		m_Texture->Bind(0);

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniform1i("u_texture", 0);

		m_FrameBuffer = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, MAX_FRAME_LAYERS, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer->SetDepthBuffer();

		m_Sphere = std::make_unique<Sphere>(m_Sectors, m_Stacks, 2, Surface::Smooth);

	}

	MultiLayerMotionBlur::~MultiLayerMotionBlur()
	{
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);
	}

	void MultiLayerMotionBlur::OnUpdate(double deltaTime)
	{
		m_DeltaTime = deltaTime;

		if (m_SphereChanged)
		{
			UpdateSphere();
		}

		auto d = m_DependendOnFPS ? m_DeltaTime : m_ConstTime;

		for (int i = m_MotionBlurLevel -1; i >= 0; i--)
		{
			if (i == m_MotionBlurLevel - 1)
			{
				calculatePosition(m_DeltaTime - (d * m_MotionBlurPower));

				if (m_PredefinedCameraMovement)
				{
					PredefinedCameraMovement(m_DeltaTime - (d * m_MotionBlurPower));
				}
			}

			calculatePosition(d * m_MotionBlurPower / m_MotionBlurLevel);

			if (m_PredefinedCameraMovement)
			{
				PredefinedCameraMovement(d * m_MotionBlurPower / m_MotionBlurLevel);
			}

			m_Sphere->setPosition(m_ModelTranslation[0], m_ModelTranslation[1], m_ModelTranslation[2]);
			m_Sphere->setRotation(m_ModelRotation[0], m_ModelRotation[1], m_ModelRotation[2]);
			m_Sphere->setScale(m_ModelScale[0], m_ModelScale[1], m_ModelScale[2]);

			m_View = m_Camera.getView();
			m_Model[i] = m_Sphere->getModelMatrix();

			m_MVP[i] = m_Proj * m_View * m_Model[i];
		}
	}

	void MultiLayerMotionBlur::OnRender()
	{
		ObjectTest::OnRender();
		m_FrameBuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_FrameBuffer->BindTexture(1);

		m_Texture->Bind(0);
		m_Program[0]->UseProgram();

		for (int i = 0; i < m_MotionBlurLevel; i++)
		{
			m_Program[0]->SetUniformMatrix4fv("u_MVP[" + std::to_string(i) + "]", m_MVP[i]);
			m_Program[0]->SetUniformMatrix4fv("u_Model[" + std::to_string(i) + "]", m_Model[i]);
		}

		m_Program[0]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

		m_Program[0]->SetUniform1i("u_texture", 0);

		m_Program[0]->SetUniform3fv("material.specular", m_Material.specular);
		m_Program[0]->SetUniform1f("material.shininess", m_Material.shininess);

		m_Program[0]->SetUniform3fv("light.direction", m_Light.direction);
		m_Program[0]->SetUniform3fv("light.ambient", m_Light.ambient);
		m_Program[0]->SetUniform3fv("light.diffuse", m_Light.diffuse);
		m_Program[0]->SetUniform3fv("light.specular", m_Light.specular);

		m_Program[0]->SetUniform1i("u_blurLevel", m_MotionBlurLevel);

		glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, m_MotionBlurLevel);

		if (m_DrawLines)
		{
			m_Program[1]->UseProgram();
			for (int i = 0; i < m_MotionBlurLevel; i++)
			{
				glm::mat4x4 a = m_MVP[i];
				glm::mat4x4 b = m_Model[i];
				m_Program[1]->SetUniformMatrix4fv("u_MVP[" + std::to_string(i) + "]", a);
			}

			m_Program[1]->SetUniform1i("u_blurLevel", m_MotionBlurLevel);

			glDrawElementsInstanced(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, m_MotionBlurLevel);
		}

		m_FrameBuffer->Unbind();
		glDisable(GL_DEPTH_TEST);

		m_FrameBuffer->BindTexture(1);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Program[2]->UseProgram();
		m_Program[2]->SetUniform1i("u_texture", 1);
		m_Program[2]->SetUniform1i("u_blurLevel", m_MotionBlurLevel);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_FrameBuffer->BindTexture(1);
		glEnable(GL_DEPTH_TEST);
	}

	void MultiLayerMotionBlur::OnImGuiRender()
	{
		ObjectTest::OnImGuiRender();

		ObjectTest::ImGuiShowMotionBlurSettings();
	}
}
