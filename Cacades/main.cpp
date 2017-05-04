
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

		engine->Start();

		return 0;
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::" << e.what() << " | " << __FILE__ << " (" << __LINE__ << ")" << std::endl;
		std::cin.ignore();
		return -1;
	}
}