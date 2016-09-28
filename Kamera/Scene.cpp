#include "Scene.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

State Scene::GetState()
{
	return m_state;
}

void Scene::SetState(State state)
{
	m_state = state;
}
