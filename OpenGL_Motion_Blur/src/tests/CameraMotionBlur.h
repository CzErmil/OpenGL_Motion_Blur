#pragma once

#include "Test.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Sphere.h"
#include "Camera.h"
#include "Inputs.h"

namespace test {

	class CameraMotionBlur : public Test
	{
	private:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBaffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<ShaderProgram> m_Program[2];
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<FrameBuffer> m_FrameBuffer[2];
		std::unique_ptr<Sphere> m_Sphere;
		Camera m_Camera;
		Inputs m_Inputs;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		glm::mat4 m_MVP;
		glm::mat4 m_ModelRotation;
		glm::mat4 m_ModelTranslation;
		glm::mat4 m_ModelScaling;
		glm::mat4 m_PrevMVP;
		float m_ModelRotationXYZ[3];
		float m_ModelTranslationXYZ[3];
		float m_ModelScalingXYZ[3];
		float m_PrevModelRotationXYZ[3];
		float m_PrevModelTranslationXYZ[3];
		float m_PrevModelScalingXYZ[3];
		unsigned int m_ColorBufferFBO;
		unsigned int m_ColorBuffer;
		unsigned int m_DepthMapFBO;
		unsigned int m_DepthMap;
		double m_CurrentTime;
		double m_DeltaTime;
		int m_SwapInterval;
		int m_MotionBlurLevel;
		float m_MotionBlurPower;
		float m_CubeSpeed;
	public:
		CameraMotionBlur();
		~CameraMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}