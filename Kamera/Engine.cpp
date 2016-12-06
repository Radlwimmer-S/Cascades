#include "Engine.h"
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Global.h"
#include <algorithm>

Engine::Engine(char* windowTitle, bool fullscreen) : m_shader(nullptr), m_scene(nullptr), m_window(*InitWindow(windowTitle, fullscreen)), m_camera(nullptr), m_activeObject(-1)
{
	glGenFramebuffers(1, &m_framebuffer);
	glGenRenderbuffers(1, &m_multisampleTex);
	glGenRenderbuffers(1, &m_renderbuffer);
	SetAASettings();
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
	/*
	GLint fMSSamples = 16, fMSCoverageSamples = 8;
	if (GLEW_NV_framebuffer_multisample_coverage)
	{
		GLint n_modes;
		glGetIntegerv(GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV, &n_modes);
		GLint *modes = new GLint[2 * n_modes];
		glGetIntegerv(GL_MULTISAMPLE_COVERAGE_MODES_NV, modes);

		for (int i = 0; i < n_modes; ++i)
		{
			if (modes[i * 2 + 1] == fMSSamples && modes[i * 2] > fMSCoverageSamples)
				fMSCoverageSamples = modes[i * 2];
		}

		delete[] modes;
	}*/

	// OpenGL configuration
	glViewport(0, 0, WIDTH, HEIGHT);
	//glEnable(GL_CULL_FACE);
	/*glEnable(GL_BLEND);
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

void Engine::PrintData(int fps) const
{
#ifdef _DEBUG
	system("cls");
	std::cout << "FPS: " << fps << std::endl;
	std::cout << "Samples: " << m_samples << std::endl;
	std::cout << "AAMode: " << m_aaMode << std::endl;
#endif
}

void Engine::SetAASettings() const
{	
	// Framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glCheckError();	

	// Create a multisampled color attachment
	glBindRenderbuffer(GL_RENDERBUFFER, m_multisampleTex);
	glCheckError();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_RGBA8, WIDTH, HEIGHT);
	glCheckError();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_multisampleTex);
	glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glCheckError();
	
	// Create a renderbuffer object for depth and stencil attachments
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
	glCheckError();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glCheckError();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glCheckError();	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();

	glEnable(GL_MULTISAMPLE);

	if (m_samples == 1)
	{
		glDisable(GL_MULTISAMPLE);
		return;
	}

	switch (m_aaMode)
	{
	case Fastest:
		glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
		glCheckError();
		break;
	case Nicest:
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glCheckError();
		break;
	case NV_Fastest:
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);
		glCheckError();
		break;
	case NV_Nicest:
		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
		glCheckError();
		break;
	}
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
		else if (m_activeObject == 0)
		{
			m_camera->ProcessInput(m_window);
			m_lights[0]->SetPosition(m_camera->GetPosition() + glm::normalize(glm::vec3(0, -.5f, -1) * m_camera->GetOrientation()));
			m_lights[0]->SetOrientation(m_camera->GetOrientation());
		}
		else
			m_lights[m_activeObject]->ProcessInput(m_window);

		m_camera->Update(deltaTime);

		m_scene->Update(deltaTime);
		
		RenderLights();

		// 1. Draw scene as normal in multisampled buffers
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glClearColor(.1f, .1f, .1f, 1.0f);
		glCheckError();

		RenderScene();

		m_camera->Render(*m_shader);

		// 2. Now blit multisampled buffer(s) to default framebuffers		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer); // Make sure your multisampled FBO is the read 
		glCheckError();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Make sure no FBO is set as the draw 
		glCheckError();
		glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glCheckError();

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
	for (int i = 0; i < m_lights.size(); ++i)
	{
		m_lights[i]->UpdateUniforms(*m_shader, i, MaxTexturesPerModel + i);
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

	GLuint bumbinessLocation = glGetUniformLocation(m_shader->Program, "bumbiness");
	glUniform1f(bumbinessLocation, m_bumpiness);
	glCheckError();

	GLuint softShadowLocation = glGetUniformLocation(m_shader->Program, "SoftShadows");
	glUniform1f(softShadowLocation, m_softShadows);
	glCheckError();
}

void Engine::RenderLights() const
{
	glCullFace(GL_FRONT);
	for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
	{
		(*it)->PreRender();
		m_scene->Render((*it)->GetShadowShader());
		(*it)->PostRender();
	}
	glCullFace(GL_BACK);
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
			if (index >= m_instance->m_lights.size())
				index = -1;
			m_instance->m_activeObject = index;
		} break;

		case GLFW_KEY_F1:
			m_instance->m_camera->SetMode(Overview);
			break;
		case GLFW_KEY_F2:
			m_instance->m_camera->SetMode(Automatic);
			break;
		case GLFW_KEY_F3:
			m_instance->m_camera->SetMode(Manual);
			break;

		case GLFW_KEY_DELETE:
		{
			int index = m_instance->m_activeObject;
			if (index != -1)
			{
				Light* activeLight = m_instance->m_lights[index];
				activeLight->IsEnabled(!activeLight->IsEnabled());
			}
		} break;

		case GLFW_KEY_KP_1:
			m_instance->m_bumpiness = std::fmaxf(0, m_instance->m_bumpiness - 0.1f);
			break;
		case GLFW_KEY_KP_7:
			m_instance->m_bumpiness = std::fminf(2, m_instance->m_bumpiness + 0.1f);
			break;

		case GLFW_KEY_KP_2:
			m_instance->m_samples = (m_instance->m_samples / 2 > 0) ? m_instance->m_samples / 2 : 16;
			m_instance->SetAASettings();
			break;
		case GLFW_KEY_KP_8:
			m_instance->m_samples = (m_instance->m_samples * 2 <= 16) ? m_instance->m_samples * 2 : 1;
			m_instance->SetAASettings();
			break;

		case GLFW_KEY_KP_3:
			m_instance->m_aaMode = static_cast<AAMode>((m_instance->m_aaMode - 1 + AAModeCount) % AAModeCount);
			m_instance->SetAASettings();
			break;
		case GLFW_KEY_KP_9:
			m_instance->m_aaMode = static_cast<AAMode>((m_instance->m_aaMode + 1) % AAModeCount);
			m_instance->SetAASettings();
			break;

		case GLFW_KEY_ENTER:
			m_instance->m_softShadows = !m_instance->m_softShadows;
			break;
		}
}

void Engine::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	m_instance->m_camera->CursorPosCallback(x, y);
}

void Engine::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
	{
		glm::vec3 cameraPos = m_instance->m_camera->GetPosition();
		glm::vec3 cameraRot = glm::eulerAngles(m_instance->m_camera->GetOrientation());
		std::cout << "path->push_back(ControlPoint(glm::vec3(" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z
			<< "), MakeQuad(" << glm::degrees(cameraRot.x) << ", " << glm::degrees(cameraRot.y) << ", " << glm::degrees(cameraRot.z) << ")));" << std::endl;
	}
}

Engine* Engine::m_instance = nullptr;
