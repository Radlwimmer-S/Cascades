#include "KdPrimitive.h"
#include "Ray.h"
#include <glm/gtc/quaternion.hpp>
#include "HitResult.h"

KdPrimitive::KdPrimitive(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) : vertices_{ v1,v2,v3 }, bbox_(*this)
{}

KdPrimitive::~KdPrimitive()
{}

glm::vec3 KdPrimitive::GetVertex(int index) const
{
	return vertices_[index];
}

glm::vec3 KdPrimitive::GetCenter() const
{
	return (vertices_[0] + vertices_[1] + vertices_[2]) / 3.0f;
}

const BoundingBox& KdPrimitive::GetBoundingBox() const
{
	return bbox_;
}

bool KdPrimitive::IsHit(HitResult& result, Ray& ray) const
{
	glm::vec3 e1, e2, h, s, q;
	float a, f, u, v, t;
	e1 = vertices_[1] - vertices_[0];
	e2 = vertices_[2] - vertices_[0];

	h = glm::cross(ray.GetDirection(), e2);
	a = glm::dot(e1, h);

	if (a > -0.00001 && a < 0.00001)
		return(false);

	f = 1 / a;
	s = ray.GetOrigin() - vertices_[0];
	u = f * (glm::dot(s, h));

	if (u < 0.0 || u > 1.0)
		return(false);

	q= glm::cross(s, e1);
	v = f * glm::dot(ray.GetDirection(), q);

	if (v < 0.0 || u + v > 1.0)
		return(false);

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * glm::dot(e2, q);

	if (t > 0.00001) // ray intersection
	{
		result.Distance = t;
		return(true);
	}
	else // this means that there is a line intersection
		 // but not a ray intersection
		return (false);
}