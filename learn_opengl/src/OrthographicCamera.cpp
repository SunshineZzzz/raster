#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

#include "../inc/OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f)
{
	m_left = l;
	m_right = r;
	m_top = t;
	m_bottom = b;
	m_near = n;
	m_far = f;
}

OrthographicCamera::~OrthographicCamera() {}

glm::mat4 OrthographicCamera::GetProjectionMatrix() 
{
	// 0 - 1，+ - 放大，- - 缩小
	float scale = std::pow(2.0f, m_scale);
	return glm::ortho(m_left * scale, m_right * scale, m_bottom * scale, m_top * scale, m_near, m_far);
}

void OrthographicCamera::Scale(float deltaScale)
{
	m_scale += deltaScale;
}