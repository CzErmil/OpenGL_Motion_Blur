#pragma once

#include "ObjectTest.h"

#include "imgui/imgui.h"

#include <memory>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Inputs.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Material.h"

namespace test {

	class TestModel : public ObjectTest
	{
	private:
		std::unique_ptr<ShaderProgram> m_Program[2];
	public:
		TestModel();
		~TestModel();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}