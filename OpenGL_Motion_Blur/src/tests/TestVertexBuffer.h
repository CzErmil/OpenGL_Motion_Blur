#pragma once

#include "Test.h"

#include <memory>

#include "VertexArrayObject.h"
#include "ShaderProgram.h"

namespace test {

	class TestVertexBuffer : public Test
	{
	public:
		TestVertexBuffer();
		~TestVertexBuffer();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
		std::unique_ptr<VertexArrayObject> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<ShaderProgram> m_Program;
	};
}