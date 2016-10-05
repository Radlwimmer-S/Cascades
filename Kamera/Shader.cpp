#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = LoadShader(vertexPath, GL_VERTEX_SHADER);
	fragment = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);

	// Shader Program
	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);
	// Print linking errors if any
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

GLuint Shader::LoadShader(const GLchar* shaderPath, GLenum shaderType)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string shaderCode;
	std::ifstream shaderFile;
	// ensures ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		shaderFile.open(shaderPath);
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
	}

	return shader;
}

GLchar* Shader::GetShaderName(GLenum shaderType)
{
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		return "VERTEX::";
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT::";
	default:
		return "";
	}
}

void Shader::Use() const
{
	glUseProgram(Program);
}
