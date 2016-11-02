#pragma once
#include <GL/glew.h>
#include "Enums.h"
#include "BaseObject.h"
#include <vector>

class Shader;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init();

	virtual void Update(GLfloat deltaTime);
	virtual void Render(Shader& shader) const;
	virtual void ProcessInput(GLfloat deltaTime);

	void AddObject(BaseObject* object);

	State GetState() const;
	void SetState(State state);

protected:
	State m_state;
	std::vector<BaseObject*> m_objects;
};

