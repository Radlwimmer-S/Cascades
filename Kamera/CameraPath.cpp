#include "CameraPath.h"
#include <vector>
#include <glm/gtx/spline.hpp>
#include "Global.h"
#include <glm/gtc/matrix_transform.inl>
#include <iostream>

CameraPath::CameraPath(std::vector<ControlPoint>& controlPoints, GLuint duration) : m_controlPoints(controlPoints), m_controlCount(controlPoints.size()), m_duration(duration), m_timer(0.0f), m_index(0)
{
	Initialize();
}

CameraPath::~CameraPath()
{
}

void CameraPath::Initialize()
{
	CalculateTimeSteps();
	CalculateApprox();
}

void CameraPath::Update(GLfloat deltaTime)
{
	GLfloat newTimer = m_timer + deltaTime;

	if (newTimer >= m_duration)
		newTimer -= m_duration;

	m_position = GetPosByTime(newTimer);

	m_timer = newTimer;
}

void CameraPath::Render(Shader& shader) const
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINE_STRIP, 0, PATH_APPROXIMATION);
	glBindVertexArray(0);
}

glm::vec3 CameraPath::GetPosByTime(GLfloat timestamp) const
{
	GLuint index = GetIndex(timestamp);

	GLfloat t = (timestamp - m_controlPoints[index].TimeStamp) / (m_controlPoints[index + 1].TimeStamp - m_controlPoints[index].TimeStamp);
	t += index;

	return CatmullRomSpline(m_controlPoints, t);
}

void CameraPath::CalculateTimeSteps()
{
	GLfloat pathLength = 0;
	std::vector<glm::vec3> vectors = std::vector<glm::vec3>();
	for (int i = 0; i < m_controlCount - 1; i++)
	{
		// V0 = P1 - P0
		vectors.push_back(m_controlPoints[i + 1].Position - m_controlPoints[i].Position);
		pathLength += glm::length(vectors[i]);
	}

	GLfloat velocity = pathLength / m_duration;

	m_controlPoints[0].TimeStamp = 0.0f;
	for (int i = 1; i < m_controlCount; i++)
	{
		// t1 = t0 + (s0 / v)
		m_controlPoints[i].TimeStamp = m_controlPoints[i - 1].TimeStamp + (glm::length(vectors[i - 1]) / velocity);
	}
	m_controlPoints[m_controlCount - 1].TimeStamp = m_duration;
}

void CameraPath::CalculateApprox()
{
	//rotated_point = origin + (orientation_quaternion * (point - origin));

	m_pathApprox = new glm::vec3[PATH_APPROXIMATION]();
	for (int i = 0; i < PATH_APPROXIMATION; i++)
	{
		m_pathApprox[i] = GetPosByTime(i* 1.0f / PATH_APPROXIMATION  * m_duration);
	}
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, PATH_APPROXIMATION * 3 * sizeof(GLfloat), m_pathApprox, GL_STATIC_DRAW);
	glVertexAttribPointer(SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(SHADER_POSITION);

	glBindVertexArray(0); // Unbind vao
}

GLuint CameraPath::GetIndex(GLfloat timestamp) const
{
	for (int i = 0; i < m_controlCount - 1; i++)
	{
		if (m_controlPoints[i].TimeStamp <= timestamp && timestamp < m_controlPoints[i + 1].TimeStamp)
			return i;
	}
	std::cerr << "Error in CameraPath::GetIndex for timeStamp: " << timestamp << std::endl;
	return 0;
}

glm::vec3 CameraPath::CatmullRomSpline(const std::vector<ControlPoint>& cp, float t) const
{
	// indices of the relevant control points
	int i0 = glm::clamp<int>(t - 1, 0, cp.size() - 1);
	int i1 = glm::clamp<int>(t - 0, 0, cp.size() - 1);
	int i2 = glm::clamp<int>(t + 1, 0, cp.size() - 1);
	int i3 = glm::clamp<int>(t + 2, 0, cp.size() - 1);

	// parameter on the local curve interval
	float local_t = glm::fract(t);

	return glm::catmullRom(cp[i0].Position, cp[i1].Position, cp[i2].Position, cp[i3].Position, local_t);
}
