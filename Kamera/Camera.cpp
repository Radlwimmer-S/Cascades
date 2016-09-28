#include "Camera.h"



Camera::Camera(glm::vec3 position, CameraPath& path, ViewPath& view) : m_position(position), m_path(path), m_view(view)
{
}


Camera::~Camera()
{
}
