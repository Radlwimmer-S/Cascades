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
#include "RenderInfo.h"
#include "AntiAliasingInfo.h"
#include "Font.h"

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
	void SetShader(Shader& shader);
	void SetScene(Scene& scene);
	void SetCamera(Camera& camera);
	void AddLight(Light& light);
	void SetHud(Hud& hud);
	void Start();
	void Resume();
	void Pause();
	void Stop();
protected:
	void m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void m_CursorPosCallback(GLFWwindow* window, double x, double y);
	void m_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MoveActiveObject();
	void Loop();
	void UpdateUniforms() const;
	void RenderLights() const;
	void RenderScene() const;
	void RenderHud() const;
	void PrintData(int frames) const;
	void SetAASettings();

	static GLFWwindow* InitWindow(const char* windowTitle, bool fullscreen);

	Shader* m_shader;
	Scene* m_scene;
	GLFWwindow& m_window;
	Camera* m_camera;
	Hud* m_hud;
	std::vector<Light*> m_lights;

	Model m_hits[2];

	int m_activeObject;
	GLuint m_framebuffer;

	RenderInfo m_renderInfo;
	AntiAliasingInfo m_aaInfo;	

	const GLuint MaxTexturesPerModel = 3;

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

