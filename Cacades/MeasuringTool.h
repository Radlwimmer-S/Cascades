#pragma once
#include "Model.h"
#include "Character.h"

class MeasuringTool
{
public:
	MeasuringTool();
	~MeasuringTool();

	void SetHit(int index, glm::vec3 position);
	glm::vec3 GetHit(int index) const;
	float GetDistance() const; 

	void Render(Shader& shader) const;
	void Update(GLfloat deltaTime);
	Model m_hits[2];
	GLuint vbo_;
	GLuint vao_;
};

