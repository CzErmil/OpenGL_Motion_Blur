#include "TestClearColor.h"

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {

	TestClearColor::TestClearColor():
		m_ClearColor{ 0.2, 0.3, 0.6, 1.0 }
	{

	}
	TestClearColor::~TestClearColor()
	{

	}

	void TestClearColor::OnUpdate(double deltaTime)
	{

	}

	void TestClearColor::OnRender()
	{	
		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}