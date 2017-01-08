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
	float u, v, w, distance;

	glm::vec3 ab = vertices_[1] - vertices_[0];
	glm::vec3 ac = vertices_[2] - vertices_[0];
	glm::vec3 qp = -ray.GetDirection();

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	glm::vec3 n = glm::cross(ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = glm::dot(qp, n);
	if (d <= 0.0f)
		return false;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	glm::vec3 ap = ray.GetOrigin() - vertices_[0];
	distance = glm::dot(ap, n);
	if (distance < 0.0f)
		return false;
	/*if (distance > d)
		return false;*/

	// For segment; exclude this code line for a ray test
	// Compute barycentric coordinate components and test if within bounds
	glm::vec3 e = glm::cross(qp, ap);
	v = glm::dot(ac, e);
	if (v < 0.0f || v > d)
		return false;
	w = -glm::dot(ab, e);
	if (w < 0.0f || v + w > d)
		return false;

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	float ood = 1.0f / d;
	distance *= ood;
	v *= ood;
	w *= ood;
	u = 1.0f - v - w;

	result.Distance = distance;

	return true;
}
