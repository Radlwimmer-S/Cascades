#pragma once
#include <GL/glew.h>
#include "Enums.h"
#include "BaseObject.h"
#include <vector>
#include "KdNode.h"
#include "Model.h"

class Shader;

class Scene
{
public:
	Scene(std::vector<Model*>& objects);
	virtual ~Scene();

	virtual void Init();

	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;
	virtual void RenderKdTree(Shader& shader) const;
	virtual void ProcessInput(GLfloat deltaTime);

	State GetState() const;
	void SetState(State state);

	KdNode* GetKdTree() const;

	int TreeRenderDepth = 0;

protected:
	State m_state;
	std::vector<Model*> m_objects;
	std::vector<KdPrimitive*> m_kdObjects;
	KdNode* m_kdTree;
};

