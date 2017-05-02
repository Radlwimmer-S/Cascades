#include "ProcedualGenerator.h"
#include <GL/glew.h>
#include <glm/detail/func_geometric.hpp>
#include <glm/gtx/optimum_pow.hpp>
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>
#include "BoundingBox.h"

ProcedualGenerator::ProcedualGenerator(int seed) : m_random(seed), m_randomAngle(0, 359)
{
	glGenTextures(1, &m_densityId);

	glBindTexture(GL_TEXTURE_3D, m_densityId);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_3D, 0);
	glCheckError();

	m_pillars[0] = Pillar{ glm::vec2(0.0f,  0.5f), -1.2f, 0.25f };
	m_pillars[1] = Pillar{ glm::vec2(-0.4f, -0.25f), 1, 0.25f };
	m_pillars[2] = Pillar{ glm::vec2(0.4f, -0.25f), 1, 0.25f };
	m_pillars[3] = Pillar{ glm::vec2(0.0f, 0.0f), 1, -1.0f };

	m_bound = Bound{ -10.0f };

	m_helix = Helix{ 1.0f * m_randomAngle(m_random), 5.0f + m_randomAngle(m_random) / 8.0f, 3.0f };

	m_shelf = Shelf{ 1.0f * m_randomAngle(m_random), 5.0f + m_randomAngle(m_random) / 8.0f, 0.5f };

	m_noise = new Noise[4]{
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 1)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 2)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 3)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 4))
	};
}


ProcedualGenerator::~ProcedualGenerator()
{
}

void ProcedualGenerator::Generate3dTexture(int startLayer)
{
	UpdateValues(startLayer);

	ApplyDataToTexture();
}

void ProcedualGenerator::Generate3dTexture()
{
	Generate3dTexture(0);
}

