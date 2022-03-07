#include "PerObjectMotionBlur.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {

	PerObjectMotionBlur::PerObjectMotionBlur():
		m_MotionBlurLevel(10),
		m_DeltaTime(0),
		m_SumTime(0),
		m_Camera(&m_DeltaTime),
		m_Sektors(30),
		m_Stack(30),
		m_DrawLines(0),
		m_Smooth(1),
		m_SphereChanged(1)
	{
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		glfwGetWindowSize(glfwGetCurrentContext(), &WINDOW_WIDTH, &WINDOW_HEIGHT);
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_FALSE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_FACE);
		glFrontFace(GL_CCW);

		glLineWidth(2);

		m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Smooth);

		m_VAO = std::make_unique<VertexArrayObject>();
		m_VAO->Bind();

		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, (6 * 100 * 80 + 3 * 100 * 20) * sizeof(Vertex), GL_DYNAMIC_DRAW);

		VertexBufferLayout layout;

		layout.Push<GLfloat>(3);
		layout.Push<GLfloat>(3);
		layout.Push<GLfloat>(2);

		m_VAO->AddVertexBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = make_unique<IndexBuffer>(nullptr, (6 * 100 * 100), GL_DYNAMIC_DRAW);
		m_IndexBuffer->Bind();

		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VelocityBuffer_Vertex.shader");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/VelocityBuffer_Fragment.shader");

		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/TestModel_vertex.glsl");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/TestModel_fragment.glsl");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

		Shader vertexShader3 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
		Shader fragmentShader3 = Shader(GL_FRAGMENT_SHADER, "src/shaders/PerObjectMotionBlur_Fragment.shader");

		m_Program[2] = std::make_unique<ShaderProgram>(vertexShader3, fragmentShader3);

		Shader vertexShader4 = Shader(GL_VERTEX_SHADER, "src/shaders/DrawLines_vertex.glsl");
		Shader geometryShader4 = Shader(GL_GEOMETRY_SHADER, "src/shaders/DrawLines_geometry.glsl");
		Shader fragmentShader4 = Shader(GL_FRAGMENT_SHADER, "src/shaders/DrawLines_fragment.glsl");

		m_Program[3] = std::make_unique<ShaderProgram>(vertexShader4, geometryShader4, fragmentShader4);

		m_Texture = std::make_unique<Texture>("res/textures/earth.bmp", GL_LINEAR);
		m_Texture->Bind(0);

		m_Program[1]->UseProgram();
		m_Program[1]->SetUniform1i("u_texture", 0);

		m_Proj = m_Camera.getProj();
		m_View = m_Camera.getView();
		m_Model = m_Sphere->getModelMatrix();
		m_prevMVP = m_Proj * m_View * m_Model;

		std::function<void(int)> w = std::bind(&Camera::setForward, &m_Camera, std::placeholders::_1);
		std::function<void(int)> s = std::bind(&Camera::setBackward, &m_Camera, std::placeholders::_1);
		std::function<void(int)> a = std::bind(&Camera::setLeft, &m_Camera, std::placeholders::_1);
		std::function<void(int)> d = std::bind(&Camera::setRight, &m_Camera, std::placeholders::_1);
		std::function<void(int)> space = std::bind(&Camera::setUp, &m_Camera, std::placeholders::_1);
		std::function<void(int)> shift = std::bind(&Camera::setDown, &m_Camera, std::placeholders::_1);
		std::function<void(int)> mouseMiddleButton = std::bind(&Camera::setMouseMiddleButton, &m_Camera, std::placeholders::_1);
		std::function<void(double, double, double, double)>  cursor = std::bind(&Camera::processMouseMovment, &m_Camera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		m_Inputs.setWSADKeyCallbacks(w, s, a, d);
		m_Inputs.setSpaceKeyCallback(space);
		m_Inputs.setShiftKeyCallback(shift);
		m_Inputs.setMouseButtonCallbacks(GLFW_MOUSE_BUTTON_3, mouseMiddleButton);
		m_Inputs.setCursorCullbuck(cursor);

		m_FrameBuffer[0] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[0]->SetDepthBuffer();

		m_FrameBuffer[1] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[1]->SetDepthBuffer();
	}

	PerObjectMotionBlur::~PerObjectMotionBlur()
	{
		glfwSetWindowAttrib(glfwGetCurrentContext(), GLFW_RESIZABLE, GLFW_TRUE);
	}

	void PerObjectMotionBlur::OnUpdate(double deltaTime)
	{
		m_DeltaTime = deltaTime;
		m_SumTime += m_DeltaTime;

		if (m_SphereChanged) {
			if (m_Smooth)
				m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Smooth);
			else
				m_Sphere = make_unique<Sphere>(m_Sektors, m_Stack, 2, Surface::Flat);

			m_View = m_Camera.getView();
			m_Model = m_Sphere->getModelMatrix();
			m_prevMVP = m_Proj * m_View * m_Model;

			m_VertexBuffer->UpdateData(m_Sphere->getVertices().data(), m_Sphere->getSize());
			m_IndexBuffer->UpdateBuffer(m_Sphere->getIndecies().data(), m_Sphere->getIndecies().size());
			m_SphereChanged = 0;
		}

		m_Sphere->rotate(0, 10*deltaTime , 0);
		m_Sphere->setPosition(glm::sin(m_SumTime*3)*2, 0, 0);

		m_View = m_Camera.getView();
		m_Model = m_Sphere->getModelMatrix();
		m_MVP = m_Proj * m_View * m_Model;
	}

	void PerObjectMotionBlur::OnRender()
	{
		m_Program[0]->UseProgram();
		m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[0]->SetUniformMatrix4fv("u_prevMVP", m_prevMVP);

		m_FrameBuffer[0]->BindTexture(1);
		m_FrameBuffer[0]->Bind();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_FrameBuffer[0]->Unbind();

		m_Texture->Bind(0);

		m_FrameBuffer[1]->BindTexture(2);
		m_Program[1]->UseProgram();
		m_Program[1]->SetUniform1i("u_texture", 0);
		m_Program[1]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[1]->SetUniformMatrix4fv("u_Model", m_Model);
		m_Program[1]->SetUniform3fv("u_CameraPos", m_Camera.getPosition());

		m_FrameBuffer[1]->Bind();
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_FrameBuffer[1]->Unbind();

		m_FrameBuffer[0]->BindTexture(1);
		m_FrameBuffer[1]->BindTexture(2);

		m_Program[2]->UseProgram();
		m_Program[2]->SetUniform1i("u_texture", 2);
		m_Program[2]->SetUniform1i("u_velocityBuffer", 1);
		m_Program[2]->SetUniform1i("u_blurLevel", m_MotionBlurLevel);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_prevMVP = m_MVP;
	}

	void PerObjectMotionBlur::OnImGuiRender()
	{
		if (ImGui::SliderInt("Sphere secttors", &m_Sektors, 3, 100, "%d")) m_SphereChanged = 1;
		if (ImGui::SliderInt("Sphere stack", &m_Stack, 3, 100, "%d")) m_SphereChanged = 1;
		
		ImGui::SliderInt("Motion Blur Level", &m_MotionBlurLevel, 1, 100, "%d");
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}