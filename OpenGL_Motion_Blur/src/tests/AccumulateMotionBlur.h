#pragma once

#include "ObjectTest.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Inputs.h"
#include "Sphere.h"

namespace test {

	class AccumulateMotionBlur : public ObjectTest
	{
	private:
		std::unique_ptr<ShaderProgram> m_Program[4];
		std::unique_ptr<FrameBuffer> m_FrameBuffer[2];
	public:
		AccumulateMotionBlur();
		~AccumulateMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void calculatePosition(double deltaTime);
	};
}