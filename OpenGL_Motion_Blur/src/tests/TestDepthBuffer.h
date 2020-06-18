#pragma once

#include "Test.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include "FrameBuffer.h"

namespace test {

	class TestDepthBuffer : public Test
	{
	private:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBaffer;
		std::unique_ptr<ShaderProgram> m_Program[2];
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<FrameBuffer> m_FrameBuffer;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		glm::mat4 m_MVP;
		glm::mat4 m_ModelRotation;
		glm::mat4 m_ModelTranslation;
		glm::mat4 m_ModelScaling;
		float m_ModelRotationXYZ[3];
		float m_ModelTranslationXYZ[3];
		float m_ModelScalingXYZ[3];
	public:
		TestDepthBuffer();
		~TestDepthBuffer();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}