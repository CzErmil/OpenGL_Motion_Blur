#include "TestModel.h"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "functional"
#include <cmath>

namespace test {

	TestModel::TestModel()
	{
		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/DirLight_fragment.glsl");

		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_vertex.glsl");
		Shader geometryShader2 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_geometry.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, geometryShader2, fragmentShader2);

		m_Program[0]->UseProgram();
	}

	TestModel::~TestModel()
	{
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);
	}

	void TestModel::OnUpdate(double deltaTime)
	{
		ObjectTest::OnUpdate(deltaTime);
	}

	void TestModel::OnRender()
	{
		ObjectTest::OnRender();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
