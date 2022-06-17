#include "PerObjectMotionBlur.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {

	PerObjectMotionBlur::PerObjectMotionBlur()
	{
		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VelocityBuffer_Vertex.shader");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/VelocityBuffer_Fragment.shader");

		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DirLight_fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

		Shader vertexShader3 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
		Shader fragmentShader3 = Shader(GL_FRAGMENT_SHADER, "src/shaders/PerObjectMotionBlur_Fragment.shader");

		m_Program[2] = std::make_unique<ShaderProgram>(vertexShader3, fragmentShader3);

		Shader vertexShader4 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_vertex.glsl");
		Shader geometryShader4 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_geometry.glsl");
		Shader fragmentShader4 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

		m_Program[3] = std::make_unique<ShaderProgram>(vertexShader4, geometryShader4, fragmentShader4);

		m_Program[1]->UseProgram();
		m_Program[1]->SetUniform1i("u_texture", 0);

		m_Sphere = std::make_unique<Sphere>(m_Sectors, m_Stacks, 2, Surface::Smooth);

		m_Proj = m_Camera.getProj();
		m_View = m_Camera.getView();
		m_Model = m_Sphere->getModelMatrix();
		m_prevMVP = m_Proj * m_View * m_Model;

		m_FrameBuffer[0] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[0]->SetDepthBuffer();

		m_FrameBuffer[1] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[1]->SetDepthBuffer();
	}

	PerObjectMotionBlur::~PerObjectMotionBlur()
	{
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);
	}

	void PerObjectMotionBlur::OnUpdate(double deltaTime)
	{
		ObjectTest::OnUpdate(deltaTime);
	}

	void PerObjectMotionBlur::OnRender()
	{
		ObjectTest::OnRender();

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[0]->SetUniformMatrix4fv("u_prevMVP", m_prevMVP);

		m_FrameBuffer[0]->BindTexture(1);
		m_FrameBuffer[0]->Bind();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_FrameBuffer[0]->Unbind();

		m_Texture->Bind(0);

		m_FrameBuffer[1]->BindTexture(2);
		m_Program[1]->UseProgram();
		m_Program[1]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[1]->SetUniformMatrix4fv("u_Model", m_Model);

		m_Program[1]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

		m_Program[1]->SetUniform1i("u_texture", 0);

		m_Program[1]->SetUniform3fv("material.specular", m_Material.specular);
		m_Program[1]->SetUniform1f("material.shininess", m_Material.shininess);

		m_Program[1]->SetUniform3fv("light.direction", m_Light.direction);
		m_Program[1]->SetUniform3fv("light.ambient", m_Light.ambient);
		m_Program[1]->SetUniform3fv("light.diffuse", m_Light.diffuse);
		m_Program[1]->SetUniform3fv("light.specular", m_Light.specular);

		m_FrameBuffer[1]->Bind();
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_FrameBuffer[1]->Unbind();

		m_FrameBuffer[0]->BindTexture(1);
		m_FrameBuffer[1]->BindTexture(2);

		m_Program[2]->UseProgram();
		m_Program[2]->SetUniform1i("u_texture", 2);
		m_Program[2]->SetUniform1i("u_velocityBuffer", 1);
		m_Program[2]->SetUniform1i("u_blurLevel", m_MotionBlurLevel);
		m_Program[2]->SetUniform1f("u_blurPower", m_MotionBlurPower);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_prevMVP = m_MVP;
	}

	void PerObjectMotionBlur::OnImGuiRender()
	{
		ObjectTest::OnImGuiRender();

		ObjectTest::ImGuiShowMotionBlurSettings();
	}

}