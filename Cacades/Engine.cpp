#include "Engine.h"
#include <GL/glew.h>
#include "Shader.h"
#include "Camera.h"
#include "Global.h"
#include <sstream>
#include "Timer.h"
#include <glm/gtc/type_ptr.hpp>
#include "Font.h"
#include "Hud.h"

Engine::Engine(GLFWwindow& window) : m_window(window), m_mcShader(nullptr), m_generator()
{

	m_shader = new  Shader("./shaders/Simple.vert", nullptr, "./shaders/Simple.frag");
	m_shader->Test("SimpleLight");
	
	Shader* hudShader = new Shader("./shaders/Text.vert", nullptr, "./shaders/Text.frag");
	hudShader->Test("Text/Hud");
	Font* font = new Font("fonts/arial.ttf", glm::ivec2(0, 24));
	m_hud = new Hud(*font, *hudShader);

	m_camera = new Camera();
}

Engine::~Engine()
{
	glfwSetWindowShouldClose(&m_window, GL_TRUE);
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

GLFWwindow* Engine::InitWindow(const char* windowTitle, bool fullscreen)
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* monitor = nullptr;
	if (fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode * mode = glfwGetVideoMode(monitor);
		SCREEN_WIDTH = mode->width;
		SCREEN_HEIGHT = mode->height;
	}

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, monitor, nullptr);
	glfwMakeContextCurrent(window);
	if (fullscreen)
		glfwMaximizeWindow(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetWindowSizeCallback(window, ResizeCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	glGetError(); // Call it once to catch glewInit()

	glDebugMessageCallback(DebugCallback, NULL);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glEnable(GL_CULL_FACE);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	return window;
}

void Engine::Init(char* windowTitle)
{
	m_instance = new Engine(*InitWindow(windowTitle, true));
}

void Engine::Init(char* windowTitle, GLuint width, GLuint height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	m_instance = new Engine(*InitWindow(windowTitle, false));
}

void Engine::Start()
{
	m_renderInfo.Seed = 0;
	m_renderInfo.Resolution = glm::vec3(m_generator.WIDTH, m_generator.LAYERS, m_generator.DEPTH);
	m_renderInfo.StartLayer = 0;
	m_renderInfo.NoiseScale = 0.6f;

	m_generator.SetRandomSeed(m_renderInfo.Seed);
	m_generator.SetResolution(m_renderInfo.Resolution);
	m_generator.SetStartLayer(m_renderInfo.StartLayer);
	m_generator.SetNoiseScale(m_renderInfo.NoiseScale);

	m_generator.GenerateMcVbo();
	m_generator.Generate3dTexture();
	m_mesh = m_generator.GenerateMesh();

	Loop();
}

void Engine::RenderMcMesh(Shader& shader)
{
}

void Engine::Update(GLfloat deltaTime)
{
	m_camera->ProcessInput(m_window);
	m_camera->Update(deltaTime);

	m_hud->Update(m_updateInfo.FPS, m_renderInfo);
}

void Engine::RenderMesh()
{
	m_shader->Use();

	glm::vec3 viewPos = m_camera->GetPosition();
	GLuint viewPosLocation = glGetUniformLocation(m_shader->Program, "viewPos");
	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPos));
	glCheckError();

	glm::vec3 geomPos(0, 0, 0);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, geomPos);
	GLuint modelLocation = glGetUniformLocation(m_shader->Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glCheckError();

	glm::mat4 view = m_camera->GetViewMatrix();
	GLuint viewLocation = glGetUniformLocation(m_shader->Program, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glCheckError();

	glm::mat4 proj = m_camera->GetProjectionMatrix();
	GLuint projLocation = glGetUniformLocation(m_shader->Program, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
	glCheckError();

	glBindVertexArray(m_mesh->GetVAO());
	glCheckError();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh->GetTriCount() * 3);
	glCheckError();
	glBindVertexArray(0);
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

		Update(deltaTime);	

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClearColor(.5f, .5f, .5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();	

		RenderMesh();
		m_hud->Render();

		glfwSwapBuffers(&m_window);
		glCheckError();

		++frame;

		if (second > 1)
		{
			m_updateInfo.FPS = frame;
			frame = 0;
			second -= 1;
		}
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Instance()->m_KeyCallback(window, key, scancode, action, mode);
}

// Is called whenever a key is pressed/released via GLFW
void Engine::m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		switch (key)
		{
		case GLFW_KEY_KP_1:
			m_renderInfo.StartLayer -= 5;
			m_generator.SetStartLayer(m_renderInfo.StartLayer);
			m_generator.Generate3dTexture();
			m_mesh = m_generator.GenerateMesh();
			break;
		case GLFW_KEY_KP_7:
			m_renderInfo.StartLayer += 5;
			m_generator.SetStartLayer(m_renderInfo.StartLayer);
			m_generator.Generate3dTexture();
			m_mesh = m_generator.GenerateMesh();
			break;

		case GLFW_KEY_KP_2:
			m_renderInfo.NoiseScale = std::max(0.2f, m_renderInfo.NoiseScale - 0.2f);
			m_generator.SetNoiseScale(m_renderInfo.NoiseScale);
			m_mesh = m_generator.GenerateMesh();
			break;
		case GLFW_KEY_KP_8:
			m_renderInfo.NoiseScale = std::min(4.0f, m_renderInfo.NoiseScale + 0.2f);
			m_generator.SetNoiseScale(m_renderInfo.NoiseScale);
			m_mesh = m_generator.GenerateMesh();
			break;

		case GLFW_KEY_KP_3:
			--m_renderInfo.Seed;
			m_generator.SetRandomSeed(m_renderInfo.Seed);
			m_generator.Generate3dTexture();
			m_mesh = m_generator.GenerateMesh();
			break;
		case GLFW_KEY_KP_9:
			++m_renderInfo.Seed;
			m_generator.SetRandomSeed(m_renderInfo.Seed);
			m_generator.Generate3dTexture();
			m_mesh = m_generator.GenerateMesh();
			break;
		}
}

void Engine::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	Instance()->m_CursorPosCallback(window, x, y);
}

