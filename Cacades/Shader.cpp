#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "Shlwapi.h"
#include "Pathcch.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath) : m_isValid(true)
{
	GLuint vertex, fragment, geometry = 0;
	GLint success;
	GLchar infoLog[512];

	// Shader Program
	Program = glCreateProgram();

	vertex = LoadShader(vertexPath, GL_VERTEX_SHADER);
	glAttachShader(Program, vertex);

	fragment = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);
	glAttachShader(Program, fragment);

	if (geometryPath != nullptr)
	{
		geometry = LoadShader(geometryPath, GL_GEOMETRY_SHADER);
		glAttachShader(Program, geometry);
	}

	glLinkProgram(Program);
	// Print linking errors if any
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(Program, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		m_isValid = false;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
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

GLuint Shader::LoadShader(const GLchar* shaderPath, GLenum shaderType)
{
	std::string shaderCode = ReadFile(shaderPath);	
	HandleIncludes(shaderCode, shaderPath);

	const GLchar* vShaderCode = shaderCode.c_str();
	// 2. Compile shader
	GLuint shader;
	GLint success;
	GLchar infoLog[512];

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &vShaderCode, nullptr);
	glCompileShader(shader);
	// Print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::" << GetShaderName(shaderType) << "COMPILATION_FAILED\n" << infoLog << std::endl;
		m_isValid = false;
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
		std::string fileName = shaderCode.substr(includeStart + textLength, (includeEnd)- (includeStart + textLength));

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
		return "VERTEX::";
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT::";
	case GL_GEOMETRY_SHADER:
		return "GEOMETRY::";
	default:
		return "UNKNOWN";
	}
}

void Shader::Use() const
{
	glUseProgram(Program);
}

bool Shader::IsValid() const
{
	return  m_isValid;
}
