
#include "Shader.h"
#include "Engine.h"
#include "CameraScene.h"
#include <filesystem>
#include "Camera.h"
#include "Global.h"

std::vector<ControlPoint>* GetPath()
{
	std::vector<ControlPoint>* path = new std::vector<ControlPoint>();
	//path->push_back(ControlPoint(glm::vec3(0, 0, 5),		MakeQuad(0, 0, 0))); // 1
	//path->push_back(ControlPoint(glm::vec3(-2, 0, 4.5f),	MakeQuad(0, 22.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(-3.5f, 0, 3.5f), MakeQuad(0, 45, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(-4.5f, 0, 2),	MakeQuad(0, 67.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(-5, 0, 0),		MakeQuad(0, 90, 0))); // 2
	//path->push_back(ControlPoint(glm::vec3(-4.5f, 0, -2),	MakeQuad(0, 112.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(-3.5f, 0, -3.5f),MakeQuad(0, 135, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(-2, 0, -4.5f),	MakeQuad(0, 157.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(0, 0, -5),		MakeQuad(0, 180, 0))); // 3
	//path->push_back(ControlPoint(glm::vec3(2, 0, -4.5f),	MakeQuad(0, -157.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(3.5f, 0, -3.5f), MakeQuad(0, -135, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(4.5f, 0, -2),	MakeQuad(0, -112.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(5, 0, 0),		MakeQuad(0, -90, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(4.5f, 0, 2),		MakeQuad(0, -67.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(3.5f, 0, 3.5f),	MakeQuad(0, -45, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(2, 0, 4.5f),		MakeQuad(0, -22.5f, 0))); // 0
	//path->push_back(ControlPoint(glm::vec3(0, 0, 5),		MakeQuad(0, 0, 0))); // 1
	
	path->push_back(ControlPoint(glm::vec3(+2.50f, 0, +10.0f), MakeQuad(0.0f, +00.0f, -30.0f))); // 1
	path->push_back(ControlPoint(glm::vec3(+1.75f, 0, +07.5f), MakeQuad(0.0f, -30.0f, -15.0f))); // 2
	path->push_back(ControlPoint(glm::vec3(+0.00f, 0, +05.0f), MakeQuad(0.0f, -45.0f, +00.0f))); // 3
	path->push_back(ControlPoint(glm::vec3(-1.75f, 0, +02.5f), MakeQuad(0.0f, -30.0f, +15.0f))); // 4
	path->push_back(ControlPoint(glm::vec3(-2.50f, 0, +00.0f), MakeQuad(0.0f, +00.0f, +30.0f))); // 5
	path->push_back(ControlPoint(glm::vec3(-1.75f, 0, -02.5f), MakeQuad(0.0f, +30.0f, +15.0f))); // 6
	path->push_back(ControlPoint(glm::vec3(+0.00f, 0, -05.0f), MakeQuad(0.0f, +45.0f, +00.0f))); // 7
	path->push_back(ControlPoint(glm::vec3(+1.75f, 0, -07.5f), MakeQuad(0.0f, +30.0f, -15.0f))); // 8
	path->push_back(ControlPoint(glm::vec3(+2.50f, 0, -10.0f), MakeQuad(0.0f, +00.0f, -30.0f))); // 9
	return path;
}

int main()
{
#ifdef _DEBUG
	Engine::Init("Camera", 1900, 1000);
#else
	Engine::Init("Camera");
#endif

	Engine* engine = Engine::Instance();

	Shader* shader = new Shader("./shaders/default.vert.shader", "./shaders/default.frag.shader");
	if (!shader->IsValid())
	{
		std::cin.ignore();
		return -1;
	}
	engine->SetShader(*shader);


	Shader* shadowShader = new Shader("./shaders/Shadow.vert.shader", "./shaders/Shadow.frag.shader");
	if (!shader->IsValid())
	{
		std::cin.ignore();
		return -1;
	}
	engine->SetShadowShader(*shadowShader);
	
	Scene* scene = new CameraScene();
	engine->SetScene(*scene);

	//CameraPath* path = new CameraPath(*GetPath(), 10);
	//Camera* camera = new Camera(path);
	Camera* camera = new Camera();
	engine->SetCamera(*camera);

	Light* mainLight = new Light(glm::vec3(0,5,1),MakeQuad(60,10,0),glm::vec3(1,1,1));
	engine->AddLight(*mainLight);

	engine->Start();

	return 0;
}