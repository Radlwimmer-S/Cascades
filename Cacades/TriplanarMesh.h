#pragma once
#include <GL/glew.h>
#include "Texture.h"
#include "Enums.h"
#include <glm/detail/type_vec3.hpp>
#include "BaseObject.h"

class TriplanarMesh : public BaseObject
{
public:
	TriplanarMesh();
	~TriplanarMesh();

	GLuint GetVAO() const;
	GLuint GetVBO() const;
	void UpdateVao(int triCount);

	void Update(GLfloat deltaTime) override;
	void Render(Shader& shader) const override;

	GLsizei GetTriCount() const;

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLsizei m_triCount;
	glm::vec3 m_color;
	ColorBlendMode m_colorMode;
	NormalBlendMode m_normalMode;
	Texture* m_texture;
	Texture* m_normalMap;
	Texture* m_displacementMap;
};

