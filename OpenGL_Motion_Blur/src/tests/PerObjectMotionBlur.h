#pragma once

#include "Test.h"

#include <memory>

#include "glm/gtx/transform.hpp"

#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"

namespace test {

	class PerObjectMotionBlur : public Test
	{
	private:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<ShaderProgram> m_Program[3];
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<FrameBuffer> m_FrameBuffer[2];
		glm::mat4 m_Proj, m_View, m_Model;
		glm::mat4 m_MVP;
	public:
		PerObjectMotionBlur();
		~PerObjectMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
