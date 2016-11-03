#include "Engine.h"
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Global.h"

Engine::Engine(char* windowTitle, bool fullscreen) : m_shader(nullptr), m_scene(nullptr), m_window(*InitWindow(windowTitle, fullscreen)), m_camera(nullptr), m_activeObject(-1)
{
}

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
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	glEnable(GL_MULTISAMPLE);
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

void Engine::PrintData(int fps) const
{
	system("cls");
	glm::vec3 cameraPos = m_camera->GetPosition();
	std::cout << "Position: x=" << cameraPos.x << ", y=" << cameraPos.y << ", z=" << cameraPos.z << std::endl;
	glm::vec3 cameraRot = glm::eulerAngles(m_camera->GetOrientation());
	std::cout << "Orientation: Pitch=" << glm::degrees(cameraRot.x) << ", Yaw=" << glm::degrees(cameraRot.y) << ", Roll=" << glm::degrees(cameraRot.z) << std::endl;
	std::cout << "FPS: " << fps << std::endl;
}

void Engine::Loop()
{
	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat second = 0.0f;
	int frame = 0;

	// Game loop
	while (!glfwWindowShouldClose(&m_window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
#ifdef _DEBUG
		deltaTime = 1.0f / 60;
#endif
		second += deltaTime;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;

		if (m_activeObject == -1)
			m_camera->ProcessInput(m_window);
		else
			m_lights[m_activeObject]->ProcessInput(m_window);

		m_camera->Update(deltaTime);

		m_scene->Update(deltaTime);

		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//m_lights[1]->SetPosition(m_camera->GetPosition());

		RenderLights();

		RenderScene();

		m_camera->Render(*m_shader);

		glfwSwapBuffers(&m_window);

		++frame;

		if (second > 1)
		{
			PrintData(frame);
			frame = 0;
			second -= 1;
		}
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void Engine::UpdateUniforms() const
{
	LightIndexer indizes = LightIndexer(MaxTexturesPerModel);
	for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		(*it)->UpdateUniforms(*m_shader, indizes);
	}

	glm::vec3 cameraPos = m_camera->GetPosition();
	GLint viewPosLoc = glGetUniformLocation(m_shader->Program, "viewPos");
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	glCheckError();

	glm::mat4 view = m_camera->GetViewMatrix();
	GLuint viewLocation = glGetUniformLocation(m_shader->Program, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glCheckError();

	glm::mat4 proj = m_camera->GetProjectionMatrix();
	GLuint projLocation = glGetUniformLocation(m_shader->Program, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
	glCheckError();
}

void Engine::RenderLights() const
{
	for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		(*it)->PreRender();
		m_scene->Render((*it)->GetShadowShader());
		(*it)->PostRender();
	}
}

void Engine::RenderScene() const
{
	m_shader->Use();
	UpdateUniforms();
	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_scene->Render(*m_shader);
	if (true)
		for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
			(*it)->RenderDebug(*m_shader);
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

void Engine::AddLight(Light& light)
{
	m_lights.push_back(&light);
}

// Is called whenever a key is pressed/released via GLFW
void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS && key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
	{
		int index = key - GLFW_KEY_0 - 1;
		if (index >= m_instance->m_lights.size())
			index = -1;
		m_instance->m_activeObject = index;
	}

}

void Engine::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	m_instance->m_camera->CursorPosCallback(x, y);
}

Engine* Engine::m_instance = nullptr;