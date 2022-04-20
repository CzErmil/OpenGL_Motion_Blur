#include "ObjectTest.h"

#include <glm/gtc/type_ptr.hpp>

namespace test 
{
	ObjectTest::ObjectTest() :
		m_Light{ glm::vec3(-0.5f,-0.5f,-0.3f),		// direction
		glm::vec3(0.1f, 0.1f, 0.1f),				// ambient
		glm::vec3(1.0f, 1.0f, 1.0f),				// diffuse
		glm::vec3(1.0f, 1.0f, 1.0f) },				// specular
		m_Material{ glm::vec3(1.0f, 1.0f, 1.0f),	// ambient
			glm::vec3(1.0f, 1.0f, 1.0f),			// diffuse
			glm::vec3(0.3f, 0.3f, 0.3f),			// specular
			16.0f },								// shininess
		m_ClearColor{ glm::vec4(0.2f, 0.2f, 0.4f, 1.0f) },
		m_ModelRotation{ glm::vec3(0.0f,0.0f,0.0f) },
		m_ModelTranslation{ glm::vec3(0.0f,0.0f,0.0f) },
		m_ModelScale{ glm::vec3(1.0f,1.0f,1.0f) },
		m_MovemnetSpeed{ glm::vec3(3.0f,3.0f,3.0f) },
		m_Radius{ glm::vec3(1.0f,1.0f,1.0f) },
		m_DeltaTime(0),
		m_Camera(&m_DeltaTime),
		m_DrawLines(0),
		m_Smooth(1),
		m_Sektors(30),
		m_Stack(30),
		m_SphereChanged(1)
	{

	}

	void ObjectTest::OnUpdate(double deltaTime)
	{

	}

	void ObjectTest::OnRender()
	{

	}

	void ObjectTest::OnImGuiRender()
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
			ImGui::SliderFloat3("Model translation", glm::value_ptr(m_OriginalModelTranslation), -10.0f, 10.0f, "%.1f", 1.0f);
			ImGui::SliderFloat3("Model rotation", glm::value_ptr(m_OriginalModelRotation), -180.0f, 180.0f, "%.0f", 1.0f);
			ImGui::SliderFloat3("Model scaling", glm::value_ptr(m_ModelScale), 0.1f, 3.0f, "%.2f", 1.0f);

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

			ImGui::SliderFloat("Radius of movement in the x axis", &m_Radius.x, 0.0f, 20.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("X-axis movement speed", &m_MovemnetSpeed.x, 0.0f, 10.0f, "%.1f", 1.0f);

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

			ImGui::SliderFloat("Radius of movement in the y axis", &m_Radius.y, 0.0f, 20.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Y-axis movement speed", &m_MovemnetSpeed.y, 0.0f, 10.0f, "%.1f", 1.0f);

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

			ImGui::SliderFloat("Radius of movement in the z axis", &m_Radius.z, 0.0f, 20.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Z-axis movement speed", &m_MovemnetSpeed.z, 0.0f, 10.0f, "%.1f", 1.0f);

			ImGui::Text("Axis of rotation");

			ImGui::Checkbox("x axies", &m_RotationAxiexXYZ[0]); ImGui::SameLine();
			ImGui::Checkbox("y axies", &m_RotationAxiexXYZ[1]); ImGui::SameLine();
			ImGui::Checkbox("z axies", &m_RotationAxiexXYZ[2]);
			ImGui::SliderFloat("X-axis rotation speed", &m_RotationSpeed.x, 0.0f, 10.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Y-axis rotation speed", &m_RotationSpeed.y, 0.0f, 10.0f, "%.1f", 1.0f);
			ImGui::SliderFloat("Z-axis rotation speed", &m_RotationSpeed.z, 0.0f, 10.0f, "%.1f", 1.0f);

			ImGui::PopItemWidth();
		}

		if (ImGui::Button("Reset"))
		{
			m_OriginalModelRotation = glm::vec3(0.0f);
			m_OriginalModelTranslation = glm::vec3(0.0f);
			m_ModelScale = glm::vec3(1.0f);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
