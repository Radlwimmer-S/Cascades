#include "ProcedualGenerator.h"
#include <GL/glew.h>
#include <glm/detail/func_geometric.hpp>
#include <glm/gtx/optimum_pow.hpp>
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>
#include "BoundingBox.h"
#include <GLFW/glfw3.h>

ProcedualGenerator::ProcedualGenerator(int seed) : m_random(seed), m_randomAngle(0, 359), m_randomRand(-glm::pi<float>(), glm::pi<float>())
{
	SetupDensity();
	SetupMC();
}

void ProcedualGenerator::SetupMC()
{
	//const GLchar* feedbackVaryings[] = { "gs_out.position", "gs_out.normal"};
	m_marchingCubeShader = new Shader("./shaders/MarchingCubes.vert", "./shaders/MarchingCubes.geom", "./shaders/MarchingCubes.frag");
	m_marchingCubeShader->Test("MarchingCubes");

	m_noise = new Noise[4]{
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 1)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 2)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 3)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 4))
	};
}

void ProcedualGenerator::SetupDensity()
{
	m_densityShader = new  Shader("./shaders/Density.vert", "./shaders/Density.geom", "./shaders/Density.frag");
	m_densityShader->Test("Density");

	m_pillars[0] = Pillar{ glm::vec2(0.0f,  0.5f), -2.2f, 0.25f };
	m_pillars[1] = Pillar{ glm::vec2(-0.4f, -0.25f), 1.8f, 0.25f };
	m_pillars[2] = Pillar{ glm::vec2(0.4f, -0.25f), 1.2f, 0.25f };
	m_pillars[3] = Pillar{ glm::vec2(0.0f, 0.0f), 1, -1.0f };

	m_bound = Bound{ -10.0f };

	m_helix = Helix{ m_randomRand(m_random), 2.0f + fmodf(m_random(), 509.0f), 3.0f };

	m_shelf = Shelf{ m_randomRand(m_random), 2.0f + fmodf(m_random(), 50.0f), 0.5f };

	glGenTextures(1, &m_densityId);
	glBindTexture(GL_TEXTURE_3D, m_densityId);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, WIDTH, DEPTH, LAYERS, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glCheckError();

	glGenFramebuffers(1, &m_fboD);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboD);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_densityId, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();

	glBindTexture(GL_TEXTURE_3D, 0);
	glCheckError();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR::DENSITY::FRAMEBUFFER_NOT_COMPLETE");
		std::cin.ignore();
	}

	GLfloat vertices[6][2] = {
		{ -1,  1 },
		{ -1, -1 },
		{  1, -1 },
		
		{ -1,  1 },
		{  1, -1 },
		{  1,  1 }
	};

	glGenVertexArrays(1, &m_vaoD);
	glGenBuffers(1, &m_vboD);
	glBindVertexArray(m_vaoD);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboD);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VS_IN_POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(VS_IN_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


ProcedualGenerator::~ProcedualGenerator()
{
}

void ProcedualGenerator::Generate3dTexture(int startLayer)
{
#if false
	UpdateValues(startLayer);

	ApplyDataToTexture();
#else
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboD);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	glViewport(0, 0, WIDTH, DEPTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glCheckError();

	m_densityShader->Use();
	SetUniformsD(*m_densityShader, startLayer);
	glBindVertexArray(m_vaoD);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, LAYERS);
	glBindVertexArray(0);
	glCheckError();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckError();
	glBindTexture(GL_TEXTURE_3D, 0);
	glCheckError();
	glUseProgram(0);
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckError();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glCheckError();
#endif
}

void ProcedualGenerator::Generate3dTexture()
{
	Generate3dTexture(0);
}

