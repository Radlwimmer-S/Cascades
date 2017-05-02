#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <random>
#include "NoiseTexture.h"
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include "BoundingBox.h"
#include "BoundingBox.h"

class Shader;

struct Pillar
{
	glm::vec2 position;
	float frequence;
	float weight;
};

struct Bound
{
	float weight;
};

struct Helix
{
	float offset;
	float frequence;
	float weight;
};

struct Shelf
{
	float offset;
	float frequence;
	float weight;
};

namespace glm
{
	inline vec2 rotate(vec2 v, float a)
	{
		float s = sin(a);
		float c = cos(a);
		mat2 m = mat2(c, -s, s, c);
		return m * v;
	}
}

class ProcedualGenerator
{
	struct Noise
	{
		Noise(const glm::quat& rot, const NoiseTexture& texture) : rotation(glm::toMat4(rot)), texture(texture) {}
		glm::mat4 rotation;
		NoiseTexture texture;
	};

public:
	ProcedualGenerator(int seed);
	~ProcedualGenerator();
	void Generate3dTexture(int startLayer);
	void Generate3dTexture();

	void GenerateVBO(glm::vec3 cubesPerDimension);

	void SetUniforms(Shader& shader);

	GLuint GetTextureId() const;
	GLuint GetVboId() const;
	GLuint GetVaoId() const;
	GLuint GetVertexCount() const;
	float GetValue(int layer, int y, int x) const;

	static const int WIDTH = 96, DEPTH = 96, LAYERS = 256;

protected:
	void UpdateValues(int startLayer);
	static float AddPillar(glm::vec2 pos, const Pillar& pillar, glm::vec2 rotatetPos);
	static float AddBounds(glm::vec2 pos, const Bound& bound);
	static float AddHelix(glm::vec2 pos, const Helix& helix, float sinLayer, float cosLayer);
	static float AddShelves(glm::vec2 pos, const Shelf& shelf, float cosLayer);
	void ApplyDataToTexture();

	static float NormalizeCoord(int coord, int dim);

	static const int LANE = WIDTH, LAYER = WIDTH * DEPTH;
	GLfloat m_values[LAYERS * DEPTH * WIDTH];

	Pillar m_pillars[4];
	Bound m_bound;
	Helix m_helix;
	Shelf m_shelf;

	GLuint m_densityId = 0;
	GLuint m_vao = 0, m_vbo = 0;
	GLuint m_vertexCount = 0;
	glm::vec3 m_resolution;
	Noise* m_noise;

	std::default_random_engine m_random;
	std::uniform_int_distribution<int> m_randomAngle;
};

