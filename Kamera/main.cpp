#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"
#include "Engine.h"
#include "CameraScene.h"
#include <filesystem>
#include "Camera.h"
#include "Global.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

std::vector<ControlPoint>* GetPath()
{
	std::vector<ControlPoint>* path = new std::vector<ControlPoint>();
	path->push_back(ControlPoint(glm::vec3(10, 0, 12.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(10, 0, 10), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(7, 0, 7.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(0, 0, 5), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(-7, 0, 2.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(-10, 0, 0), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(-7, 0, -2.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(0, 0, -5), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(7, 0, -7.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(0, 0, -10), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	path->push_back(ControlPoint(glm::vec3(0, 0, -12.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f)));
	return path;
}

// Window dimensions

// The MAIN function, from here we start the application and run the game loop
int main()
{
	GLFWwindow* window = Engine::InitWindow(WIDTH, HEIGHT, "Test");

	// Build and compile our shader program
	Shader* shader = new Shader("./shaders/default.vs", "./shaders/default.frag");

	Scene* scene = new CameraScene();

	CameraPath* path = new CameraPath(*GetPath(), 15);
	Camera* camera = new Camera(path);

	Engine engine(*window, *shader, *scene, *camera);

	engine.Start();

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}