void ProcedualGenerator::GenerateVBO(glm::vec3 cubesPerDimension)
{
	glDeleteVertexArrays(1, &m_vaoMC);
	glDeleteBuffers(1, &m_vboMC);
	glCheckError();

	m_mcResolution = 2.0f / cubesPerDimension;
	glm::vec3 vertexPerDimension = cubesPerDimension - glm::vec3(1, 0, 1);

	m_vertexCount = vertexPerDimension.x * vertexPerDimension.y * vertexPerDimension.z;
	glm::vec3* vertices = new glm::vec3[m_vertexCount];

	glm::vec3 pos = glm::vec3(-1) + m_mcResolution / 2.0f;
	int index = 0;
	for (int layer = 0; layer < vertexPerDimension.y; ++layer)
	{
		pos.x = -1.0f + m_mcResolution.x / 2.0f;
		for (int x = 0; x < vertexPerDimension.x; ++x)
		{
			pos.z = -1.0f + m_mcResolution.z / 2.0f;
			for (int z = 0; z < vertexPerDimension.z; ++z)
			{
				vertices[index] = pos;
				++index;
				pos.z += m_mcResolution.z;
			}
			pos.x += m_mcResolution.x;
		}
		pos.y += m_mcResolution.y;
	}

	glGenVertexArrays(1, &m_vaoMC);
	glBindVertexArray(m_vaoMC);

	glGenBuffers(1, &m_vboMC);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboMC);
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
	return m_vboMC;
}

GLuint ProcedualGenerator::GetVaoId() const
{
	return m_vaoMC;
}

GLuint ProcedualGenerator::GetVertexCount() const
{
	return m_vertexCount;
}

void ProcedualGenerator::SetUniformsMC(Shader& shader)
{
	glm::vec3 resolution = m_mcResolution;
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

void ProcedualGenerator::SetUniformsD(Shader& shader, int startLayer)
{
	GLuint resLocation = glGetUniformLocation(shader.Program, "resolution");
	glUniform3f(resLocation, WIDTH, LAYERS, DEPTH);
	glCheckError();

	GLuint layerLocation = glGetUniformLocation(shader.Program, "startLayer");
	glUniform1i(layerLocation, startLayer);
	glCheckError();

	for (int i = 0; i < 4; ++i)
	{
		GLuint posLocation = glGetUniformLocation(shader.Program, ("pillars[" + std::to_string(i) + "].position").c_str());
		glUniform2fv(posLocation, 1, glm::value_ptr(m_pillars[i].position));
		glCheckError();

		GLuint freqLocation = glGetUniformLocation(shader.Program, ("pillars[" + std::to_string(i) + "].frequence").c_str());
		glUniform1f(freqLocation, m_pillars[i].frequence);
		glCheckError();

		GLuint weightLocation = glGetUniformLocation(shader.Program, ("pillars[" + std::to_string(i) + "].weight").c_str());
		glUniform1f(weightLocation, m_pillars[i].weight);
		glCheckError();
	}

	{
		GLuint weightLocation = glGetUniformLocation(shader.Program, "bound.weight");
		glUniform1f(weightLocation, m_bound.weight);
		glCheckError();
	}

	{
		GLuint offsetLocation = glGetUniformLocation(shader.Program, "helix.offset");
		glUniform1f(offsetLocation, m_helix.weight);
		glCheckError();

		GLuint freqLocation = glGetUniformLocation(shader.Program, "helix.frequence");
		glUniform1f(freqLocation, m_helix.weight);
		glCheckError();

		GLuint weightLocation = glGetUniformLocation(shader.Program, "helix.weight");
		glUniform1f(weightLocation, m_helix.weight);
		glCheckError();
	}

	{
		GLuint offsetLocation = glGetUniformLocation(shader.Program, "shelf.offset");
		glUniform1f(offsetLocation, m_shelf.weight);
		glCheckError();

		GLuint freqLocation = glGetUniformLocation(shader.Program, "shelf.frequence");
		glUniform1f(freqLocation, m_shelf.weight);
		glCheckError();

		GLuint weightLocation = glGetUniformLocation(shader.Program, "shelf.weight");
		glUniform1f(weightLocation, m_shelf.weight);
		glCheckError();
	}
}

void ProcedualGenerator::RenderDebugQuad(int layer) const
{
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
