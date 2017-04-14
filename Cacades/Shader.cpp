#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "Shlwapi.h"
#include "Pathcch.h"
#include "Global.h"
#include <chrono>

Shader::Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath) : Program(0), m_delegate(&Shader::SetDirty, this), m_isTempValid(false), m_isValid(false), m_isDirty(true)
{
	m_sourceFiles[0] = vertexPath;
	m_sourceFiles[1] = geometryPath;
	m_sourceFiles[2] = fragmentPath;
	m_watcher = new FileWatcher(m_sourceFiles, MAX_FILES, m_delegate);
	Load();
}

void Shader::Load()
{
	GLuint program = glCreateProgram();
	GLuint vertex, fragment = 0, geometry = 0;
	GLint success;
	//GLchar infoLog[512];

	m_isTempValid = true;
	m_isDirty = false;

	vertex = LoadShader(m_sourceFiles[0], GL_VERTEX_SHADER, m_isTempValid);
	glAttachShader(program, vertex);

	if (m_sourceFiles[1] != nullptr)
	{
		geometry = LoadShader(m_sourceFiles[1], GL_GEOMETRY_SHADER, m_isTempValid);
		glAttachShader(program, geometry);
	}

	if (m_sourceFiles[2] != nullptr)
	{
		fragment = LoadShader(m_sourceFiles[2], GL_FRAGMENT_SHADER, m_isTempValid);
		glAttachShader(program, fragment);
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (m_isTempValid)
	{
		if (success)
		{
			glDeleteProgram(Program);
			Program = program;
			m_isValid = true;
		}
		else
		{
			m_isTempValid = false;
			// Print linking errors if any
			GLint errorLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLength);
			glCheckError();
			char* infoLog = new GLchar[errorLength];
			glGetProgramInfoLog(program, errorLength, nullptr, infoLog);
			glCheckError();
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteProgram(program);
		}
	}
	else
		glDeleteProgram(program);


	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);
	glCheckError();
}

std::string Shader::ReadFile(const GLchar* shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	// ensures ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		shaderFile.open(shaderPath);

		if (!shaderFile.good())
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << shaderPath << std::endl;
			std::cin.ignore();
		}

		std::stringstream shaderStream;
		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// Convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << shaderPath << std::endl;
	}

	return shaderCode;
}

GLuint Shader::LoadShader(const GLchar* shaderPath, GLenum shaderType, bool& isValid)
{
	std::string shaderCode = ReadFile(shaderPath);
	HandleIncludes(shaderCode, shaderPath);

	const GLchar* vShaderCode = shaderCode.c_str();
	// 2. Compile shader
	GLuint shader;
	GLint success;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &vShaderCode, nullptr);
	glCompileShader(shader);
	// Print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		// Print linking errors if any
		GLint errorLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);
		char* infoLog = new GLchar[errorLength];
		glGetShaderInfoLog(shader, errorLength, nullptr, infoLog);
		std::cout << "ERROR::SHADER::" << GetShaderName(shaderType) << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		delete[] infoLog;
		isValid = false;
	}

	return shader;
}

void Shader::HandleIncludes(std::string& shaderCode, const GLchar* shaderPath)
{
	GLchar parentPath[MAX_PATH];
	_splitpath_s(shaderPath, nullptr, 0, parentPath, MAX_PATH, nullptr, 0, nullptr, 0);

	char includeText[] = "#pragma include \"";
	int textLength = sizeof(includeText) - 1;

	for (size_t includeStart = shaderCode.find(includeText, 0); includeStart != std::string::npos; includeStart = shaderCode.find(includeText, 0))
	{
		size_t includeEnd = shaderCode.find("\"", includeStart + textLength);
		std::string filePath(parentPath);
		std::string fileName = shaderCode.substr(includeStart + textLength, (includeEnd)-(includeStart + textLength));

		filePath.append(fileName);
		std::string includeCode = ReadFile(filePath.c_str());
		shaderCode.replace(includeStart, (includeEnd + 1) - includeStart, includeCode);
	}
}

GLchar* Shader::GetShaderName(GLenum shaderType)
{
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		return "VERTEX";
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT";
	case GL_GEOMETRY_SHADER:
		return "GEOMETRY";
	default:
		return "UNKNOWN";
	}
}

void Shader::Use()
{
	if (m_isDirty)
	{
		std::cout << "INFO::SHADER::HOT_RELOAD" << std::endl;
		Load();
		if (!m_isTempValid)
		{
			std::cout << "INFO::SHADER::HOT_RELOAD_ABORTED" << std::endl;
		}
	}

	glUseProgram(Program);
}

bool Shader::IsValid() const
{
	return  m_isValid;
}

bool Shader::IsDirty() const
{
	return m_isDirty;
}

void Shader::IsDirty(bool dirty)
{
	m_isDirty = dirty;
}

void Shader::SetDirty()
{
	m_isDirty = true;
}

const GLchar*const* Shader::GetSourceFiles() const
{
	return m_sourceFiles;
}
