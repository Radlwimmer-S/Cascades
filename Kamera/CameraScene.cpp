#include "CameraScene.h"
#include "Texture.h"


CameraScene::CameraScene()
{
	Texture* texture = new Texture("textures/container.jpg", "Container");
	m_box = new Box(glm::vec3(0,0,0), 1, *texture);
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

	m_box->Update(deltaTime);
}

void CameraScene::Render(Shader& shader) const
{
	if (m_state != Running && m_state != Paused)
		return;

	m_box->Render(shader);
}

void CameraScene::ProcessInput(GLfloat deltaTime)
{
}
