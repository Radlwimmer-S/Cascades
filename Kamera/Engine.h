#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <stdexcept>
#include "PointLight.h"
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
	void SetScene(Scene& scene);
	void SetCamera(Camera& camera);
	void AddLight(PointLight& light);
	void Start();
	void Resume();
	void Pause();
	void Stop();
protected:
	void PrintData(int frames) const;
	void Loop();
	void UpdateUniforms() const;
	void RenderLights() const;
	void RenderScene() const;

	Shader* m_shader;
	Scene* m_scene;
	GLFWwindow& m_window;
	Camera* m_camera;
	std::vector<PointLight*> m_lights;
	int m_activeObject;
	static GLFWwindow* InitWindow(const char* windowTitle, bool fullscreen);
	const GLuint MaxTextures = 2;




	static Engine* m_instance;
	explicit Engine(char* windowTitle, bool fullscreen);
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

