#pragma once

#include "Test.h"

#include "imgui/imgui.h"

#include "Texture.h"
#include "Inputs.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Material.h"

namespace test
{
	class ObjectTest : public Test
	{
	protected:
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Model> m_Sphere;
		glm::mat4 m_Proj, m_View, m_Model;
		glm::mat4 m_MVP;
		Inputs m_Inputs;
		Camera m_Camera;
		DirectionalLight m_Light;
		Material m_Material;
		glm::vec4 m_ClearColor;
		glm::vec3 m_ModelRotation;
		glm::vec3 m_ModelTranslation;
		glm::vec3 m_OriginalModelTranslation;
		glm::vec3 m_OriginalModelRotation;
		glm::vec3 m_ModelScale;
		glm::vec3 m_Radius;
		glm::vec3 m_MovemnetSpeed;
		glm::vec3 m_RotationSpeed;
		bool m_RotationAxiexXYZ[3];
		int m_TypeOfMovementXYZ[3];
		double m_DeltaTime;
		double m_MovementSumTimeXYZ[3];
		double m_RotationSumTimeXYZ[3];
		bool m_DrawLines;
		bool m_Smooth;
		int m_Sektors, m_Stack;
		bool m_SphereChanged;
	public:
		ObjectTest();
		virtual ~ObjectTest() {};

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}

