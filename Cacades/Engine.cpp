#include "Engine.h"
#include <GL/glew.h>
#include "Shader.h"
#include "Camera.h"
#include "Global.h"
#include <algorithm>
#include <sstream>
#include "Timer.h"
#include <glm/gtc/type_ptr.hpp>

Engine::Engine(GLFWwindow& window) : m_window(window), m_mcShader(nullptr), m_generator(1), m_activeLayer(0)
{
}

Engine::~Engine()
{
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
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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
	if (fullscreen)
		glfwMaximizeWindow(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	glGetError(); // Call it once to catch glewInit()

	// OpenGL configuration
	glViewport(0, 0, WIDTH, HEIGHT);
	//glEnable(GL_CULL_FACE);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	return window;
}

void Engine::Init(char* windowTitle)
{
	m_instance = new Engine(*InitWindow(windowTitle, true));
}

void Engine::Init(char* windowTitle, GLuint width, GLuint height)
{
	WIDTH = width;
	HEIGHT = height;
	m_instance = new Engine(*InitWindow(windowTitle, false));
}

void Engine::Start(Camera* camera, Shader* mcShader, Hud* hud)
{
	m_mcShader = mcShader;
	m_hud = hud;
	m_camera = camera;

#define TIME
#ifdef TIME
	Timer::TimeAndPrint("Generate Texture", Delegate(&ProcedualGenerator::Generate3dTexture, &m_generator, 0));
	Timer::TimeAndPrint("Generate Texture", Delegate(&ProcedualGenerator::GenerateVBO, &m_generator, glm::vec3(m_generator.WIDTH, m_generator.LAYERS, m_generator.DEPTH)));
#else
	m_generator.Generate3dTexture(0);
	m_generator.GenerateVBO(glm::vec3(m_generator.WIDTH, m_generator.LAYERS, m_generator.DEPTH));
#endif

	glCheckError();
	m_lookupTable.WriteLookupTablesToGpu();
	m_lookupTable.UpdateUniforms(*m_mcShader);

	// Create transform feedback buffer
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(glm::vec3) * m_generator.GetVertexCount(), nullptr, GL_STATIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckError();

	Loop();
}

void Engine::DrawActiveLayer() const
{
	system("cls");
	std::stringstream ss;
	for (int y = 0; y < m_generator.DEPTH; ++y)
	{
		ss << '|';
		for (int x = 0; x < m_generator.WIDTH; ++x)
		{
			ss << (m_generator.GetValue(m_activeLayer, y, x) >= 0 ? 'X' : ' ');
		}
		ss << '|' << std::endl;
	}
	ss << "==================================================================================================" << std::endl;
	std::cout << ss.str();
}

void Engine::RenderMcMesh(Shader& shader)
{
	shader.Use();

	m_generator.SetUniforms(shader);
	glCheckError();

	int numAttribs;
	glGetProgramiv(shader.Program, GL_ACTIVE_ATTRIBUTES, &numAttribs);
	
	// Create query object to collect info
	//GLuint query;
	//glGenQueries(1, &query);

	// Perform feedback transform
	glEnable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

	glBeginTransformFeedback(GL_TRIANGLES);
		//glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
			glBindVertexArray(m_generator.GetVaoId());
			glDrawArrays(GL_POINTS, 0, m_generator.GetVertexCount());
		//glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glEndTransformFeedback();
	glBindVertexArray(0);
	glCheckError();

	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	glDisable(GL_RASTERIZER_DISCARD);

	glFlush();

	// Fetch and print results
	//GLuint primitives = 0;
	//glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

	//std::cout << "INOF::MC Generated " << primitives << " primitives." << std::endl;
	glCheckError();
}

void Engine::Update(GLfloat deltaTime, int fps)
{
}

void Engine::Loop()
{
	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat second = 0.0f;
	int frame = 0;
	int fps = 0;

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

		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(.5f, .5f, .5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		m_camera->ProcessInput(m_window);
		m_camera->Update(deltaTime);

		RenderMcMesh(*m_mcShader);

		glfwSwapBuffers(&m_window);
		glCheckError();

		++frame;

		if (second > 1)
		{
			fps = frame;
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
			m_activeLayer = std::max(0, m_activeLayer - 1);
			DrawActiveLayer();
			break;
		case GLFW_KEY_KP_7:
			m_activeLayer = std::min(m_generator.LAYERS - 1, m_activeLayer + 1);
			DrawActiveLayer();
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

void Engine::m_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

Engine* Engine::m_instance = nullptr;
