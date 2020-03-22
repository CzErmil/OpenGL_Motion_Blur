#include "Shader.h"
#include <fstream>
#include <string>

Shader::Shader(GLenum shaderType, string shaderSource):m_ShaderType(shaderType)
{
	m_ID = glCreateShader(shaderType);
	string shaderCode = ReadFile(shaderSource);
	CompileShader(shaderCode);
	CheckCompileStatus();
}

Shader::~Shader()
{
    glDeleteShader(m_ID);
}

void Shader::CompileShader(string shaderCode)
{
    const char* cShaderCode = shaderCode.c_str();
	glShaderSource(m_ID, 1, &cShaderCode, NULL);
	glCompileShader(m_ID);
}

string Shader::ReadFile(string shaderSource)
{
    string line, shaderCode;
    ifstream file(shaderSource);

    if (file.is_open())
    {
        while (getline(file, line))
        {
            shaderCode += line + "\n";
        }
    }
    else std::cout << "Unable to open file : " << shaderSource << "\n";

    file.close();

    return shaderCode;
}

GLuint Shader::CheckCompileStatus()
{
    int result;
    glGetShaderiv(m_ID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &length);
        char* error = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(m_ID, length, &length, error);
        std::cout << error << "\n";
        return -1;
    }
    return 1;
}