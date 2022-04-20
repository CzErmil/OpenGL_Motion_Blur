#include "TestModel.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "functional"
#include <cmath>

namespace test {

	TestModel::TestModel()
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
		m_MovementSumTimeXYZ[XYZ] += deltaTime * m_MovemnetSpeed[XYZ];

		switch (typeOfMovement)
		{
		case 0:
			m_ModelTranslation[XYZ] = m_OriginalModelTranslation[XYZ];
			break;
		case 1:
			m_ModelTranslation[XYZ] = m_OriginalModelTranslation[XYZ] + sinf(m_MovementSumTimeXYZ[XYZ]) * m_Radius[XYZ];
			break;
		case 2:
			m_ModelTranslation[XYZ] = m_OriginalModelTranslation[XYZ] + cosf(m_MovementSumTimeXYZ[XYZ]) * m_Radius[XYZ];
			break;
		default:
			break;
		}
	}

	void TestModel::calculateRotation(double deltaTime)
	{
		for (int i = 0; i < 3; i++)
		{
			m_RotationSumTimeXYZ[i] += deltaTime * m_RotationSpeed[i];

			if (m_RotationAxiexXYZ[i])
			{
				m_ModelRotation[i] = fmod(m_OriginalModelRotation[i] + 180 + m_RotationSumTimeXYZ[i] * 30, 360) - 180;
			}
			else
			{
				m_ModelRotation[i] = m_OriginalModelRotation[i];
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

		m_Sphere->setPosition(m_ModelTranslation[0], m_ModelTranslation[1], m_ModelTranslation[2]);
		m_Sphere->setRotation(m_ModelRotation[0], m_ModelRotation[1], m_ModelRotation[2]);
		m_Sphere->setScale(m_ModelScale[0], m_ModelScale[1], m_ModelScale[2]);

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
		ObjectTest::OnImGuiRender();
	}
}
