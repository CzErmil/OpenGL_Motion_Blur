#pragma once

#include "ObjectTest.h"

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

	class CameraMotionBlur : public ObjectTest
	{
	private:
		std::unique_ptr<ShaderProgram> m_Program[2];
		std::unique_ptr<FrameBuffer> m_FrameBuffer[2];
		glm::mat4 m_PrevMVP;
	public:
		CameraMotionBlur();
		~CameraMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}