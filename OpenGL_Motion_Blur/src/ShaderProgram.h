#pragma once

#include "Shader.h"

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <unordered_map>

class ShaderProgram
{
private:
	GLuint m_ID;
	std::unordered_map<std::string, GLint> m_UniformLocationCache;
	GLint GetUniformLocation(const std::string& name);
public:
	ShaderProgram();
	ShaderProgram(Shader vertexShader, Shader fragmentShader);
	~ShaderProgram();

	void UseProgram();

	void SetUniform1i(const std::string& name, GLuint x1);
	void SetUniform1f(const std::string& name, GLfloat x1);
	void SetUniform2f(const std::string& name, GLfloat x1, GLfloat x2);
	void SetUniform3f(const std::string& name, GLfloat x1, GLfloat x2, GLfloat x3);
	void SetUniform4f(const std::string& name, GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4);
	void SetUniformMatrix4fv(const std::string& name, glm::mat4 m);
};

