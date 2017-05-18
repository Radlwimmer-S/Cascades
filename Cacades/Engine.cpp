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
#include "Light.h"

Engine::Engine(GLFWwindow& window) : m_window(window), m_generator(), m_activeObject(-1), m_mesh(nullptr)
{
	m_shader = new Shader("./shaders/TriPlanar.vert", nullptr, "./shaders/TriPlanar.frag");
	m_shader->Test("TriPlanar");

	m_debugShader = new Shader("./shaders/Simple.vert", nullptr, "./shaders/Simple.frag");
	m_debugShader->Test("Debug");

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
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	m_renderInfo.Seed = -7;
	m_renderInfo.Resolution = glm::vec3(m_generator.WIDTH, m_generator.LAYERS, m_generator.DEPTH);
	m_renderInfo.StartLayer = 0;
	m_renderInfo.NoiseScale = 0.6f;
	m_renderInfo.IsoLevel = 0;
	m_renderInfo.GeometryScale = glm::vec3(5, 10, 5);

	m_generator.SetRandomSeed(m_renderInfo.Seed);
	m_generator.SetResolution(m_renderInfo.Resolution);
	m_generator.SetStartLayer(m_renderInfo.StartLayer);
	m_generator.SetNoiseScale(m_renderInfo.NoiseScale);
	m_generator.SetIsoLevel(m_renderInfo.IsoLevel);
	m_generator.SetGeometryScale(m_renderInfo.GeometryScale);

	m_generator.GenerateMcVbo();
	m_generator.Generate3dTexture();
	m_mesh = m_generator.GenerateMesh();

	Loop();
}

void Engine::AddLight(Light& light)
{
	m_lights.push_back(&light);
}

void Engine::Update(GLfloat deltaTime)
{
	MoveActiveObject();

	m_hud->Update(m_updateInfo.FPS, m_renderInfo);

	if (!m_updateInfo.IsPaused)

	for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		if ((*it)->IsEnabled())
			(*it)->Update(deltaTime);
	}

	m_camera->Update(deltaTime);
}

void Engine::RenderScene()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(.1f, .1f, .1f, 1.0f);

	m_shader->Use();
	UpdateUniforms(*m_shader);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mesh->Render(*m_shader);

	m_debugShader->Use();
	UpdateUniforms(*m_debugShader);
	if (m_renderInfo.DrawLightPosition)
		for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
			if ((*it)->IsEnabled())
				(*it)->RenderDebug(*m_debugShader);
}

void Engine::RenderHud()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	m_hud->Render();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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

		glCheckError();

		RenderLights();

		RenderScene();

		RenderHud();

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

void Engine::RenderLights() const
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		if (!(*it)->CastsShadows() || !(*it)->IsEnabled())
			continue;

		(*it)->PreRender();
		m_mesh->Render((*it)->GetShadowShader());
		(*it)->PostRender();
	}
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}

void Engine::UpdateUniforms(const Shader& shader) const
{
	for (int i = 0; i < m_lights.size(); ++i)
	{
		m_lights[i]->UpdateUniforms(shader, i, MaxTexturesPerModel + i);
	}

	glm::vec3 viewPos = m_camera->GetPosition();
	GLuint viewPosLocation = glGetUniformLocation(shader.Program, "viewPos");
	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPos));
	glCheckError();

	glm::mat4 view = m_camera->GetViewMatrix();
	GLuint viewLocation = glGetUniformLocation(shader.Program, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glCheckError();

	glm::mat4 proj = m_camera->GetProjectionMatrix();
	GLuint projLocation = glGetUniformLocation(shader.Program, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
	glCheckError();
}

void Engine::MoveActiveObject()
{
	if (m_activeObject == -1)
		m_camera->ProcessInput(m_window);
	//else if (m_activeObject == 0)
	//{
	//	m_camera->ProcessInput(m_window);
	//	m_lights[0]->SetPosition(m_camera->GetPosition() + glm::vec3(0, -.5f, -1) * m_camera->GetOrientation());
	//	m_lights[0]->SetOrientation(m_camera->GetOrientation());
	//}
	else
		m_lights[m_activeObject]->ProcessInput(m_window);
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
		case GLFW_KEY_0:
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:
		case GLFW_KEY_4:
		case GLFW_KEY_5:
		case GLFW_KEY_6:
		case GLFW_KEY_7:
		case GLFW_KEY_8:
		case GLFW_KEY_9:
		{
			int index = key - GLFW_KEY_0 - 1;
			if (index >= m_lights.size())
				index = -1;
			m_activeObject = index;
		} break;

		case GLFW_KEY_P:
		{
			m_updateInfo.IsPaused = !m_updateInfo.IsPaused;
		} break;

		case GLFW_KEY_DELETE:
		{
			int index = m_activeObject;
			if (index != -1)
			{
				Light* activeLight = m_lights[index];
				activeLight->IsEnabled(!activeLight->IsEnabled());
			}
		} break;

		case GLFW_KEY_UP:
			if (m_renderInfo.Resolution.y < 4000)
				m_renderInfo.Resolution *= 2;// += glm::ivec3(6, 16, 6);
			m_generator.SetResolution(m_renderInfo.Resolution);
			m_generator.GenerateMcVbo();
			m_mesh = m_generator.GenerateMesh();
			break;
		case GLFW_KEY_DOWN:
			if (m_renderInfo.Resolution.y > 64)
				m_renderInfo.Resolution /= 2;// -= glm::ivec3(6, 16, 6);
			m_generator.SetResolution(m_renderInfo.Resolution);
			m_generator.GenerateMcVbo();
			m_mesh = m_generator.GenerateMesh();
			break;

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
