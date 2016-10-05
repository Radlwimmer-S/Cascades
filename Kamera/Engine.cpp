#include "Engine.h"
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

Engine::Engine(GLFWwindow& window, Shader& shader, Scene& scene, Camera& camera): m_shader(shader), m_scene(scene), m_window(window), m_camera(camera)
{
}


Engine::~Engine()
{
}

GLFWwindow* Engine::InitWindow(GLint width, GLint height, const char* title)
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();
	
	glGetError(); // Call it once to catch glewInit()

	// OpenGL configuration
	glViewport(0, 0, width, height);
	/*glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	return window;
}

void Engine::Start()
{
	m_scene.SetState(Running);
	Loop();
}

void Engine::Resume()
{
	m_scene.SetState(Running);
}

void Engine::Pause()
{
	m_scene.SetState(Paused);
}

void Engine::Stop()
{
	m_scene.SetState(Stopped);
}

void Engine::Loop()
{
	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	
	// Game loop
	while (!glfwWindowShouldClose(&m_window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		m_scene.ProcessInput(deltaTime);

		m_camera.Update(deltaTime);

		m_scene.Update(deltaTime);

		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader.Use();

		glm::mat4 view = m_camera.GetViewMatrix();
		GLuint viewLocation = glGetUniformLocation(m_shader.Program, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 proj = m_camera.GetProjectionMatrix();
		GLuint projLocation = glGetUniformLocation(m_shader.Program, "projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

		m_scene.Render(m_shader);

		m_camera.Render(m_shader);

		glfwSwapBuffers(&m_window);

	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

// Is called whenever a key is pressed/released via GLFW
void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
