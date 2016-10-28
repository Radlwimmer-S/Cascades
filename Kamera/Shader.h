#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader
{

public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void Use() const;
	bool IsValid() const;

private:
	GLuint LoadShader(const GLchar* shaderPath, GLenum shaderType);
	static GLchar* GetShaderName(GLenum shaderType);
	bool m_isValid;
};

#endif