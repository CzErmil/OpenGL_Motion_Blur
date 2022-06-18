#pragma once

#include "Test.h"

#include "imgui/imgui.h"

#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Inputs.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"
#include "Light.h"
#include "Material.h"

namespace test
{
	class ObjectTest : public Test
	{
	private:
		const DirectionalLight LIGHT
		{
			glm::vec3(-0.5f, -0.5f, -0.3f),	// direction
			glm::vec3(0.1f, 0.1f, 0.1f),	// ambient
			glm::vec3(1.0f, 1.0f, 1.0f),	// diffuse
			glm::vec3(1.0f, 1.0f, 1.0f)		// specular
		};
		const Material MATERIAL
		{
			glm::vec3(1.0f, 1.0f, 1.0f),	// ambient
			glm::vec3(1.0f, 1.0f, 1.0f),	// diffuse
			glm::vec3(0.3f, 0.3f, 0.3f),	// specular
			16.0f							// shininess
		};
		const glm::vec4 CLEARCOLOR{ glm::vec4(0.2f, 0.2f, 0.4f, 1.0f) };
		const glm::vec3 MODELROTATION { glm::vec3(0.0f,0.0f,0.0f) };
		const glm::vec3 MODELTRANSLATION { glm::vec3(0.0f,0.0f,0.0f) };
		const glm::vec3	MODELSCALE{ glm::vec3(1.0f,1.0f,1.0f) };
		const glm::vec3	MOVEMENTSPEED { glm::vec3(3.0f,3.0f,3.0f) };
		const glm::vec3	ROTATIONSPEED { glm::vec3(1.0f,1.0f,1.0f) };
		const glm::vec3	RADIUS { glm::vec3(1.0f,1.0f,1.0f) };
		const bool DRAWLINES{ false };
		const bool SMOOTH{ true };
		const int SECTORS{ 30 };
		const int STACKS{ 30 };
	protected:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
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
		int m_TypeOfMovementXYZ[3];
		bool m_RotationAxiexXYZ[3];
		double m_DeltaTime;
		double m_MovementSumTimeXYZ[3];
		double m_RotationSumTimeXYZ[3];
		float m_CameraHorizontalMovementRadius;
		float m_CameraVerticalMovementRadius;
		float m_CameraHorizontalRotationSpeed;
		int m_CameraVerticalTypeOfMovement;
		float m_CameraVerticalMovementSpeed;
		double m_CameraHorizontalRotationSumTime;
		double m_CameraVerticalMovementSumTime;
		bool m_DrawLines;
		bool m_Smooth;
		int m_Sectors, m_Stacks;
		bool m_SphereChanged;
		bool m_PredefinedCameraMovement;
		int m_MotionBlurLevel;
		float m_MotionBlurPower;
		int WINDOW_WIDTH, WINDOW_HEIGHT;
	public:
		ObjectTest();
		virtual ~ObjectTest() {};

		void BindInput();
		void UnbindInput();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void ResetEnviromentSettings();
		void ResetObjectParameters();
		void ResetSphereModyfications();
		void ResetObjectMovement();
		void ResetCameraMovement();

		void calculatePosition(double deltaTime);
		void calculateMovement(int typeOfMovement, int XYZ, double deltaTime);
		void calculateRotation(double deltaTime);
		void UpdateSphere();

		void ImGuiSetEnviromentSettings();
		void ImGuiSetObjectParameters();
		void ImGuiSetSphereModifications();
		void ImGuiSetObjectMovement();
		void ImGuiSetCameraMovement();

		void ImGuiShowMotionBlurSettings();
	};
}

