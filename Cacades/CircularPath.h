#pragma once
#include "BasePath.h"

class Shader;

class CircularPath : public BasePath
{
public:
	CircularPath(std::vector<ControlPoint>& controlPoints, GLuint duration);
	CircularPath(std::vector<ControlPoint>& controlPoints, GLuint duration, bool evenOutSpeed);
	~CircularPath();
protected:
	glm::vec3 CatmullRomSpline(const std::vector<ControlPoint>& cp, float t) const override;
	glm::quat Squad(const std::vector<ControlPoint>& cp, float t) const override;

	void CalculateTimeSteps();
	void Circularise();
};

