#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "BoundingBox.h"

class Ray;

class KdPrimitive
{
public:
	KdPrimitive(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	~KdPrimitive();

	glm::vec3 GetVertex(int index)const;
	glm::vec3 GetCenter() const;
	const BoundingBox& GetBoundingBox() const;

	bool IsHit(HitResult& result, Ray& ray) const;

protected:
	glm::vec3 vertices_[3];
	BoundingBox bbox_;
};

