
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
	path->push_back(ControlPoint(glm::vec3(1.90088, 2.71778, 8.36604), MakeQuad(43.5854, 49.3878, 35.8498)));
	path->push_back(ControlPoint(glm::vec3(5.5548, 2.16439, 6.6418), MakeQuad(0.597455, 4.29696, 0.0447613)));
	path->push_back(ControlPoint(glm::vec3(6.07589, 2.09267, -1.36941), MakeQuad(13.1986, -74.6709, -12.7447)));
	path->push_back(ControlPoint(glm::vec3(1.68801, 2.0471, -4.55512), MakeQuad(176.958, -48.1796, -177.732)));
	path->push_back(ControlPoint(glm::vec3(-1.5213, 1.71298, -0.850699), MakeQuad(11.594, -56.7565, -9.73634)));
	path->push_back(ControlPoint(glm::vec3(-4.77908, 1.43391, -4.22467), MakeQuad(96.988, -81.0292, -97.0736)));
	path->push_back(ControlPoint(glm::vec3(-9.22049, 1.24461, -4.98984), MakeQuad(174.254, 31.579, 176.984)));
	path->push_back(ControlPoint(glm::vec3(-9.26498, 1.11791, 0.951546), MakeQuad(-7.14398, 78.5915, -7.00425)));
	path->push_back(ControlPoint(glm::vec3(-3.84344, 1.22615, 6.16254), MakeQuad(-7.92887, 79.7276, -7.80336)));
	path->push_back(ControlPoint(glm::vec3(1.90088, 2.71778, 8.36604), MakeQuad(43.5854, 49.3878, 35.8498)));
	return path;
}

Scene* GetScene()
{
	Scene* scene = new Scene();
	Texture* floorTex = new Texture("textures/brickwall.jpg", "wall");
	Texture* floorNormal = new Texture("textures/brickwall_normal.jpg", "wall_normal");
	scene->AddObject(new TexturedModel(glm::vec3(0, -0.1f, 0), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(20, 0.2f, 20), glm::vec2(10, 10)), 12, glm::vec3(0.3f, 0.3f, 0.3f), *floorTex, floorNormal));
	Texture* containerTex = new Texture("textures/container.jpg", "box");
	Texture* containerNormal = new Texture("textures/container_normal.jpg", "box_normal");
	scene->AddObject(new TexturedModel(glm::vec3(7, 0.5f, 6), MakeQuad(0, 0, 0), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex, containerNormal));
	scene->AddObject(new TexturedModel(glm::vec3(-4, 3, -3), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex, containerNormal));
	scene->AddObject(new TexturedModel(glm::vec3(-7, 1, -3), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex, containerNormal));
	scene->AddObject(new TexturedModel(glm::vec3(0, 2, 0), MakeQuad(0, 0, 0), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex, containerNormal));
	scene->AddObject(new TexturedModel(glm::vec3(-2, 1, 4), MakeQuad(0, 50, 15), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex, containerNormal));
	Texture* containerSmallTex = new Texture("textures/container_small.jpg", "box");
	Texture* containerSmallNormal = new Texture("textures/container_small_normal.jpg", "box_normal");
	scene->AddObject(new TexturedModel(glm::vec3(6.25f, 0.25f, 5.25f), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
	scene->AddObject(new TexturedModel(glm::vec3(6.25f, 0.75f, 5.25f), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
	scene->AddObject(new TexturedModel(glm::vec3(5.75f, 0.25f, 5.25f), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
	scene->AddObject(new TexturedModel(glm::vec3(3, 2, -2), MakeQuad(0, 20, 70), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
	scene->AddObject(new TexturedModel(glm::vec3(-5, 1, -2), MakeQuad(0, 20, 40), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
	scene->AddObject(new TexturedModel(glm::vec3(-3, 1, -3), MakeQuad(0, 45, 20), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
	scene->AddObject(new TexturedModel(glm::vec3(0, 2, 0), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), *containerSmallTex, containerSmallNormal));
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

	CameraPath* path = new CameraPath(*GetPath(), 20);
	Camera* camera = new Camera(path);
	//Camera* camera = new Camera();
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