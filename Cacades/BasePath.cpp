#include "BasePath.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Box.h"
#include <glm/gtx/spline.hpp>

BasePath::BasePath(std::vector<ControlPoint>& controlPoints, GLuint duration) : m_duration(duration), m_controlPoints(controlPoints), m_controlCount(controlPoints.size()), m_vao(0), m_timer(0.0f)
{
	m_debugCube = new Model(glm::vec3(0), glm::quat(), Box::GetTris(glm::vec3(0.1f, 0.1f, 0.1f)), 12, glm::vec3(1.0f, 0.1f, 0.0f));
}

BasePath::~BasePath()
{
}

void BasePath::Update(GLfloat deltaTime)
{
	m_timer += deltaTime;

	if (m_timer >= m_duration)
		m_timer -= m_duration;

	m_position = GetPosByTime(m_timer);
	m_rotation = GetRotByTime(m_timer);
}

void BasePath::Render(Shader& shader) const
{
	glm::vec3 color(1.0f, 0.1f, 0.0f);
	GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
	glUniform3f(objectColorLoc, color.x, color.y, color.z);

	glm::mat4 modelPos;
	modelPos = glm::translate(modelPos, glm::vec3(0, 0, 0));
	GLuint modelLocation = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelPos));

	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINE_STRIP, 0, PATH_APPROXIMATION);
	glBindVertexArray(0);

	for (std::vector<ControlPoint>::const_iterator it = m_controlPoints.begin(); it != m_controlPoints.end(); ++it)
	{
		m_debugCube->SetPosition(it->Position);
		m_debugCube->Render(shader);
	}
}


void BasePath::CalculateApprox()
{
	//rotated_point = origin + (orientation_quaternion * (point - origin));

	glm::vec3* pathApprox = new glm::vec3[PATH_APPROXIMATION]();
	for (int i = 0; i < PATH_APPROXIMATION; i++)
	{
		pathApprox[i] = GetPosByTime(i* 1.0f / PATH_APPROXIMATION * m_duration);
	}
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, PATH_APPROXIMATION * 3 * sizeof(GLfloat), pathApprox, GL_STATIC_DRAW);
	glVertexAttribPointer(VS_IN_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(VS_IN_POSITION);

	glBindVertexArray(0); // Unbind vao
}

glm::vec3 BasePath::GetPosByTime(GLfloat timestamp) const
{
	return CatmullRomSpline(m_controlPoints, NormalizeTimeStamp(timestamp));
}

glm::quat BasePath::GetRotByTime(GLfloat timestamp) const
{
	return Squad(m_controlPoints, NormalizeTimeStamp(timestamp));
}


GLfloat BasePath::NormalizeTimeStamp(GLfloat timestamp) const
{
	GLuint index = GetIndex(timestamp);

	GLfloat t = (timestamp - m_controlPoints[index].TimeStamp) / (m_controlPoints[index + 1].TimeStamp - m_controlPoints[index].TimeStamp);
	t += index;

	return t;
}

GLuint BasePath::GetIndex(GLfloat timestamp) const
{
	for (int i = 0; i < m_controlCount; i++)
	{
		if (m_controlPoints[i].TimeStamp <= timestamp && timestamp < m_controlPoints[i + 1].TimeStamp)
			return i;
	}
	std::cerr << "Error in CameraPath::GetIndex for timeStamp: " << timestamp << std::endl;
	return 0;
}

glm::vec3 BasePath::CatmullRomSpline(ControlPoint cp0, ControlPoint cp1, ControlPoint cp2, ControlPoint cp3, float t)
{
	return glm::catmullRom(cp0.Position, cp1.Position, cp2.Position, cp3.Position, t);
}

glm::quat BasePath::Squad(ControlPoint cp0, ControlPoint cp1, ControlPoint cp2, ControlPoint cp3, float t)
{
	glm::quat q0(ClampAngles(cp0.Orientation - cp1.Orientation));
	glm::quat q1;
	glm::quat q2(ClampAngles(cp2.Orientation - cp1.Orientation));
	glm::quat q3(ClampAngles(cp3.Orientation - cp1.Orientation));

	//return glm::normalize(glm::slerp(cp[i1].Rotation, cp[i2].Rotation, local_t));

	glm::quat s1 = glm::intermediate(q0, q1, q2);
	glm::quat s2 = glm::intermediate(q1, q2, q3);

	glm::quat rotation = glm::squad(q1, q2, s1, s2, t);
	glm::vec3 orientation = cp1.Orientation + glm::eulerAngles(rotation);

	return glm::quat(orientation);
}
