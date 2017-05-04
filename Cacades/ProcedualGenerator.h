#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <random>
#include "NoiseTexture.h"
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include "BoundingBox.h"

class Shader;

struct Randoms
{
	float offset;
	int frequenceSign;
	float frequence;
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

	void SetUniformsMC(Shader& shader, int startLayer);
	void SetUniformsD(Shader& shader, int startLayer);

	GLuint GetTextureId() const;
	GLuint GetVboId() const;
	GLuint GetVaoId() const;
	GLuint GetVertexCount() const;

	void SetRandomSeed(int seed);

	static const int WIDTH = 96, DEPTH = 96, LAYERS = 256;

protected:
	void SetupMC();
	void SetupDensity();

	static float NormalizeCoord(int coord, int dim);
	static int ToSignBit(int random);

	static const int LANE = WIDTH, LAYER = WIDTH * DEPTH;
	GLfloat m_values[LAYERS * DEPTH * WIDTH];

	Randoms m_pillars[4];
	Randoms m_helix;
	Randoms m_shelf;

	GLuint m_densityId = 0;
	GLuint m_vaoMC = 0, m_vboMC = 0, m_vaoD = 0, m_vboD = 0, m_fboD = 0;
	GLuint m_vertexCount = 0;
	glm::vec3 m_mcResolution;
	Noise* m_noise;

	Shader* m_marchingCubeShader, *m_densityShader, *m_debugShader;

	std::default_random_engine m_random;
	std::uniform_int_distribution<int> m_randomAngle;
	std::uniform_real_distribution<float> m_randomRand;
	std::uniform_real_distribution<float> m_randomFloat;
};

