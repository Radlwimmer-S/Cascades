#include "Shader.h"
#include "Engine.h"
#include "CameraScene.h"
#include <filesystem>
#include "Camera.h"
#include "Global.h"
#include <glm/gtc/matrix_transform.inl>

std::vector<ControlPoint>* GetPath()
{
	std::vector<ControlPoint>* path = new std::vector<ControlPoint>();
	//path->push_back(ControlPoint(glm::vec3(+5.0f, 0, +10.1f),	MakeQuad(0, +00, -30))); // 0
	path->push_back(ControlPoint(glm::vec3(+5.0f, 0, +10.0f), MakeQuad(0, +00, -30))); // 1
	path->push_back(ControlPoint(glm::vec3(+3.5f, 0, +07.5f), MakeQuad(0, -30, -15))); // 2
	path->push_back(ControlPoint(glm::vec3(+0.0f, 0, +05.0f), MakeQuad(0, -45, +00))); // 3
	path->push_back(ControlPoint(glm::vec3(-3.5f, 0, +02.5f), MakeQuad(0, -30, +15))); // 4
	path->push_back(ControlPoint(glm::vec3(-5.0f, 0, +00.0f), MakeQuad(0, +00, +30))); // 5
	path->push_back(ControlPoint(glm::vec3(-3.5f, 0, -02.5f), MakeQuad(0, +30, +15))); // 6
	path->push_back(ControlPoint(glm::vec3(+0.0f, 0, -05.0f), MakeQuad(0, +45, +00))); // 7
	path->push_back(ControlPoint(glm::vec3(+3.5f, 0, -07.5f), MakeQuad(0, +30, -15))); // 8
	path->push_back(ControlPoint(glm::vec3(+5.0f, 0, -10.0f), MakeQuad(0, +00, -30))); // 9
	//path->push_back(ControlPoint(glm::vec3(+5.0f, 0, -10.1f),	MakeQuad(0, +00, -30))); //10
	return path;
}

// Window dimensions

// The MAIN function, from here we start the application and run the game loop
int main()
{
#ifdef _DEBUG
	Engine::Init("Camera", 1900, 1000);
#else
	Engine::Init("Camera");
#endif


	Engine* engine = Engine::Instance();

	// Build and compile our shader program
	Shader* shader = new Shader("./shaders/default.vert.shader", "./shaders/default.frag.shader");
	engine->SetShader(*shader);

	Scene* scene = new CameraScene();
	engine->SetScene(*scene);

	CameraPath* path = new CameraPath(*GetPath(), 15);
	Camera* camera = new Camera(path);
	engine->SetCamera(*camera);

	engine->Start();

	return 0;
}