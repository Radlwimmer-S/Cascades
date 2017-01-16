#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin_(origin), dir_(direction), minT_(0), maxT_(INFINITY)
{
}

Ray::~Ray()
{
}

glm::vec3 Ray::GetOrigin() const
{
	return origin_;
}

glm::vec3 Ray::GetDirection() const
{
	return dir_;
}

float Ray::GetMinT() const
{
	return minT_;
}

float Ray::GetMaxT() const
{
	return maxT_;
}
