#include "BoundingBox.h"
#include <algorithm>
#include "Ray.h"
#include "HitResult.h"
#include "Vertex.h"
#include "Global.h"
#include <glm/gtc/type_ptr.inl>
#include "Enums.h"
#include "Shader.h"

BoundingBox::BoundingBox()
{
	GenerateVao();
}

BoundingBox::BoundingBox(KdPrimitive& tri) : min_(tri.GetVertex(0)), max_(tri.GetVertex(0))
{
	for (int i = 1; i < 3; ++i)
	{
		glm::vec3 vertex = tri.GetVertex(i);
		for (int axis = 0; axis < 3; ++axis)
		{
			min_[axis] = std::min(min_[axis], vertex[axis]);
			max_[axis] = std::max(max_[axis], vertex[axis]);
		}
	}
	GenerateVao();
}

BoundingBox::BoundingBox(std::vector<KdPrimitive*>& tris) : min_(INFINITY), max_(-INFINITY)
{
	for (int axis = 0; axis < 3; ++axis)
	{
		std::nth_element(tris.begin(), tris.begin(), tris.end(), [&](const KdPrimitive* t1, const KdPrimitive* t2)
		{
			return t1->GetBoundingBox().GetMin()[axis] < t2->GetBoundingBox().GetMin()[axis];
		});
		min_[axis] = (*tris.begin())->GetBoundingBox().GetMin()[axis];
	}
	for (int axis = 0; axis < 3; ++axis)
	{
		std::nth_element(tris.begin(), tris.end() - 1, tris.end(), [&](const KdPrimitive* t1, const KdPrimitive* t2)
		{
			return t1->GetBoundingBox().GetMax()[axis] < t2->GetBoundingBox().GetMax()[axis];
		});
		max_[axis] = (*(tris.end() - 1))->GetBoundingBox().GetMax()[axis];
	}
	GenerateVao();
	return;
	for (auto it = tris.begin(); it != tris.end(); ++it)
	{
		Expand((*it)->GetBoundingBox());
	}
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Expand(BoundingBox bbox)
{
	for (int axis = 0; axis < 3; ++axis)
	{
		min_[axis] = std::min(min_[axis], bbox.min_[axis]);
		max_[axis] = std::max(max_[axis], bbox.max_[axis]);
	}
}

int BoundingBox::GetLongestAxis() const
{
	glm::vec3 size = max_ - min_;
	int longestAxis = 0;

	for (int axis = 1; axis < 3; ++axis)
	{
		if (size[axis] > size[longestAxis])
			longestAxis = axis;
	}
	return longestAxis;
}

glm::vec3 BoundingBox::GetMax() const
{
	return max_;
}

glm::vec3 BoundingBox::GetMin() const
{
	return min_;
}

glm::vec3 BoundingBox::GetCenter() const
{
	return min_ + GetSize();
}

glm::vec3 BoundingBox::GetSize() const
{
	return (max_ - min_) / 2.0f;
}

bool BoundingBox::IsHit(Ray& ray) const
{
	float tMin = (min_.x - ray.GetOrigin().x) / ray.GetDirection().x;
	float tMax = (max_.x - ray.GetOrigin().x) / ray.GetDirection().x;

	if (tMin > tMax)
		std::swap(tMin, tMax);

	float tyMin = (min_.y - ray.GetOrigin().y) / ray.GetDirection().y;
	float tyMax = (max_.y - ray.GetOrigin().y) / ray.GetDirection().y;

	if (tyMin > tyMax)
		std::swap(tyMin, tyMax);

	if ((tMin > tyMax) || (tyMin > tMax))
		return false;

	if (tyMin > tMin)
		tMin = tyMin;

	if (tyMax < tMax)
		tMax = tyMax;

	float tzMin = (min_.z - ray.GetOrigin().z) / ray.GetDirection().z;
	float tzMax = (max_.z - ray.GetOrigin().z) / ray.GetDirection().z;

	if (tzMin > tzMax)
		std::swap(tzMin, tzMax);

	if ((tMin > tzMax) || (tzMin > tMax))
		return false;

	if (tzMin > tMin)
		tMin = tzMin;

	if (tzMax < tMax)
		tMax = tzMax;

	return ((tMin < ray.GetMaxT()) && (tMax > ray.GetMinT()));
}

void BoundingBox::Render(Shader& shader, int depth) const
{
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glCheckError();

	GLint colorModeLoc = glGetUniformLocation(shader.Program, "colorMode");
	glUniform1i(colorModeLoc, ColorOnly);
	glCheckError();

	GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
	glUniform3fv(objectColorLoc, 1, glm::value_ptr(glm::vec3(depth / 5.f, 1 - depth / 5.f, depth / 10.f)));
	glCheckError();

	GLint normalModeLoc = glGetUniformLocation(shader.Program, "normalMode");
	glUniform1i(normalModeLoc, NoNormals);
	glCheckError();

	glBindVertexArray(m_vao);
	glCheckError();    
	glLineWidth(2);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glGetError();

	glBindVertexArray(0);
}

void BoundingBox::GenerateVao()
{
	Vertex vertices[8] = {
		Vertex(glm::vec3(min_.x, min_.y, min_.z)),
		Vertex(glm::vec3(max_.x, min_.y, min_.z)),
		Vertex(glm::vec3(max_.x, max_.y, min_.z)),
		Vertex(glm::vec3(min_.x, max_.y, min_.z)),
		Vertex(glm::vec3(min_.x, min_.y, max_.z)),
		Vertex(glm::vec3(max_.x, min_.y, max_.z)),
		Vertex(glm::vec3(max_.x, max_.y, max_.z)),
		Vertex(glm::vec3(min_.x, max_.y, max_.z))
	};

	GLushort elements[16] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5,
		2, 6, 3, 7
	};

	GLuint vbo;
	GLuint ibo_elements;
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo_elements);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16 * sizeof(GLushort), elements, GL_STATIC_DRAW);

	glVertexAttribPointer(VS_IN_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(VS_IN_POSITION);

	glBindVertexArray(0); // Unbind vao
}
