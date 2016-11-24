
#include "Shader.h"
#include "Engine.h"
#include <filesystem>
#include "Camera.h"
#include "Global.h"
#include "TexturedModel.h"
#include "Texture.h"
#include "Box.h"
#include "SpotLight.h"


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

Scene* GetScene()
{
	Scene* scene = new Scene();
	Texture* floorTex = new Texture("textures/brickwall.jpg", "wall");
	Texture* floorNormal = new Texture("textures/brickwall_normal.jpg", "wall_normal");
	scene->AddObject(new TexturedModel(glm::vec3(0, -0.2f, 0), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(20, 0.2f, 20), glm::vec2(10, 10)), 12, glm::vec3(0.3f, 0.3f, 0.3f), *floorTex, floorNormal));
	Texture* containerTex = new Texture("textures/container.jpg", "box");
	scene->AddObject(new TexturedModel(glm::vec3(-4, 3, -3), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(-3, 1, -3), MakeQuad(0, 45, 20), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(-7, 1, -3), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(-5, 1, -2), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(0, 2, 0), MakeQuad(0, 0, 0), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(3, 2, -2), MakeQuad(0, 20, 70), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(-2, 1, 4), MakeQuad(0, 50, 15), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	scene->AddObject(new TexturedModel(glm::vec3(0, 2, 0), MakeQuad(0, 0, 0), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	return scene;
}

int main()
{
#ifdef _DEBUG
	Engine::Init("Camera", 1900, 1000);
#else
	Engine::Init("Camera");
#endif

	Engine* engine = Engine::Instance();

	Shader* shader = new Shader("./shaders/default.vert.shader", "./shaders/uniformLight.frag.shader");
	if (!shader->IsValid())
	{
		std::cin.ignore();
		return -1;
	}
	engine->SetShader(*shader);

	Shader* pointLightShader = new Shader("./shaders/PointLight.vert.shader", "./shaders/PointLight.frag.shader", "./shaders/PointLight.geom.shader");
	if (!pointLightShader->IsValid())
	{
		std::cin.ignore();
		return -1;
	}

	Shader* directionalLightShader = new Shader("./shaders/DirectionalLight.vert.shader", "./shaders/DirectionalLight.frag.shader");
	if (!directionalLightShader->IsValid())
	{
		std::cin.ignore();
		return -1;
	}

	engine->SetScene(*GetScene());

	//CameraPath* path = new CameraPath(*GetPath(), 10);
	//Camera* camera = new Camera(path);
	Camera* camera = new Camera();
	engine->SetCamera(*camera);

	DirectionalLight* mainLight = new DirectionalLight(glm::vec3(10, 5, -10), glm::vec3(0, 0, 0.5f), *directionalLightShader, 50, -10);
	engine->AddLight(*mainLight);
	SpotLight* flashLight = new SpotLight(glm::vec3(-10, 5, 10), glm::vec3(0.5f, 0.5f, 0.5f), *directionalLightShader, 60, 50, 1);
	engine->AddLight(*flashLight);

	PointLight* greenLight = new PointLight(glm::vec3(-5, 5, -3), glm::vec3(0, 0.5f, 0), *pointLightShader, 15);
	engine->AddLight(*greenLight);
	PointLight* redLight = new PointLight(glm::vec3(5, 4, 3), glm::vec3(0.5f, 0, 0), *pointLightShader, 25);
	engine->AddLight(*redLight);


	engine->Start();

	return 0;
}