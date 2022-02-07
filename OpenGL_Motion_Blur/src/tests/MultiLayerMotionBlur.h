#pragma once

#include "Test.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Inputs.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"

namespace test {

	class MultiLayerMotionBlur : public Test
	{
	private:
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<ShaderProgram> m_Program[2];
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Model> m_Sphere;
		glm::mat4 m_Proj, m_View,  m_Model;
		glm::mat4 m_MVP;
		glm::mat4 m_ModelRotation, m_ModelTranslation, m_ModelScaling;
		Inputs m_Inputs;
		Camera m_Camera;
		float m_ModelRotationXYZ[3];
		float m_ModelTranslationXYZ[3];
		float m_ModelScaleXYZ[3];
		double m_DeltaTime;
		bool m_DrawLines;
		bool m_Smooth;
		int m_Sektors, m_Stack;
		bool m_SphereChanged;
	public:
		MultiLayerMotionBlur();
		~MultiLayerMotionBlur();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}