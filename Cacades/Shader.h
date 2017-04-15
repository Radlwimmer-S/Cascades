#pragma once

#include <GL/glew.h>
#include <chrono>
#include "Delegate.h"
#include "FileWatcher.h"
typedef std::chrono::system_clock::time_point time_point;

class Shader
{

public:
	GLuint Program;
	void Load();
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* geometryPath = nullptr, const GLchar* fragmentPath = nullptr);
	Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath, const GLchar** transformFeedbackVariables, int variablesCount);
	void Use();
	bool IsValid() const;

	bool IsDirty() const;
	void IsDirty(bool dirty);
	void SetDirty();

	const GLchar*const* GetSourceFiles() const;

private:
	static void HandleIncludes(std::string& shaderCode, const GLchar* shaderPath);
	static std::string ReadFile(const GLchar* shaderPath);
	static GLuint LoadShader(const GLchar* shaderPath, GLenum shaderType, bool& isValid);
	static GLchar* GetShaderName(GLenum shaderType);

	static const int MAX_FILES = 3;

	const GLchar* m_sourceFiles[MAX_FILES];
	FileWatcher* m_watcher;
	const GLchar** m_transformFeedbackVariables;
	GLuint m_transformFeedbackVariablesCount;
	bool m_isTempValid, m_isValid, m_isDirty;

};
