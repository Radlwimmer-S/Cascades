#include "Engine.h"
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Global.h"

Engine::Engine(char* windowTitle, bool fullscreen) : m_shader(nullptr), m_scene(nullptr), m_camera(nullptr), m_window(*InitWindow(windowTitle, fullscreen))
{}

Engine::~Engine()
{
	delete m_shader;
	m_shader = nullptr;
	delete m_scene;
	m_scene = nullptr;
	delete m_camera;
	m_camera = nullptr;
	glfwSetWindowShouldClose(&m_window, GL_TRUE);
}

GLFWwindow* Engine::InitWindow(const char* windowTitle, bool fullscreen)
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* monitor = nullptr;
	if (fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode * mode = glfwGetVideoMode(monitor);
		WIDTH = mode->width;
		HEIGHT = mode->height;
	}

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, windowTitle, monitor, nullptr);
	glfwMakeContextCurrent(window);
	glfwMaximizeWindow(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	glGetError(); // Call it once to catch glewInit()

	// OpenGL configuration
	glViewport(0, 0, WIDTH, HEIGHT);
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
	m_scene->SetState(Running);
	Loop();
}

void Engine::Resume()
{
	m_scene->SetState(Running);
}

void Engine::Pause()
{
	m_scene->SetState(Paused);
}

void Engine::Stop()
{
	m_scene->SetState(Stopped);
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

		m_camera->Update(deltaTime);

		m_scene->Update(deltaTime);

		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader->Use();

		glm::mat4 view = m_camera->GetViewMatrix();
		GLuint viewLocation = glGetUniformLocation(m_shader->Program, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 proj = m_camera->GetProjectionMatrix();
		GLuint projLocation = glGetUniformLocation(m_shader->Program, "projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

		m_scene->Render(*m_shader);

		m_camera->Render(*m_shader);

		glfwSwapBuffers(&m_window);

	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void Engine::Init(char* windowTitle)
{
	m_instance = new Engine(windowTitle, true);
}

void Engine::Init(char* windowTitle, GLuint width, GLuint height)
{
	WIDTH = width;
	HEIGHT = height;
	m_instance = new Engine(windowTitle, false);
}

void Engine::SetShader(Shader& shader)
{
	m_shader = &shader;
}

void Engine::SetScene(Scene& scene)
{
	m_scene = &scene;
}

void Engine::SetCamera(Camera& camera)
{
	m_camera = &camera;
}

// Is called whenever a key is pressed/released via GLFW
void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	m_instance->m_camera->KeyCallback(key, scancode, action, mode);
}

void Engine::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	m_instance->m_camera->CursorPosCallback(x, y);
}

Engine* Engine::m_instance = nullptr;