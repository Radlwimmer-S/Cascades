#include "CameraScene.h"
#include "Texture.h"

CameraScene::CameraScene()
{
	m_objects.push_back(new Box(glm::vec3(0, 0, +00), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -10), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -20), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -30), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -40), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -50), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -60), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -70), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -80), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
	m_objects.push_back(new Box(glm::vec3(0, 0, -90), glm::vec3(2, 4, 2), glm::vec3(0.7f, 0.7f, 0.7f)));
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

	for (std::vector<Model*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

void CameraScene::Render(Shader& shader) const
{
	if (m_state != Running && m_state != Paused)
		return;

	for (std::vector<Model*>::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Render(shader);
	}
}

void CameraScene::ProcessInput(GLfloat deltaTime)
{
}
