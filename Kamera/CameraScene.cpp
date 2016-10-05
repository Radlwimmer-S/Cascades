#include "CameraScene.h"
#include "Texture.h"
#include "Object.h"

CameraScene::CameraScene()
{
	Texture* texture = new Texture("textures/container.jpg", "Container");
	m_obj = new Box(glm::vec3(0, 0, 0), 1, *texture);
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

	m_obj->Update(deltaTime);
}

void CameraScene::Render(Shader& shader) const
{
	if (m_state != Running && m_state != Paused)
		return;

	m_obj->Render(shader);
}

void CameraScene::ProcessInput(GLfloat deltaTime)
{
}
