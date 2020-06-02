#pragma once

#include "Test.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Texture.h"

namespace test {

	class AccumulateMotionBlur : public Test
	{
	private:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBaffer;
		std::unique_ptr<ShaderProgram> m_Program[3];
		std::unique_ptr<Texture> m_Texture;
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
		unsigned int m_ColorBufferFBO2;
		unsigned int m_ColorBuffer2;
		float m_CurrentTime;
		int m_SwapInterval;
		int m_MotionBlurLevel;
		float m_CubeSpeed;
	public:
		AccumulateMotionBlur();
		~AccumulateMotionBlur();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}