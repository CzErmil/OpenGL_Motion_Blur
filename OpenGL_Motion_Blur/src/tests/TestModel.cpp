#include "TestModel.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "functional"
#include <cmath>

namespace test {

	TestModel::TestModel() :
		m_Light{ glm::vec3(-0.5f,-0.5f,-0.3f),		// direction
			glm::vec3(0.1f, 0.1f, 0.1f),			// ambient
			glm::vec3(1.0f, 1.0f, 1.0f),			// diffuse
			glm::vec3(1.0f, 1.0f, 1.0f) },			// specular
		m_Material{ glm::vec3(1.0f, 1.0f, 1.0f),	// ambient
			glm::vec3(1.0f, 1.0f, 1.0f),			// diffuse
			glm::vec3(0.3f, 0.3f, 0.3f),			// specular
			16.0f },									// shininess
		m_ClearColor{ glm::vec4(0.2f, 0.2f, 0.4f, 1.0f) },
		m_ModelRotationXYZ{ 0.0f,0.0f,0.0f },
		m_ModelTranslationXYZ{ 0.0f,0.0f,0.0f },
		m_ModelScaleXYZ{ 1.0f,1.0f,1.0f },
		m_MovemnetSpeedXYZ{ 3.0f,3.0f,3.0f },
		m_RadiusXYZ{ 1.0f,1.0f,1.0f },
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
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_FALSE);

		glLineWidth(2);

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
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/DirLight_fragment.glsl");

		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_vertex.glsl");
		Shader geometryShader2 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_geometry.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, geometryShader2, fragmentShader2);

		m_Texture = std::make_unique<Texture>("res/textures/earth.bmp", GL_LINEAR);
		m_Texture->Bind(0);

		m_Program[0]->UseProgram();

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

	TestModel::~TestModel()
	{
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);
	}

	void TestModel::calculatePosition(double deltaTime)
	{
		calculateMovement(m_TypeOfMovementXYZ[0], 0, deltaTime);
		calculateMovement(m_TypeOfMovementXYZ[1], 1, deltaTime);
		calculateMovement(m_TypeOfMovementXYZ[2], 2, deltaTime);
		calculateRotation(deltaTime);
	}

	void TestModel::calculateMovement(int typeOfMovement, int XYZ, double deltaTime)
	{
		m_MovementSumTimeXYZ[XYZ] += deltaTime * m_MovemnetSpeedXYZ[XYZ];

		switch (typeOfMovement)
		{
		case 0:
			m_ModelTranslationXYZ[XYZ] = m_OriginalModelTranslationXYZ[XYZ];
			break;
		case 1:
			m_ModelTranslationXYZ[XYZ] = m_OriginalModelTranslationXYZ[XYZ] + sinf(m_MovementSumTimeXYZ[XYZ]) * m_RadiusXYZ[XYZ];
			break;
		case 2:
			m_ModelTranslationXYZ[XYZ] = m_OriginalModelTranslationXYZ[XYZ] + cosf(m_MovementSumTimeXYZ[XYZ]) * m_RadiusXYZ[XYZ];
			break;
		default:
			break;
		}
	}

	void TestModel::calculateRotation(double deltaTime)
	{
		for (int i = 0; i < 3; i++)
		{
			m_RotationSumTimeXYZ[i] += deltaTime * m_RotationSpeedXYZ[i];

			if (m_RotationAxiexXYZ[i])
			{
				m_ModelRotationXYZ[i] = fmod(m_OriginalModelRotationXYZ[i] + 180 + m_RotationSumTimeXYZ[i] * 30, 360) - 180;
			}
			else
			{
				m_ModelRotationXYZ[i] = m_OriginalModelRotationXYZ[i];
				m_RotationSumTimeXYZ[i] = 0;
			}

		}
	}

	void TestModel::OnUpdate(double deltaTime)
	{
		m_DeltaTime = deltaTime;

		calculatePosition(deltaTime);

		if (m_SphereChanged) {
			if (m_Smooth)
				m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Smooth);
			else
				m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Flat);

			m_VertexBuffer->UpdateData(m_Sphere->getVertices().data(), m_Sphere->getSize());
			m_IndexBuffer->UpdateBuffer(m_Sphere->getIndecies().data(), m_Sphere->getIndecies().size());
			m_SphereChanged = 0;
		}

		m_Sphere->setPosition(m_ModelTranslationXYZ[0], m_ModelTranslationXYZ[1], m_ModelTranslationXYZ[2]);
		m_Sphere->setRotation(m_ModelRotationXYZ[0], m_ModelRotationXYZ[1], m_ModelRotationXYZ[2]);
		m_Sphere->setScale(m_ModelScaleXYZ[0], m_ModelScaleXYZ[1], m_ModelScaleXYZ[2]);

		m_View = m_Camera.getView();
		m_Model = m_Sphere->getModelMatrix();

		m_MVP = m_Proj * m_View * m_Model;
	}

	void TestModel::OnRender()
	{
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[0]->SetUniformMatrix4fv("u_Model", m_Model);
		m_Program[0]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

		m_Program[0]->SetUniform1i("u_texture", 0);

		m_Program[0]->SetUniform3fv("material.ambient", m_Material.ambient);
		m_Program[0]->SetUniform3fv("material.diffuse", m_Material.diffuse);
		m_Program[0]->SetUniform3fv("material.specular", m_Material.specular);
		m_Program[0]->SetUniform1f("material.shininess", m_Material.shininess);

		m_Program[0]->SetUniform3fv("light.direction", m_Light.direction);
		m_Program[0]->SetUniform3fv("light.ambient", m_Light.ambient);
		m_Program[0]->SetUniform3fv("light.diffuse", m_Light.diffuse);
		m_Program[0]->SetUniform3fv("light.specular", m_Light.specular);

		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

		if (m_DrawLines)
		{
			m_Program[1]->UseProgram();
			m_Program[1]->SetUniformMatrix4fv("u_MVP", m_MVP);
			m_Program[1]->SetUniform3f("u_color", 0.0f, 0.0f, 0.0f);

			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void TestModel::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("Enviroment settings", ImGuiTreeNodeFlags_None))
		{
			ImGui::ColorEdit4("Clear color", glm::value_ptr(m_ClearColor));
			ImGui::SliderFloat3("Light scaling", glm::value_ptr(m_Light.direction), -1.0f, 1.0f, "%.3f", 1.0f);
			ImGui::ColorEdit3("Light ambient", glm::value_ptr(m_Light.ambient));
			ImGui::ColorEdit3("Light diffuse", glm::value_ptr(m_Light.diffuse));
			ImGui::ColorEdit3("Light specular", glm::value_ptr(m_Light.specular));
		}

		if (ImGui::CollapsingHeader("Object parameters", ImGuiTreeNodeFlags_None))
		{
			ImGui::SliderFloat3("Model translation", m_OriginalModelTranslationXYZ, -10.0f, 10.0f, "%.1f", 1.0f);
			ImGui::SliderFloat3("Model rotation", m_OriginalModelRotationXYZ, -180.0f, 180.0f, "%.0f", 1.0f);
			ImGui::SliderFloat3("Model scaling", m_ModelScaleXYZ, 0.1f, 3.0f, "%.2f", 1.0f);

			ImGui::ColorEdit3("Model material specular", glm::value_ptr(m_Material.specular));
			ImGui::SliderFloat("Model material shininess", &m_Material.shininess, 1.0f, 256.0f, "%.1f", 1.0f);
		}

		if (ImGui::CollapsingHeader("Sphere modifications", ImGuiTreeNodeFlags_None))
		{
			if (ImGui::SliderInt("Secttors", &m_Sektors, 3, 100, "%d")) m_SphereChanged = 1;
			if (ImGui::SliderInt("Stack", &m_Stack, 3, 100, "%d")) m_SphereChanged = 1;

			if (ImGui::RadioButton("Don't draw lines", m_DrawLines == 0))
			{
				m_DrawLines = 0;
			} ImGui::SameLine();

			if (ImGui::RadioButton("Draw lines", m_DrawLines == 1))
			{
				m_DrawLines = 1;
			}

			if (ImGui::RadioButton("Smooth surface", m_Smooth == 1))
			{
				m_Smooth = 1;
				m_SphereChanged = 1;
			} ImGui::SameLine();

			if (ImGui::RadioButton("Smooth Flat", m_Smooth == 0))
			{
				m_Smooth = 0;
				m_SphereChanged = 1;
			}
		}

		if (ImGui::CollapsingHeader("Objct movement", ImGuiTreeNodeFlags_None))
		{
			ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.7f);

			if (ImGui::RadioButton("x = const", m_TypeOfMovementXYZ[0] == 0))
			{
				m_TypeOfMovementXYZ[0] = 0;
			} ImGui::SameLine();

			if (ImGui::RadioButton("x = sin(t)", m_TypeOfMovementXYZ[0] == 1))
			{
				m_TypeOfMovementXYZ[0] = 1;
			} ImGui::SameLine();

			if (ImGui::RadioButton("x = cos(t)", m_TypeOfMovementXYZ[0] == 2))
			{
				m_TypeOfMovementXYZ[0] = 2;
			}

			ImGui::SliderFloat("Radius of movement in the x axis", &m_RadiusXYZ[0], 0.0f, 20.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("X-axis movement speed", &m_MovemnetSpeedXYZ[0], 0.0f, 10.0f, "%.1f", 1.0f);

			if (ImGui::RadioButton("y = const", m_TypeOfMovementXYZ[1] == 0))
			{
				m_TypeOfMovementXYZ[1] = 0;
			} ImGui::SameLine();

			if (ImGui::RadioButton("y = sin(t)", m_TypeOfMovementXYZ[1] == 1))
			{
				m_TypeOfMovementXYZ[1] = 1;
			} ImGui::SameLine();

			if (ImGui::RadioButton("y = cos(t)", m_TypeOfMovementXYZ[1] == 2))
			{
				m_TypeOfMovementXYZ[1] = 2;
			}

			ImGui::SliderFloat("Radius of movement in the y axis", &m_RadiusXYZ[1], 0.0f, 20.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Y-axis movement speed", &m_MovemnetSpeedXYZ[1], 0.0f, 10.0f, "%.1f", 1.0f);

			if (ImGui::RadioButton("z = const", m_TypeOfMovementXYZ[2] == 0))
			{
				m_TypeOfMovementXYZ[2] = 0;
			} ImGui::SameLine();

			if (ImGui::RadioButton("z = sin(t)", m_TypeOfMovementXYZ[2] == 1))
			{
				m_TypeOfMovementXYZ[2] = 1;
			} ImGui::SameLine();

			if (ImGui::RadioButton("z = cos(t)", m_TypeOfMovementXYZ[2] == 2))
			{
				m_TypeOfMovementXYZ[2] = 2;
			}

			ImGui::SliderFloat("Radius of movement in the z axis", &m_RadiusXYZ[2], 0.0f, 20.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Z-axis movement speed", &m_MovemnetSpeedXYZ[2], 0.0f, 10.0f, "%.1f", 1.0f);

			ImGui::Text("Axis of rotation");

			ImGui::Checkbox("x axies", &m_RotationAxiexXYZ[0]); ImGui::SameLine();
			ImGui::Checkbox("y axies", &m_RotationAxiexXYZ[1]); ImGui::SameLine();
			ImGui::Checkbox("z axies", &m_RotationAxiexXYZ[2]);
			ImGui::SliderFloat("X-axis rotation speed", &m_RotationSpeedXYZ[0], 0.0f, 10.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Y-axis rotation speed", &m_RotationSpeedXYZ[1], 0.0f, 10.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Z-axis rotation speed", &m_RotationSpeedXYZ[2], 0.0f, 10.0f, "%.1f", 1.0f);

			ImGui::PopItemWidth();
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