void ProcedualGenerator::GenerateVBO(glm::vec3 cubesPerDimension)
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glCheckError();

	m_resolution = 2.0f / cubesPerDimension;
	glm::vec3 vertexPerDimension = cubesPerDimension - glm::vec3(1, 0, 1);

	m_vertexCount = vertexPerDimension.x * vertexPerDimension.y * vertexPerDimension.z;
	glm::vec3* vertices = new glm::vec3[m_vertexCount];

	glm::vec3 pos = glm::vec3(-1) + m_resolution / 2.0f;
	int index = 0;
	for (int layer = 0; layer < vertexPerDimension.y; ++layer)
	{
		pos.x = -1.0f + m_resolution.x / 2.0f;
		for (int x = 0; x < vertexPerDimension.x; ++x)
		{
			pos.z = -1.0f + m_resolution.z / 2.0f;
			for (int z = 0; z < vertexPerDimension.z; ++z)
			{
				vertices[index] = pos;
				++index;
				pos.z += m_resolution.z;
			}
			pos.x += m_resolution.x;
		}
		pos.y += m_resolution.y;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
	glCheckError();
	// Position attribute
	glEnableVertexAttribArray(VS_IN_POSITION);
	glVertexAttribPointer(VS_IN_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckError();
}

GLuint ProcedualGenerator::GetTextureId() const
{
	return m_densityId;
}

GLuint ProcedualGenerator::GetVboId() const
{
	return m_vbo;
}

GLuint ProcedualGenerator::GetVaoId() const
{
	return m_vao;
}

GLuint ProcedualGenerator::GetVertexCount() const
{
	return m_vertexCount;
}

void ProcedualGenerator::SetUniforms(Shader& shader)
{
	glm::vec3 resolution = m_resolution;
	GLint resolutioneLoc = glGetUniformLocation(shader.Program, "resolution");
	glUniform3fv(resolutioneLoc, 1, glm::value_ptr(resolution));
	glCheckError();

	for (int i = 0; i < 4; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		GLuint textureId = m_noise[i].texture.GetId();
		GLint textureLoc = glGetUniformLocation(shader.Program, ("noise[" + std::to_string(i) + "].tex").c_str());
		glUniform1i(textureLoc, i);
		glBindTexture(GL_TEXTURE_3D, textureId);
		glCheckError();

		glm::mat4 rot = m_noise[i].rotation;
		GLuint rotLocation = glGetUniformLocation(shader.Program, ("noise[" + std::to_string(i) + "].rotation").c_str());
		glUniformMatrix4fv(rotLocation, 1, GL_FALSE, glm::value_ptr(rot));
		glCheckError();
	}

	glActiveTexture(GL_TEXTURE4);
	GLuint textureId = m_densityId;
	GLint textureLoc = glGetUniformLocation(shader.Program, "densityTex");
	glUniform1i(textureLoc, 4);
	glBindTexture(GL_TEXTURE_3D, textureId);
	glCheckError();

	GLuint isoLevel = 0;
	GLint isoLevelLoc = glGetUniformLocation(shader.Program, "isoLevel");
	glUniform1i(isoLevelLoc, isoLevel);
	glCheckError();

	glm::vec3 scale = glm::vec3(5, 10, 5);
	GLuint scaleLocation = glGetUniformLocation(shader.Program, "scale");
	glUniform3fv(scaleLocation, 1, glm::value_ptr(scale));
	glCheckError();
}

float ProcedualGenerator::GetValue(int layer, int y, int x) const
{
	return m_values[layer * LAYER + y * LANE + x];
}

void ProcedualGenerator::UpdateValues(int startLayer)
{
#pragma omp parallel for
	for (int layer = startLayer; layer < LAYERS + startLayer; ++layer)
	{
		float ws_l = NormalizeCoord(layer, LAYERS);
		float helixAngle = m_helix.offset + m_helix.frequence * glm::pi<float>() * ws_l;
		float sinHelix = sin(helixAngle);
		float cosHelix = cos(helixAngle);

		float shelvesAngle = m_shelf.offset + m_shelf.frequence * glm::pi<float>() * ws_l;
		float cosShelves = cos(shelvesAngle);

		glm::vec2 pillar0Position = glm::rotate(m_pillars[0].position, m_pillars[0].frequence * glm::pi<float>() * ws_l);
		glm::vec2 pillar1Position = glm::rotate(m_pillars[1].position, m_pillars[1].frequence * glm::pi<float>() * ws_l);
		glm::vec2 pillar2Position = glm::rotate(m_pillars[2].position, m_pillars[2].frequence * glm::pi<float>() * ws_l);
		glm::vec2 pillar3Position = glm::rotate(m_pillars[3].position, m_pillars[3].frequence * glm::pi<float>() * ws_l);

		int layerIndex = layer * LAYER;
		for (int y = 0; y < DEPTH; ++y)
		{
			float ws_y = NormalizeCoord(y, DEPTH);
			int laneIndex = y * LANE;

			for (int x = 0; x < WIDTH; ++x)
			{
				float ws_x = NormalizeCoord(x, WIDTH);
				glm::vec2 ws(ws_x, ws_y);

				float f = 0;
				f += AddPillar(ws, m_pillars[0], pillar0Position);
				f += AddPillar(ws, m_pillars[1], pillar1Position);
				f += AddPillar(ws, m_pillars[2], pillar2Position);
				f += AddPillar(ws, m_pillars[3], pillar3Position);
				f += AddBounds(ws, m_bound);
				f += AddHelix(ws, m_helix, sinHelix, cosHelix);
				f += AddShelves(ws, m_shelf, cosShelves);

				m_values[layerIndex + laneIndex + x] = f;
			}
		}
	}
}

float ProcedualGenerator::AddPillar(glm::vec2 pos, const Pillar& pillar, glm::vec2 rotatetPos)
{
	return pillar.weight * (1.0f / glm::length(pos - rotatetPos) - 1.0f);
}

float ProcedualGenerator::AddBounds(glm::vec2 pos, const Bound& bound)
{
	return bound.weight * glm::pow3(glm::length(pos));
}

float ProcedualGenerator::AddHelix(glm::vec2 pos, const Helix& helix, float sinLayer, float cosLayer)
{
	return helix.weight * glm::dot(glm::vec2(cosLayer, sinLayer), pos);
}

float ProcedualGenerator::AddShelves(glm::vec2 pos, const Shelf& shelf, float cosLayer)
{
	return shelf.weight * cosLayer;
}

void ProcedualGenerator::ApplyDataToTexture()
{
	glBindTexture(GL_TEXTURE_3D, m_densityId);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, WIDTH, DEPTH, LAYERS, 0, GL_RED, GL_FLOAT, m_values);
	glBindTexture(GL_TEXTURE_3D, 0);
	glCheckError();
}

float ProcedualGenerator::NormalizeCoord(int coord, int dim)
{
	return 2.0f * (coord * 1.0f / dim - 0.5f);
}
