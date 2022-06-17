#pragma once

#include "ObjectTest.h"

#include <memory>

#include "glm/gtx/transform.hpp"

#include "VertexArrayObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Sphere.h"
#include "Inputs.h"
#include "Camera.h"

namespace test {

	class PerObjectMotionBlur : public ObjectTest
	{
	private:
		std::unique_ptr<ShaderProgram> m_Program[4];
		std::unique_ptr<FrameBuffer> m_FrameBuffer[2];
		std::unique_ptr<Model> m_Sphere;
		glm::mat4 m_prevMVP;
	public:
		PerObjectMotionBlur();
		~PerObjectMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
