#include "Object.h"
#include <iostream>
#include "Global.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "TOL/tiny_obj_loader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.inl>
#include "Shader.h"


Object::Object(std::string filePath, glm::vec3 position) : m_position(position)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	GLuint* vao = new GLuint[shapes.size()];
	glGenVertexArrays(shapes.size(), vao);

	glGenBuffers(1, &m_vbo);

	GLuint* ebo = new GLuint[shapes.size()];
	glGenBuffers(shapes.size(), ebo);

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		glBindVertexArray(vao[s]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * attrib.vertices.size(), attrib.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[s]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[s].mesh.indices.size(), shapes[s].mesh.indices.data(), GL_STATIC_DRAW);
	
		// Position attribute
		glVertexAttribPointer(SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(SHADER_POSITION);

		glBindVertexArray(0); // Unbind m_vao
		
		m_meshes.push_back(new Mesh(vao[s], shapes[s].mesh.indices.size()));
	}
}


Object::~Object()
{
}

void Object::Update(GLfloat delta_time)
{
}

void Object::Render(const Shader& shader)
{
	glm::mat4 modelPos;
	modelPos = glm::translate(modelPos, m_position);
	GLuint modelLocation = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelPos));

	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		(*it)->Render(shader);
	}
}
