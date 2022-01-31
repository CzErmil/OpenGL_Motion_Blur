#pragma once

#include "Test.h"

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

	class PerObjectMotionBlur : public Test
	{
	private:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<ShaderProgram> m_Program[4];
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<FrameBuffer> m_FrameBuffer[2];
		std::unique_ptr<Model> m_Sphere;
		glm::mat4 m_Proj, m_View, m_Model;
		glm::mat4 m_MVP, m_prevMVP;
		Inputs m_Inputs;
		Camera m_Camera;
		int m_MotionBlurLevel;
		double m_DeltaTime;
		double m_SumTime;
		int m_Sektors, m_Stack;
		bool m_DrawLines, m_Smooth, m_SphereChanged;
	public:
		PerObjectMotionBlur();
		~PerObjectMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
