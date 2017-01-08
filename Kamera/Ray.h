#pragma once
#include "include/glm/detail/type_vec3.hpp"
#include "BoundingBox.h"

struct HitResult;

class Ray
{
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	~Ray();

	glm::vec3 GetOrigin() const;
	glm::vec3 GetDirection() const;

protected:
	glm::vec3 origin_;
	glm::vec3 dir_;
};

