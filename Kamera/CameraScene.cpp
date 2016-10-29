#include "CameraScene.h"
#include "Texture.h"
#include "TexturedModel.h"

CameraScene::CameraScene()
{
	Texture* containerTex = new Texture("textures/container.jpg", "box");
	m_objects.push_back(new Model(glm::vec3(0, -0.1f, 0), Box::GetVN(glm::vec3(10, 0.2f, 10)), 36, glm::vec3(0.3f, 0.3f, 0.3f)));
	m_objects.push_back(new TexturedModel(glm::vec3(0, 0.5f, 0), Box::GetVNT(glm::vec3(1, 1, 1)), 36, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	m_objects.push_back(new TexturedModel(glm::vec3(2, 0.5f, 0), Box::GetVNT(glm::vec3(1, 1, 1)), 36, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	m_objects.push_back(new TexturedModel(glm::vec3(-1, 0.5f, 1), Box::GetVNT(glm::vec3(1, 1, 1)), 36, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
	m_objects.push_back(new TexturedModel(glm::vec3(5, 0.5f, -3), Box::GetVNT(glm::vec3(1, 1, 1)), 36, glm::vec3(0.7f, 0.7f, 0.7f), *containerTex));
}


CameraScene::~CameraScene()
{
}

void CameraScene::Init()
{
}

void CameraScene::Update(GLfloat deltaTime)
{
	if (m_state != Running)
		return;

	for (std::vector<BaseObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

void CameraScene::Render(Shader& shader) const
{
	if (m_state != Running && m_state != Paused)
		return;

	for (std::vector<BaseObject*>::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Render(shader);
	}
}

void CameraScene::ProcessInput(GLfloat deltaTime)
{
}
