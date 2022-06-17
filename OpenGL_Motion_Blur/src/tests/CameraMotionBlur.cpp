#include "CameraMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {

	CameraMotionBlur::CameraMotionBlur()
	{
		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/DirLight_fragment.glsl");

		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/CameraMotionBlur_Vertex.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/CameraMotionBlur_Fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

		m_Proj = m_Camera.getProj();
		m_View = m_Camera.getView();
		m_PrevMVP = m_Proj * m_View;

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

		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);
	}

	void CameraMotionBlur::OnUpdate(double deltaTime)
	{
		ObjectTest::OnUpdate(deltaTime);
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
		m_Program[0]->SetUniformMatrix4fv("u_Model", m_Model);

		m_Program[0]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

		m_Program[0]->SetUniform1i("u_texture", 0);

		m_Program[0]->SetUniform3fv("material.specular", m_Material.specular);
		m_Program[0]->SetUniform1f("material.shininess", m_Material.shininess);

		m_Program[0]->SetUniform3fv("light.direction", m_Light.direction);
		m_Program[0]->SetUniform3fv("light.ambient", m_Light.ambient);
		m_Program[0]->SetUniform3fv("light.diffuse", m_Light.diffuse);
		m_Program[0]->SetUniform3fv("light.specular", m_Light.specular);

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
		ObjectTest::OnImGuiRender();

		ObjectTest::ImGuiShowMotionBlurSettings();
	}
}
