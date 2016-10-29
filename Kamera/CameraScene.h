#pragma once

#include "Scene.h"
#include "Box.h"
#include <vector>
#include "Light.h"

class Object;

class CameraScene :
	public Scene
{
public:
	CameraScene();
	~CameraScene();

	void Init() override;
	
	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader) const override;
	void ProcessInput(GLfloat deltaTime) override;
	
private:
	std::vector<BaseObject*> m_objects;
};

