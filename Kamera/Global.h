#pragma once
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <iostream>

const int PATH_APPROXIMATION = 100;
const int SHADER_POSITION = 0;
const int SHADER_NORMAL = 1;
const int SHADER_TEXTURE = 2;
const int KEY_COUNT = 512;
const float KEY_SENSITIVITY = 0.05f;
const float MOUSE_SENSITIVITY = 0.005f;

static GLuint WIDTH = 1920;
static GLuint HEIGHT = 1080;

#ifdef _DEBUG
	#define glCheckError() glCheckError_(__FILE__, __LINE__) 
#else
#define glCheckError() 
#endif

struct LightIndexer
{
	LightIndexer() {}
	LightIndexer(int textureOffset) : TextureIndex(textureOffset) {}

	int DirIndex = 0;
	int PointIndex = 0;
	int TextureIndex = 0;
};

inline glm::quat MakeQuad(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	return glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll)));
}

GLenum static glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