void Engine::m_CursorPosCallback(GLFWwindow* window, double x, double y)
{
	m_camera->CursorPosCallback(x, y);
}

void Engine::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Instance()->m_MouseButtonCallback(window, button, action, mods);
}

void Engine::ResizeCallback(GLFWwindow* window, int width, int height)
{
	Instance()->m_ResizeCallback(window, width, height);
}

void Engine::m_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void Engine::m_ResizeCallback(GLFWwindow* window, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

Engine* Engine::m_instance = nullptr;

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	char* sourceString = "", *typeString = "", *severityString = "";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:			sourceString = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceString = "WINDOW_SYSTEM"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:	sourceString = "THIRD_PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION:	sourceString = "APPLICATION"; break;
	case GL_DEBUG_SOURCE_OTHER:			sourceString = "OTHER"; break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				typeString = "ERROR"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	typeString = "DEPRECATED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	typeString = "UNDEFINED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_PORTABILITY:			typeString = "PORTABILITY"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:			typeString = "PERFORMANCE"; break;
	case GL_DEBUG_TYPE_MARKER:				typeString = "MARKER"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			typeString = "PUSH_GROUP"; break;
	case GL_DEBUG_TYPE_POP_GROUP:			typeString = "POP_GROUP"; break;
	case GL_DEBUG_TYPE_OTHER:				typeString = "OTHER"; break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			severityString = "HIGH"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:			severityString = "MEDIUM"; break;
	case GL_DEBUG_SEVERITY_LOW:				severityString = "LOW"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	severityString = "NOTIFICATION"; break;
	}

	printf("%s::%s[%s](%d): %s\n", sourceString, typeString, severityString, id, message);
}
