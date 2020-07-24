#include "PerObjectMotionBlur.h"

#include "GLFW/glfw3.h"

namespace test {

	PerObjectMotionBlur::PerObjectMotionBlur()
	{
		GLFWwindow* window = glfwGetCurrentContext();
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		glfwGetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

		const float data[]{
			-1.0f, -1.0f,  1.0f, 0.0f,  0.5f,
			 1.0f,  1.0f,  1.0f, 0.25f, 0.75f,
			-1.0f,  1.0f,  1.0f, 0.0f,  0.75f,
			-1.0f, -1.0f,  1.0f, 0.0f,  0.5f,
			 1.0f, -1.0f,  1.0f, 0.25f, 0.5f,
			 1.0f,  1.0f,  1.0f, 0.25f, 0.75f,
			 1.0f, -1.0f,  1.0f, 0.25f, 0.5f,
			 1.0f,  1.0f, -1.0f, 0.5f,  0.75f,
			 1.0f,  1.0f,  1.0f, 0.25f, 0.75f,
			 1.0f, -1.0f,  1.0f, 0.25f, 0.5f,
			 1.0f, -1.0f, -1.0f, 0.5f,  0.5f,
			 1.0f,  1.0f, -1.0f, 0.5f,  0.75f,
			 1.0f, -1.0f, -1.0f, 0.5f,  0.5f,
			-1.0f,  1.0f, -1.0f, 0.75f, 0.75f,
			 1.0f,  1.0f, -1.0f, 0.5f,  0.75f,
			 1.0f, -1.0f, -1.0f, 0.5f,  0.5f,
			-1.0f, -1.0f, -1.0f, 0.75f, 0.5f,
			-1.0f,  1.0f, -1.0f, 0.75f, 0.75f,
			-1.0f, -1.0f, -1.0f, 0.75f, 0.5f,
			-1.0f,  1.0f,  1.0f, 1.0f,  0.75f,
			-1.0f,  1.0f, -1.0f, 0.75f, 0.75f,
			-1.0f, -1.0f, -1.0f, 0.75f, 0.5f,
			-1.0f, -1.0f,  1.0f, 1.0f,  0.5f,
			-1.0f,  1.0f,  1.0f, 1.0f,  0.75f,
			-1.0f,  1.0f,  1.0f, 0.0f,  0.75f,
			 1.0f,  1.0f, -1.0f, 0.25f, 1.0f,
			-1.0f,  1.0f, -1.0f, 0.0f,  1.0f,
			-1.0f,  1.0f,  1.0f, 0.0f,  0.75f,
			 1.0f,  1.0f,  1.0f, 0.25f, 0.75f,
			 1.0f,  1.0f, -1.0f, 0.25f, 1.0f,
			-1.0f, -1.0f,  1.0f, 0.0f,  0.25f,
			-1.0f, -1.0f, -1.0f, 0.0f,  0.5f,
			 1.0f, -1.0f, -1.0f, 0.25f, 0.5f,
			-1.0f, -1.0f,  1.0f, 0.0f,  0.25f,
			 1.0f, -1.0f, -1.0f, 0.25f, 0.5f,
			 1.0f, -1.0f,  1.0f, 0.25f, 0.25f
		};

		m_VAO = std::make_unique<VertexArrayObject>();
		m_VAO->Bind();

		m_VertexBuffer = std::make_unique<VertexBuffer>(&data, sizeof(data));

		VertexBufferLayout layout;

		layout.Push<GLfloat>(3);
		layout.Push<GLfloat>(2);

		m_VAO->AddVertexBuffer(*m_VertexBuffer, layout);

		Shader vertexShader = Shader(GL_VERTEX_SHADER, "src/shaders/VelocityBuffer_Vertex.shader");
		Shader fragmentShader = Shader(GL_FRAGMENT_SHADER, "src/shaders/VelocityBuffer_Fragment.shader");

		m_Program[0] = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);

		Shader vertexShader2 = Shader(GL_VERTEX_SHADER, "src/shaders/VertexShader_texture.shader");
		Shader fragmentShader2 = Shader(GL_FRAGMENT_SHADER, "src/shaders/FragmentShader_texture.shader");

		m_Program[1] = std::make_unique<ShaderProgram>(vertexShader2, fragmentShader2);

		Shader vertexShader3 = Shader(GL_VERTEX_SHADER, "src/shaders/RenderTexture.vertex_shader");
		Shader fragmentShader3 = Shader(GL_FRAGMENT_SHADER, "src/shaders/PerObjectMotionBlur_Fragment.shader");

		m_Program[2] = std::make_unique<ShaderProgram>(vertexShader3, fragmentShader3);

		m_Texture = std::make_unique<Texture>("res/textures/grass.png");
		m_Texture->Bind(0);

		m_Proj = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 1.0f, 150.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
		m_Model = glm::mat4(1);

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);

		m_FrameBuffer[0] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[0]->SetDepthBuffer();

		m_FrameBuffer[1] = std::make_unique<FrameBuffer>(Attachment::COLOR_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_FrameBuffer[1]->SetDepthBuffer();
	}

	PerObjectMotionBlur::~PerObjectMotionBlur()
	{

	}

	void PerObjectMotionBlur::OnUpdate(double deltaTime)
	{

	}

	void PerObjectMotionBlur::OnRender()
	{
		m_MVP = m_Proj * m_View * m_Model;

		m_Texture->Bind(0);

		m_Program[0]->UseProgram();
		m_Program[0]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[0]->SetUniformMatrix4fv("u_prevMVP", glm::translate(m_MVP, glm::vec3(1, 0, 0)));

		m_FrameBuffer[0]->BindTexture(1);
		m_FrameBuffer[0]->Bind();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		m_FrameBuffer[0]->Unbind();

		m_Texture->Bind(0);

		m_FrameBuffer[1]->BindTexture(2);
		m_Program[1]->UseProgram();
		m_Program[1]->SetUniformMatrix4fv("u_MVP", m_MVP);
		m_Program[1]->SetUniform1i("u_texture", 0);
		m_FrameBuffer[1]->Bind();
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		m_FrameBuffer[1]->Unbind();

		m_FrameBuffer[0]->BindTexture(1);
		m_FrameBuffer[1]->BindTexture(2);

		m_Program[2]->UseProgram();
		m_Program[2]->SetUniform1i("u_texture", 2);
		m_Program[2]->SetUniform1i("u_velocityBuffer", 1);
		m_Program[2]->SetUniform1i("u_blurLevel", 30);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void PerObjectMotionBlur::OnImGuiRender()
	{

	}

}