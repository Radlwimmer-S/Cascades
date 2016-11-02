#include "Scene.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Init()
{
}

void Scene::Update(GLfloat deltaTime)
{
	if (m_state != Running)
		return;

	for (std::vector<BaseObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

void Scene::Render(Shader& shader) const
{
	if (m_state != Running && m_state != Paused)
		return;

	for (std::vector<BaseObject*>::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Render(shader);
	}
}

void Scene::ProcessInput(GLfloat deltaTime)
{
}

void Scene::AddObject(BaseObject* object)
{
	m_objects.push_back(object);
}

State Scene::GetState() const
{
	return m_state;
}

void Scene::SetState(State state)
{
	m_state = state;
}
