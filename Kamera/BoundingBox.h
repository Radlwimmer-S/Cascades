#pragma once
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>

class Shader;
class Ray;
class KdPrimitive;
struct HitResult;

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(KdPrimitive& tri);
	BoundingBox(std::vector<KdPrimitive*>& tris);
	~BoundingBox();

	void Expand(BoundingBox bbox);

	int GetLongestAxis() const;

	glm::vec3 GetMax() const;
	glm::vec3 GetMin() const;
	glm::vec3 GetCenter() const;
	glm::vec3 GetSize() const;

	bool IsHit(Ray& ray) const;

	void Render(Shader& shader, int depth) const;

protected:
	void GenerateVao();

	glm::vec3 min_;
	glm::vec3 max_;

	GLuint m_vao;
};

