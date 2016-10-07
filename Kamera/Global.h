#pragma once
#include <glm/gtc/quaternion.hpp>

const int PATH_APPROXIMATION = 100;
const int SHADER_POSITION = 0;
const int SHADER_COLOR = 1;
const int SHADER_TEXTURE = 2;

static GLuint WIDTH = 1920;
static GLuint HEIGHT = 1080;

inline glm::quat MakeQuad(GLfloat pitch, GLfloat yaw, GLfloat rotate)
{
	return glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(rotate)));
}