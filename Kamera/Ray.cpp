#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin_(origin), dir_(direction)
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
