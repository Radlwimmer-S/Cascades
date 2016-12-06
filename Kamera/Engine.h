#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <stdexcept>
#include <vector>
#include "DirectionalLight.h"

class Camera;
class Shader;

enum AAMode
{
	Fastest = 0,
	Nicest = 1,
	NV_Fastest = 2,
	NV_Nicest = 3
};
const int AAModeCount = 4;

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
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void Init(char* windowTitle);
	static void Init(char* windowTitle, GLuint width, GLuint height);
	void SetShader(Shader& shader);
	void SetScene(Scene& scene);
	void SetCamera(Camera& camera);
	void AddLight(Light& light);
	void Start();
	void Resume();
	void Pause();
	void Stop();
protected:
	void PrintData(int frames) const;
	void SetAASettings() const;	
	void Loop();
	void UpdateUniforms() const;
	void RenderLights() const;
	void RenderScene() const;

	Shader* m_shader;
	Scene* m_scene;
	GLFWwindow& m_window;
	Camera* m_camera;
	std::vector<Light*> m_lights;
	int m_activeObject;
	float m_bumpiness = 1.0f;
	bool m_softShadows = true;
	static GLFWwindow* InitWindow(const char* windowTitle, bool fullscreen);
	const GLuint MaxTexturesPerModel = 3;

	AAMode m_aaMode = NV_Fastest;
	int m_samples = 4;
	GLuint m_framebuffer, m_multisampleTex, m_renderbuffer;

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

