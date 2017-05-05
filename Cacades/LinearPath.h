#pragma once
#include "BasePath.h"
class LinearPath : public BasePath
{
public:
	LinearPath(std::vector<ControlPoint>& controlPoints, GLuint duration);
	LinearPath(std::vector<ControlPoint>& controlPoints, GLuint duration, bool evenOutSpeed);
	~LinearPath();
	void Update(GLfloat deltaTime) override;
protected:
	glm::vec3 CatmullRomSpline(const std::vector<ControlPoint>& cp, float t) const override;
	glm::quat Squad(const std::vector<ControlPoint>& cp, float t) const override;

	void CalculateTimeSteps();

	bool m_forward;
};

