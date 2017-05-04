#include "ProcedualGenerator.h"
#include <GL/glew.h>
#include <glm/gtx/optimum_pow.hpp>
#include "Global.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <string>
#include "BoundingBox.h"

ProcedualGenerator::ProcedualGenerator(int seed) : m_random(seed), m_randomAngle(0, 359), m_randomRand(-glm::pi<float>(), glm::pi<float>()), m_randomFloat(0.0f, 1000.0f)
{
	SetupDensity(); 
	SetRandomSeed(seed);
	SetupMC();
}

void ProcedualGenerator::SetupMC()
{
	//const GLchar* feedbackVaryings[] = { "position", "normal"};
	////m_marchingCubeShader = new Shader("./shaders/MarchingCubes.vert", "./shaders/MarchingCubes.geom", "./shaders/MarchingCubes.frag");
	//m_marchingCubeShader = new Shader("./shaders/MarchingCubes.vert", "./shaders/MarchingCubes.geom", nullptr, feedbackVaryings, 2);
	//m_marchingCubeShader->Test("MarchingCubes");

	m_noise = new Noise[4]{
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 1)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 2)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 3)),
		Noise(glm::toMat4(MakeQuad(m_randomAngle(m_random), m_randomAngle(m_random), m_randomAngle(m_random))), NoiseTexture(16, 16, 16, 4))
	};
	//return random() & 1 ? 1 : -1
}

void ProcedualGenerator::SetupDensity()
{
	m_densityShader = new  Shader("./shaders/Density.vert", "./shaders/Density.geom", "./shaders/Density.frag");
	m_densityShader->Test("Density"); 

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

	m_vertexCount = vertexPerDimension.x * vertexPerDimension.z;
	glm::vec2* vertices = new glm::vec2[m_vertexCount];
	glm::vec2 tmpResolution(m_mcResolution.x, m_mcResolution.z);

	glm::vec2 pos = glm::vec2(-1) + tmpResolution / 2.0f;
	int index = 0;
	for (int x = 0; x < vertexPerDimension.x; ++x)
	{
		pos.y = -1.0f + m_mcResolution.z / 2.0f;
		for (int z = 0; z < vertexPerDimension.z; ++z)
		{
			vertices[index] = pos;
			++index;
			pos.y += m_mcResolution.z;
		}
		pos.x += m_mcResolution.x;
	}

	glGenVertexArrays(1, &m_vaoMC);
	glBindVertexArray(m_vaoMC);

	glGenBuffers(1, &m_vboMC);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboMC);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(glm::vec2), vertices, GL_STATIC_DRAW);
	glCheckError();
	// Position attribute
	glEnableVertexAttribArray(VS_IN_POSITION);
	glVertexAttribPointer(VS_IN_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
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

void ProcedualGenerator::SetRandomSeed(int seed)
{
	m_random.seed(seed);

	m_pillars[0] = Randoms{ m_randomRand(m_random), ToSignBit(m_random()), m_randomFloat(m_random) };
	m_pillars[1] = Randoms{ m_randomRand(m_random), ToSignBit(m_random()), m_randomFloat(m_random) };
	m_pillars[2] = Randoms{ m_randomRand(m_random), ToSignBit(m_random()), m_randomFloat(m_random) };
	m_pillars[3] = Randoms{ m_randomRand(m_random), ToSignBit(m_random()), m_randomFloat(m_random) };

	m_helix = Randoms{ m_randomRand(m_random), ToSignBit(m_random()), m_randomFloat(m_random) };

	m_shelf = Randoms{ m_randomRand(m_random), ToSignBit(m_random()), m_randomFloat(m_random) };
}

void ProcedualGenerator::SetUniformsMC(Shader& shader, int startLayer)
{
	glm::vec3 resolution = m_mcResolution;
	GLint resolutioneLoc = glGetUniformLocation(shader.Program, "resolution");
	glUniform3fv(resolutioneLoc, 1, glm::value_ptr(resolution));
	glCheckError();

	GLuint layerLocation = glGetUniformLocation(shader.Program, "startLayer");
	glUniform1i(layerLocation, startLayer);
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
		GLuint posLocation = glGetUniformLocation(shader.Program, ("pillars[" + std::to_string(i) + "].offset").c_str());
		glUniform1f(posLocation, m_pillars[i].offset);
		glCheckError();

		GLuint freqLocation = glGetUniformLocation(shader.Program, ("pillars[" + std::to_string(i) + "].frequence").c_str());
		glUniform1f(freqLocation, m_pillars[i].frequence);
		glCheckError();

		GLuint signLocation = glGetUniformLocation(shader.Program, ("pillars[" + std::to_string(i) + "].frequenceSign").c_str());
		glUniform1i(signLocation, m_pillars[i].frequenceSign);
		glCheckError();
	}

	{
		GLuint offsetLocation = glGetUniformLocation(shader.Program, "helix.offset");
		glUniform1f(offsetLocation, m_helix.offset);
		glCheckError();

		GLuint freqLocation = glGetUniformLocation(shader.Program, "helix.frequence");
		glUniform1f(freqLocation, m_helix.frequence);
		glCheckError();

		GLuint signLocation = glGetUniformLocation(shader.Program, "helix.frequenceSign");
		glUniform1i(signLocation, m_helix.frequenceSign);
		glCheckError();
	}

	{
		GLuint offsetLocation = glGetUniformLocation(shader.Program, "shelf.offset");
		glUniform1f(offsetLocation, m_shelf.offset);
		glCheckError();

		GLuint freqLocation = glGetUniformLocation(shader.Program, "shelf.frequence");
		glUniform1f(freqLocation, m_shelf.frequence);
		glCheckError();

		GLuint signLocation = glGetUniformLocation(shader.Program, "shelf.frequenceSign");
		glUniform1i(signLocation, m_shelf.frequenceSign);
		glCheckError();
	}
}

float ProcedualGenerator::NormalizeCoord(int coord, int dim)
{
	return 2.0f * (coord * 1.0f / dim - 0.5f);
}

int ProcedualGenerator::ToSignBit(int random)
{
	return random & 1 ? 1 : -1;
}
