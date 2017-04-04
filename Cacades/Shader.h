#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader
{

public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	std::string ReadFile(const GLchar* shaderPath);
	void Use() const;
	bool IsValid() const;

private:
	void HandleIncludes(std::string& shaderCode, const GLchar* shaderPath);
	GLuint LoadShader(const GLchar* shaderPath, GLenum shaderType);
	static GLchar* GetShaderName(GLenum shaderType);
	bool m_isValid;
};

#endif