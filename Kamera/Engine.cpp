#include "Engine.h"
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Global.h"
#include <algorithm>
#include "Hud.h"
#include <iomanip>
#include "Box.h"
#include "Ray.h"
#include "HitResult.h"

Engine::Engine(char* windowTitle, bool fullscreen) : m_shader(nullptr), m_scene(nullptr), m_window(*InitWindow(windowTitle, fullscreen)), m_camera(nullptr), m_activeObject(-1),
m_hits{ Model(glm::vec3(0,0,0), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(.1f,.1f,.1f)), 12, glm::vec3(1,0,0), NoNormals),
		Model(glm::vec3(0,0,0), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(.1f,.1f,.1f)), 12, glm::vec3(1,0,0), NoNormals) }
{
	glGenFramebuffers(1, &m_framebuffer);
	m_aaInfo.Init();
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
	m_instance = new Engine(windowTitle, true);
}

void Engine::Init(char* windowTitle, GLuint width, GLuint height)
{
	WIDTH = width;
	HEIGHT = height;
	m_instance = new Engine(windowTitle, false);
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

void Engine::SetHud(Hud& hud)
{
	m_hud = &hud;
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

		//deltaTime = 0.001f;

		MoveActiveObject();

		m_camera->Update(deltaTime);
		m_scene->Update(deltaTime);
		m_hits[0].Update(deltaTime);
		m_hits[1].Update(deltaTime);
		m_hud->Update(fps, m_renderInfo, m_aaInfo);

		RenderLights();

		RenderScene();

		RenderHud();

		glfwSwapBuffers(&m_window);
		glCheckError();

		++frame;

		if (second > 1)
		{
			fps = frame;
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
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
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
	glUniform1f(bumbinessLocation, m_renderInfo.NormalMapFactor);
	glCheckError();

	GLuint softShadowLocation = glGetUniformLocation(m_shader->Program, "SoftShadows");
	glUniform1f(softShadowLocation, m_renderInfo.EnableSoftShadowing);
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
	// 1. Draw scene as normal in multisampled buffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glCheckError();

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(.05f, .05f, .05f, 1.0f);

	m_shader->Use();
	UpdateUniforms();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_scene->Render(*m_shader);

	if (m_renderInfo.DrawLightPosition)
		for (std::vector<Light*>::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
			(*it)->RenderDebug(*m_shader);

	m_hits[0].Render(*m_shader);
	m_hits[1].Render(*m_shader);

	m_camera->Render(*m_shader);

	// 2. Now blit multisampled buffer(s) to default framebuffers		
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer); // Make sure your multisampled FBO is the read 
	glCheckError();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Make sure no FBO is set as the draw 
	glCheckError();
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void Engine::RenderHud() const
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glViewport(0, 0, WIDTH, HEIGHT);
	m_hud->Render();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Engine::PrintData(int fps) const
{
}

void Engine::SetAASettings()
{
	glDisable(GL_MULTISAMPLE);

	// Framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glCheckError();

	// Create a renderbuffer object for color attachment
	glBindRenderbuffer(GL_RENDERBUFFER, m_aaInfo.ColorBuffer);
	glCheckError();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_aaInfo.Samples, GL_RGBA8, WIDTH, HEIGHT);
	glCheckError();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_aaInfo.ColorBuffer);
	glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glCheckError();

	// Create a renderbuffer object for depth and stencil attachments
	glBindRenderbuffer(GL_RENDERBUFFER, m_aaInfo.DepthBuffer);
	glCheckError();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_aaInfo.Samples, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glCheckError();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_aaInfo.DepthBuffer);
	glCheckError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glCheckError();

	glGetIntegerv(GL_SAMPLES, &m_aaInfo.ActiveSamples);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();

	glEnable(GL_MULTISAMPLE);
}

void Engine::MoveActiveObject()
{
	if (m_activeObject == -1)
		m_camera->ProcessInput(m_window);
	else if (m_activeObject == 0)
	{
		m_camera->ProcessInput(m_window);
		m_lights[0]->SetPosition(m_camera->GetPosition() + glm::vec3(0, -.5f, -1) * m_camera->GetOrientation());
		m_lights[0]->SetOrientation(m_camera->GetOrientation());
	}
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

		case GLFW_KEY_F1:
			m_camera->SetMode(Overview);
			break;
		case GLFW_KEY_F2:
			m_camera->SetMode(Automatic);
			break;
		case GLFW_KEY_F3:
			m_camera->SetMode(Manual);
			break;

		case GLFW_KEY_DELETE:
		{
			int index = m_activeObject;
			if (index != -1)
			{
				Light* activeLight = m_lights[index];
				activeLight->IsEnabled(!activeLight->IsEnabled());
			}
		} break;

		case GLFW_KEY_KP_1:
			m_renderInfo.NormalMapFactor = std::fmaxf(0, m_renderInfo.NormalMapFactor - 0.1f);
			break;
		case GLFW_KEY_KP_7:
			m_renderInfo.NormalMapFactor = std::fminf(2, m_renderInfo.NormalMapFactor + 0.1f);
			break;

		case GLFW_KEY_KP_2:
			--m_scene->TreeRenderDepth;
			break;
		case GLFW_KEY_KP_8:
			++m_scene->TreeRenderDepth;
			break;

		case GLFW_KEY_KP_3:
			m_aaInfo.Samples = (m_aaInfo.Samples - 1 >= 0) ? m_aaInfo.Samples - 1 : m_aaInfo.MaxSamples;
			SetAASettings();
			break;
		case GLFW_KEY_KP_9:
			m_aaInfo.Samples = (m_aaInfo.Samples + 1 <= m_aaInfo.MaxSamples) ? m_aaInfo.Samples + 1 : 0;
			SetAASettings();
			break;

		case GLFW_KEY_ENTER:
			m_renderInfo.EnableSoftShadowing = !m_renderInfo.EnableSoftShadowing;
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
	if ((button == GLFW_MOUSE_BUTTON_2 || button == GLFW_MOUSE_BUTTON_1) && action == GLFW_PRESS)
	{
		Ray ray(m_camera->GetPosition(), -(glm::vec3(0, 0, 1)* m_camera->GetOrientation()));

		KdNode* tree = m_scene->GetKdTree();

		HitResult result;
		if (!tree->IsHit(result, ray))
		{
			std::cout << "Miss!" << std::endl;
			return;
		}

		glm::vec3 hit = ray.GetOrigin() + ray.GetDirection() * result.Distance;

		std::cout << "Hit @ ";
		std::cout << hit.x << '/' << hit.y << '/' << hit.z << std::endl;

		m_hits[button - GLFW_MOUSE_BUTTON_1].SetPosition(hit);
	}
}

Engine* Engine::m_instance = nullptr;
