#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <stdexcept>
#include "Light.h"
#include <vector>

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

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void Init(char* windowTitle);
	static void Init(char* windowTitle, GLuint width, GLuint height);
	void SetShader(Shader& shader);
	void SetShadowShader(Shader& shader);
	void SetScene(Scene& scene);
	void SetCamera(Camera& camera);
	void AddLight(Light& light);
	void Start();
	void Resume();
	void Pause();
	void Stop();
protected:
	void Loop();
	void ConfigureShader() const;
	void ConfigureMatrices() const;
	void RenderLights() const;
	void RenderScene() const;

	Shader* m_shader;
	Shader* m_shadowShader;
	Scene* m_scene;
	GLFWwindow& m_window;
	Camera* m_camera;
	std::vector<Light*> m_lights;

	static GLFWwindow* InitWindow(const char* windowTitle, bool fullscreen);





	static Engine* m_instance;
	explicit Engine(char* windowTitle, bool fullscreen);
	/* verhindert, dass ein Objekt von au�erhalb von Engine erzeugt wird. */
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
