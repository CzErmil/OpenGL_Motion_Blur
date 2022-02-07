#include "MultiLayerMotionBlur.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "functional"

namespace test {

	MultiLayerMotionBlur::MultiLayerMotionBlur() :
		m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
		m_ModelTranslationXYZ{ 0.0f,0.0f,0.0f },
		m_ModelScaleXYZ{ 1.0f,1.0f,1.0f },
		m_DeltaTime(0),
		m_Camera(&m_DeltaTime),
		m_DrawLines(0),
		m_Smooth(1),
		m_Sektors(30),
		m_Stack(30),
		m_SphereChanged(1)
	{
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		glfwGetWindowSize(glfwGetCurrentContext(), &WINDOW_WIDTH, &WINDOW_HEIGHT);

		glLineWidth(2);

		m_VAO = std::make_unique<VertexArrayObject>();
		m_VAO->Bind();

		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, (6*100*80+3*100*20) * sizeof(Vertex), GL_DYNAMIC_DRAW);

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

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_vertex.glsl");
		Shader geometryShader2 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_geometry.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, geometryShader2, fragmentShader2);

		m_Texture = std::make_unique<Texture>("res/textures/earth.bmp", GL_LINEAR);
		m_Texture->Bind(0);

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniform1i("u_texture", 0);

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
	}

	MultiLayerMotionBlur::~MultiLayerMotionBlur()
	{

	}

	void MultiLayerMotionBlur::OnUpdate(double deltaTime)
	{
		m_DeltaTime = deltaTime;

		if (m_SphereChanged) {
			if (m_Smooth)
				m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Smooth);
			else
				m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Flat);

			m_VertexBuffer->UpdateData(m_Sphere->getVertices().data(), m_Sphere->getSize());
			m_IndexBuffer->UpdateBuffer(m_Sphere->getIndecies().data(), m_Sphere->getIndecies().size());
			m_SphereChanged = 0;
		}

		m_ModelRotationXYZ[1] += (float)m_DeltaTime;

		if (m_ModelRotationXYZ[1] > 180)
			m_ModelRotationXYZ[1] -= 360;

		m_Sphere->setPosition(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]);
		m_Sphere->setRotation(m_ModelRotationXYZ[0], m_ModelRotationXYZ[1], m_ModelRotationXYZ[2]);
		m_Sphere->setScale(m_ModelScaleXYZ[0], m_ModelScaleXYZ[1], m_ModelScaleXYZ[2]);

		m_View = m_Camera.getView();
		m_Model = m_Sphere->getModelMatrix();

		m_MVP = m_Proj * m_View * m_Model;
	}

	void MultiLayerMotionBlur::OnRender()
	{

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[0]->SetUniformMatrix4fv("u_Model", m_Model);
		m_Program[0]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

		if (m_DrawLines)
		{
			m_Program[1]->UseProgram();
			m_Program[1]->SetUniformMatrix4fv("u_MVP", m_MVP);
			m_Program[1]->SetUniform3f("u_color", 0.0f, 0.0f, 0.0f);

			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void MultiLayerMotionBlur::OnImGuiRender()
	{
		{
			ImGui::SliderFloat3("Model translation", m_ModelTranslationXYZ, -10.0f, 10.0f, "%.1f", 2.0f);
			ImGui::SliderFloat3("Model rotation", m_ModelRotationXYZ, -180.0f, 180.0f, "%.0f", 1.0f);
			ImGui::SliderFloat3("Model scaling", m_ModelScaleXYZ, 0.1f, 3.0f, "%.2f", 3.0f);
			if(ImGui::SliderInt("Sphere secttors", &m_Sektors, 3, 100, "%d")) m_SphereChanged=1;
			if(ImGui::SliderInt("Sphere stack", &m_Stack, 3, 100, "%d")) m_SphereChanged = 1;

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
				m_ModelTranslationXYZ[0] = m_ModelTranslationXYZ[1] = m_ModelTranslationXYZ[2] = 0.0f;
				m_ModelScaleXYZ[0] = m_ModelScaleXYZ[1] = m_ModelScaleXYZ[2] = 1.0f;
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	}
}
