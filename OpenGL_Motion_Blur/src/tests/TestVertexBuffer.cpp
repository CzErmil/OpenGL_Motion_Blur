#include "TestVertexBuffer.h"

#include <iostream>
#include "GL/glew.h"
//#include "imgui/imgui.h"

namespace test {

	TestVertexBuffer::TestVertexBuffer():
		m_ClearColor{ 0.2, 0.3, 0.6, 1.0 }
	{
		const float data[6]{
			-0.25, -0.25,
			0.25, -0.25,
			0.25, 0.25
		};

		m_VAO = std::make_unique<VertexArrayObject>();
		m_VertexBuffer = std::make_unique <VertexBuffer>(&data, sizeof(data));

		VertexBufferLayout layout = VertexBufferLayout();

		layout.Push<float>(2);

		m_VAO->AddVertexBuffer(*m_VertexBuffer, layout);

		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/BasicVertexShader.shader");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/BasicFragmentShader.shader");

		m_Program = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
		m_Program->UseProgram();
	}

	TestVertexBuffer::~TestVertexBuffer()
	{

	}

	void TestVertexBuffer::OnUpdate(double deltaTime)
	{

	}

	void TestVertexBuffer::OnRender()
	{	
		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void TestVertexBuffer::OnImGuiRender()
	{
	}
}