
#pragma once
#include "Shader.h"
#include "Engine.h"
#include "Camera.h"
#include "Global.h"
#include "Font.h"
#include "Hud.h"

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

void TestShader(Shader& shader, char* description)
{
	while (!shader.IsValid())
	{
		std::cout << "ERROR::SHADER " << description << " invalid! Press Enter to reload: ";
		std::cin.ignore();
		shader.Use();
	}
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
		Engine::Init("Camera", 1900, 1000);
#endif

		Engine* engine = Engine::Instance();

		Shader* marchingCubeShader = new Shader("./shaders/MarchingCubes.vert", "./shaders/MarchingCubes.geom", "./shaders/MarchingCubes.frag");
		TestShader(*marchingCubeShader, "MarchingCubes");

		Shader* hudShader = new Shader("./shaders/Text.vert", nullptr, "./shaders/Text.frag");
		TestShader(*hudShader, "Text/Hud");

		Font* font = new Font("fonts/arial.ttf", glm::ivec2(0, 24));
		Hud* hud = new Hud(*font, *hudShader);

		Camera* camera = new Camera();

		engine->Start(camera, marchingCubeShader, hud);

		return 0;
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::" << e.what() << std::endl;
		std::cin.ignore();
		return -1;
	}
}


//-Ein RenderPass um eine 3D Texture mit density Werten zu fuellen(in OGL eing GL_TEXTURE_3D generieren und mit den passenden Dimensionen erstellen)
//- Jetzt kann man mit "Layered Rendering" in diese Texture malen(dazu mit glDrawArrayInstanced(TYPE, START, VERTEX_END, INSTANCE_COUNT) pro layer in der 3D Texture einmal ein Full - Screen Qaud zeichnen)
//- Wenn man mit glDrawArraysInstanced zeichnet bekommt der VS den Wert "gl_InstanceID" gesetzt(bei mir von 0 - 255) und im GS kann man gl_Layer setzten
//- Ist gl_Layer vorm FS gesetzt, dann werden alle pixel ausm FS in diese Layer gezeichnet