#pragma once

#include <glm/glm.hpp>

class Light 
{
public:
	Light() {};
	virtual ~Light() {};

public:
	// 光的颜色
	glm::vec3 m_color{ 1.0f };
	// 镜面反射光斑亮度
	float m_specularIntensity{ 1.0f };
};