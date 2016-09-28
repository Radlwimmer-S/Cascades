#pragma once
#include <GL/glew.h>
#include "Enums.h"

class Shader;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init() = 0;

	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render(Shader& shader) const = 0;
	virtual void ProcessInput(GLfloat deltaTime) = 0;

	State GetState();
	void SetState(State state);

protected:
	State m_state;
};

