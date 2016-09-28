#include "Engine.h"
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include "Plane.h"
#include "Texture.h"
#include <Soil/SOIL.h>

Engine::Engine(): m_shader(nullptr), m_scene(nullptr), m_window(nullptr)
{
}


Engine::~Engine()
{
}

void Engine::Init()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	m_window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	// Set the required callback functions
	glfwSetKeyCallback(m_window, KeyCallback);

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

void Engine::SetShader(Shader* shader)
{
	m_shader = shader;
}

void Engine::SetScene(Scene* scene)
{
	m_scene = scene;
}

void Engine::Loop()
{
	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	
	// Game loop
	while (!glfwWindowShouldClose(m_window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		m_scene->ProcessInput(deltaTime);

		// Update Game state
		m_scene->Update(deltaTime);

		// Render
		glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader->Use();

		m_scene->Render(*m_shader);

		glfwSwapBuffers(m_window);

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
