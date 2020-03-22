#include "GL/glew.h"
#include "Shader.h"

class ShaderProgram
{
private:
	GLuint m_ID;
public:
	ShaderProgram(Shader vertexShader, Shader fragmentShader);
	~ShaderProgram();

	void UseProgram();
};

