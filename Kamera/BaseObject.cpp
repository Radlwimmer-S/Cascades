#include "BaseObject.h"

BaseObject::BaseObject(glm::vec3 position, glm::quat orientaton) : m_position(position), m_orientation(orientaton)
{
}

BaseObject::~BaseObject()
{
}

