#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Scene.h"

class Camera;
class Shader;

class Engine
{
public:
	Engine(GLFWwindow& window, Shader& shader, Scene& scene, Camera& camera);
	~Engine();

	static GLFWwindow* InitWindow(GLint width, GLint height, const char* title);
	void Start();
	void Resume();
	void Pause();
	void Stop();
protected:
	void Loop();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	// Window dimensions
	const GLuint WIDTH = 800, HEIGHT = 600;
	Shader& m_shader;
	Scene& m_scene;
	GLFWwindow& m_window;
	Camera& m_camera;
};

