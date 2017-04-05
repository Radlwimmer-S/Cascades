#pragma once

#define NOMINMAX
#include <Windows.h>
#include <GL/glew.h>
#include <vector>

class Shader
{

public:
	GLuint Program;
	void Load();
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	void Use();
	bool IsValid() const;

	bool IsDirty() const;
	void IsDirty(bool dirty);

	const GLchar*const* GetSourceFiles() const;

	const FILETIME* GetLastBuildTime() const;

private:
	static void HandleIncludes(std::string& shaderCode, const GLchar* shaderPath);
	static std::string ReadFile(const GLchar* shaderPath);
	static GLuint LoadShader(const GLchar* shaderPath, GLenum shaderType, bool& isValid);
	static GLchar* GetShaderName(GLenum shaderType);

	const GLchar* m_sourceFiles[3];
	bool m_isValid, m_isDirty;
	FILETIME m_lastBuild;
};