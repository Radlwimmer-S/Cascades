#include "Scene.h"

Scene::Scene(std::vector<Model*>& objects) : m_state(Running), m_objects(objects), TreeRenderDepth(0)
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->GetKdPrimitives(m_kdObjects);
	}
	m_kdTree = new KdNode(m_kdObjects);
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

	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

void Scene::Render(Shader& shader) const
{
	if (m_state != Running && m_state != Paused)
		return;

	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		(*it)->Render(shader);
	}
}

void Scene::RenderKdTree(Shader& shader) const
{
	if (TreeRenderDepth > 0)
		m_kdTree->RenderLeafs(shader, TreeRenderDepth, 1);
}

void Scene::ProcessInput(GLfloat deltaTime)
{
}

State Scene::GetState() const
{
	return m_state;
}

void Scene::SetState(State state)
{
	m_state = state;
}

KdNode* Scene::GetKdTree() const
{
	return m_kdTree;
}
