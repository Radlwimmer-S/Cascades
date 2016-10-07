#include "CameraPath.h"
#include <vector>
#include <glm/gtx/spline.hpp>
#include "Global.h"
#include <glm/gtc/matrix_transform.inl>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

CameraPath::CameraPath(std::vector<ControlPoint>& controlPoints) : m_controlPoints(controlPoints), m_controlCount(controlPoints.size()), m_duration(controlPoints[controlPoints.size() - 1].TimeStamp), m_timer(0.0f), m_index(0)
{
	Initialize(false);
}

CameraPath::CameraPath(std::vector<ControlPoint>& controlPoints, GLuint duration) : m_controlPoints(controlPoints), m_controlCount(controlPoints.size()), m_duration(duration), m_timer(0.0f), m_index(0)
{
	Initialize(true);
}

CameraPath::~CameraPath()
{
}

void CameraPath::Initialize(bool recalculateTimeSteps)
{
	DoubleControlPoints();
	DoubleControlPoints();
	if (recalculateTimeSteps)
		CalculateTimeSteps();
	CalculateApprox();
}

void CameraPath::Update(GLfloat deltaTime)
{
	GLfloat newTimer = m_timer + deltaTime;

	if (newTimer >= m_duration)
		newTimer -= m_duration;

	m_position = GetPosByTime(newTimer);
	m_rotation = GetRotByTime(newTimer);

	m_timer = newTimer;
}

void CameraPath::Render(Shader& shader) const
{
	glm::mat4 modelPos;
	modelPos = glm::translate(modelPos, glm::vec3(0,0,0));
	GLuint modelLocation = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelPos));

	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINE_STRIP, 0, PATH_APPROXIMATION);
	glBindVertexArray(0);
}

glm::vec3 CameraPath::GetPosByTime(GLfloat timestamp) const
{
	return CatmullRomSpline(m_controlPoints, NormalizeTimeStamp(timestamp));
}

glm::quat CameraPath::GetRotByTime(GLfloat timestamp) const
{
	return Squad(m_controlPoints, NormalizeTimeStamp(timestamp));
}

void CameraPath::DoubleControlPoints()
{
	std::vector<ControlPoint>* newCP = new std::vector<ControlPoint>();
	for (int i = 0; i < m_controlCount - 1; i++)
	{
		newCP->push_back(m_controlPoints[i]);
		glm::vec3 position = CatmullRomSpline(m_controlPoints, i + 0.5f);
		glm::quat rotation = Squad(m_controlPoints, i + 0.5f);
		GLfloat timestamp = m_controlPoints[i].TimeStamp + (m_controlPoints[i + 1].TimeStamp - m_controlPoints[i].TimeStamp) / 2;
		newCP->push_back(ControlPoint(position, rotation, timestamp));
	}
	newCP->push_back(m_controlPoints[m_controlCount - 1]);
	m_controlPoints = *newCP;
	m_controlCount = 2 * m_controlCount - 1;
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

GLfloat CameraPath::NormalizeTimeStamp(GLfloat timestamp) const
{
	GLuint index = GetIndex(timestamp);

	GLfloat t = (timestamp - m_controlPoints[index].TimeStamp) / (m_controlPoints[index + 1].TimeStamp - m_controlPoints[index].TimeStamp);
	t += index;

	return t;
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

glm::quat CameraPath::Squad(const std::vector<ControlPoint>& cp, float t) const
{
	// indices of the relevant control points
	int i0 = glm::clamp<int>(t - 1, 0, cp.size() - 1);
	int i1 = glm::clamp<int>(t - 0, 0, cp.size() - 1);
	int i2 = glm::clamp<int>(t + 1, 0, cp.size() - 1);
	int i3 = glm::clamp<int>(t + 2, 0, cp.size() - 1);

	// parameter on the local curve interval
	float local_t = glm::fract(t);

	glm::quat s1 = glm::intermediate(cp[i0].Rotation, cp[i1].Rotation, cp[i2].Rotation);
	glm::quat s2 = glm::intermediate(cp[i1].Rotation, cp[i2].Rotation, cp[i3].Rotation);

	return glm::squad(cp[i1].Rotation, cp[i2].Rotation, s1, s2, local_t);
}
