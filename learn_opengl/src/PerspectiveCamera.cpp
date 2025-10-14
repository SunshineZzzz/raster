#include <glm/gtc/matrix_transform.hpp>

#include "../inc/PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far) 
{
	m_fovy = fovy;
	m_aspect = aspect;
	m_near = near;
	m_far = far;
}

PerspectiveCamera::~PerspectiveCamera() {}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() 
{
	return glm::perspective(glm::radians(m_fovy), m_aspect, m_near, m_far);
}

void PerspectiveCamera::Scale(float deltaScale) 
{
	auto front = glm::cross(m_up, m_right);
	m_position += (front * deltaScale);
}