#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "ProcedualGenerator.h"
#include "RenderInfo.h"
#include "UpdateInfo.h"
#include "ParticleSystem.h"
#include "Camera.h"

class Light;
class Hud;
class Camera;
class Shader;

class Engine
{
public:
	
	static Engine* Instance()
	{
		static CGuard g;   // Speicherbereinigung
		if (!m_instance)
			throw std::logic_error("Call Engine::Init beforehand!");
		return m_instance;
	}
	static void Init(char* windowTitle);
	static void Init(char* windowTitle, GLuint width, GLuint height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ResizeCallback(GLFWwindow* window, int width, int height);
	void Update(GLfloat deltaTime);
	void RenderScene();
	void RenderHud();
	void Start();
	void AddLight(Light& light);
protected:
	void RenderLights() const;
	void UpdateUniforms(const Shader& shader) const;
	void MoveActiveObject();
	void m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void m_CursorPosCallback(GLFWwindow* window, double x, double y);
	void m_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void m_ResizeCallback(GLFWwindow* window, int width, int height);
	void Loop();

	static GLFWwindow* InitWindow(const char* windowTitle, bool fullscreen);

	Hud* m_hud;
	GLFWwindow& m_window;
	Shader* m_shader;
	Shader* m_debugShader;
	Camera m_camera;
	std::vector<Light*> m_lights;

	ProcedualGenerator m_generator;
	ParticleSystem m_particleSystem;
	int m_activeObject;

	TriplanarMesh* m_mesh;
		
	const GLuint MaxTexturesPerModel = 10;

	RenderInfo m_renderInfo;
	UpdateInfo m_updateInfo;

	static Engine* m_instance;
	explicit Engine(GLFWwindow& window);
	/* verhindert, dass ein Objekt von auﬂerhalb von Engine erzeugt wird. */
	Engine(const Engine&);
	/* verhindert, dass eine weitere Instanz via Kopie-Konstruktor erstellt werden kann */
	~Engine();
	class CGuard
	{
	public:
		~CGuard()
		{
			if (nullptr != m_instance)
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}
	};
};

