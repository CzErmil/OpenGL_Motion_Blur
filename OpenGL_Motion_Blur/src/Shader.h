#pragma once

#include "GL/glew.h"
#include <iostream>

using namespace std;

class Shader
{
private:
	GLuint m_ID;
	GLenum m_ShaderType;
	string m_ShaderSource;

	void CompileShader(string shaderCode);
	string ReadFile(string shaderSource);
	GLuint CheckCompileStatus();
public:
	Shader(GLenum shaderType, string shaderSource);
	~Shader();

	inline GLenum GetShaderID() { return m_ID; }
	inline GLenum GetShaderType() { return m_ShaderType; }
};

