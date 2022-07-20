#pragma once

#include "ObjectTest.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Inputs.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"

namespace test {

	#define MAX_FRAME_LAYERS 100

	class MultiLayerMotionBlur : public ObjectTest
	{
	private:
		std::unique_ptr<ShaderProgram> m_Program[3];
		std::unique_ptr<FrameBuffer> m_FrameBuffer;
		glm::mat4 m_Model[MAX_FRAME_LAYERS];
		glm::mat4 m_MVP[MAX_FRAME_LAYERS];
		int m_FrameBufferLayers;
	public:
		MultiLayerMotionBlur();
		~MultiLayerMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}