#include "ProcedualGenerator.h"
#include <GL/glew.h>
#include <glm/detail/func_geometric.hpp>
#include <glm/gtx/optimum_pow.hpp>
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>

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

	m_shelveFrequence = 5 + m_randomAngle(m_random) / 4.0f;
	m_helixFrequence = 5 + m_randomAngle(m_random) / 4.0f;
	m_shelveOffset = m_randomAngle(m_random);
	m_helixOffset = m_randomAngle(m_random);

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
		float helixAngle = m_helixOffset + m_helixFrequence * glm::pi<float>() * ws_l;
		float sinHelix = sin(helixAngle);
		float cosHelix = cos(helixAngle);

		float shelvesAngle = m_shelveOffset + m_shelveFrequence * glm::pi<float>() * ws_l;
		float cosShelves = cos(shelvesAngle);

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
				f += 0.25f * AddPillar(ws, m_pillars[0]);
				f += 0.25f * AddPillar(ws, m_pillars[1]);
				f += 0.25f * AddPillar(ws, m_pillars[2]);
				f -= AddPillar(ws, glm::vec2());
				f -= 10.0f * AddBounds(ws);
				f += 3.0f * AddHelix(ws, sinHelix, cosHelix);
				f += 0.5f * AddShelves(cosShelves);

				m_values[layerIndex + laneIndex + x] = f;
			}
		}
	}
}

float ProcedualGenerator::AddPillar(glm::vec2 pos, glm::vec2 pillar)
{
	return 1.0f / glm::length(pos - pillar) - 1.0f;
}

float ProcedualGenerator::AddBounds(glm::vec2 pos)
{
	return glm::pow3(glm::length(pos));
}

float ProcedualGenerator::AddHelix(glm::vec2 pos, float sinLayer, float cosLayer)
{
	return glm::dot(glm::vec2(cosLayer, sinLayer), pos);
}

float ProcedualGenerator::AddShelves(float cosLayer)
{
	return cosLayer;
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
