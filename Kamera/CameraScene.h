#pragma once

#include "Scene.h"
#include "Box.h"

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
	Box* m_obj;
};

