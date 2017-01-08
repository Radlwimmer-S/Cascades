
#pragma once
#include "Shader.h"
#include "Engine.h"
#include "Camera.h"
#include "Global.h"
#include "Model.h"
#include "Texture.h"
#include "Box.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Hud.h"
#include <iomanip>
#include "Plane.h"

std::vector<ControlPoint>* GetPath()
{
	std::vector<ControlPoint>* path = new std::vector<ControlPoint>();
	path->push_back(ControlPoint(glm::vec3(+0.0, +2.0, +5.0), glm::radians(glm::vec3(0, 0, 0))));
	path->push_back(ControlPoint(glm::vec3(-3.5, +2.0, +3.5), glm::radians(glm::vec3(0, 45, 0))));
	path->push_back(ControlPoint(glm::vec3(-5.0, +2.0, +0.0), glm::radians(glm::vec3(0, 90, 0))));
	path->push_back(ControlPoint(glm::vec3(-3.5, +2.0, -3.5), glm::radians(glm::vec3(0, 135, 0))));
	path->push_back(ControlPoint(glm::vec3(+0.0, +2.0, -5.0), glm::radians(glm::vec3(0, 180, 0))));
	path->push_back(ControlPoint(glm::vec3(+3.5, +2.0, -3.5), glm::radians(glm::vec3(0, -135, 0))));
	path->push_back(ControlPoint(glm::vec3(+5.0, +2.0, +0.0), glm::radians(glm::vec3(0, -90, 0))));
	path->push_back(ControlPoint(glm::vec3(+3.5, +2.0, +3.5), glm::radians(glm::vec3(0, -45, 0))));
	path->push_back(ControlPoint(glm::vec3(+0.0, +2.0, +5.0), glm::radians(glm::vec3(0, 0, 0))));
	return path;
}

Scene* GetScene()
{
	std::vector<Model*>* models = new std::vector<Model*>();
	Texture* floorTex = new Texture("textures/brickwall.jpg", "wall");
	Texture* floorNormal = new Texture("textures/brickwall_normal.jpg", "wall_normal");
	for (int x = 0; x < 10; ++x)
		for (int z = 0; z < 10; ++z)
		{
			models->push_back(new Model(glm::vec3(-9 + 2 * x, 0, -9 + 2 * z), MakeQuad(0, 0, 0), Plane::GetTris(glm::vec2(2, 2), glm::vec2(1, 1)), 2, glm::vec3(0.3f, 0.3f, 0.3f), TextureOnly, floorTex, BumpMapOnly, floorNormal));
		}

	Texture* containerTex = new Texture("textures/container.jpg", "box");
	Texture* containerNormal = new Texture("textures/container_normal.jpg", "box_normal");
	models->push_back(new Model(glm::vec3(7, 0.5f, 6), MakeQuad(0, 0, 0), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerTex, BumpMapOnly, containerNormal));
	models->push_back(new Model(glm::vec3(-4, 3, -3), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerTex, BumpMapOnly, containerNormal));
	models->push_back(new Model(glm::vec3(-7, 1, -3), MakeQuad(0, 20, 40), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerTex, BumpMapOnly, containerNormal));
	models->push_back(new Model(glm::vec3(0, 2, 0), MakeQuad(0, 0, 0), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerTex, BumpMapOnly, containerNormal));
	models->push_back(new Model(glm::vec3(-2, 1, 4), MakeQuad(0, 50, 15), Box::GetTris(), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerTex, BumpMapOnly, containerNormal));

	Texture* containerSmallTex = new Texture("textures/container_small.jpg", "box");
	Texture* containerSmallNormal = new Texture("textures/container_small_normal.jpg", "box_normal");
	models->push_back(new Model(glm::vec3(6.25f, 0.25f, 5.25f), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	models->push_back(new Model(glm::vec3(6.25f, 0.75f, 5.25f), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	models->push_back(new Model(glm::vec3(5.75f, 0.25f, 5.25f), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	models->push_back(new Model(glm::vec3(3, 2, -2), MakeQuad(0, 20, 70), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	models->push_back(new Model(glm::vec3(-5, 1, -2), MakeQuad(0, 20, 40), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	models->push_back(new Model(glm::vec3(-3, 1, -3), MakeQuad(0, 45, 20), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	models->push_back(new Model(glm::vec3(0, 2.75f, 0), MakeQuad(0, 0, 0), Box::GetTris(glm::vec3(0.5f)), 12, glm::vec3(0.7f, 0.7f, 0.7f), TextureOnly, containerSmallTex, BumpMapOnly, containerSmallNormal));
	return new Scene(*models);
}

void TestCSAA()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(10, 10, "Test", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	PrintCSAA();

	glfwSetWindowShouldClose(window, GL_TRUE);
	glfwTerminate();

	std::cin.ignore();
}

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--test") == 0)
		{
			TestCSAA();
			return 0;
		}
	}

	try {
#ifdef _DEBUG
		Engine::Init("Camera", 1900, 1000);
#else
		Engine::Init("Camera");
#endif

		Engine* engine = Engine::Instance();

		Shader* shader = new Shader("./shaders/default.vert", "./shaders/default.frag");
		if (!shader->IsValid())
		{
			std::cin.ignore();
			return -1;
		}
		engine->SetShader(*shader);

		Shader* pointLightShader = new Shader("./shaders/PointLight.vert", "./shaders/PointLight.frag", "./shaders/PointLight.geom");
		if (!pointLightShader->IsValid())
		{
			std::cin.ignore();
			return -1;
		}

		Shader* directionalLightShader = new Shader("./shaders/DirectionalLight.vert", "./shaders/DirectionalLight.frag");
		if (!directionalLightShader->IsValid())
		{
			std::cin.ignore();
			return -1;
		}

		Shader* hudShader = new Shader("./shaders/Text.vert", "./shaders/Text.frag");
		if (!hudShader->IsValid())
		{
			std::cin.ignore();
			return -1;
		}

		engine->SetScene(*GetScene());

		CameraPath* path = new CameraPath(*GetPath(), 20);
		Camera* camera = new Camera(path);
		//Camera* camera = new Camera();
		engine->SetCamera(*camera);

		SpotLight* flashLight = new SpotLight(glm::vec3(-10, 5, 10), MakeQuad(22.5f, 45, 0), glm::vec3(0.8f, 0.8f, 0.8f), *directionalLightShader, 60, 50, 1);
		engine->AddLight(*flashLight);
		DirectionalLight* mainLight = new DirectionalLight(glm::vec3(10, 5, -10), glm::vec3(0, 0, .5f), *directionalLightShader, 50, -10);
		engine->AddLight(*mainLight);

		PointLight* greenLight = new PointLight(glm::vec3(-5, 5, -3), glm::vec3(0, 0.5f, 0), *pointLightShader, 15);
		engine->AddLight(*greenLight);
		PointLight* redLight = new PointLight(glm::vec3(5, 4, 3), glm::vec3(0.5f, 0, 0), *pointLightShader, 25);
		engine->AddLight(*redLight);

		Font* font = new Font("fonts/arial.ttf", glm::ivec2(0, 24));
		Hud* hud = new Hud(*font, *hudShader);
		engine->SetHud(*hud);

		engine->Start();

		return 0;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		std::cin.ignore();
		return -1;
	}
}