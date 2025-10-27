#pragma once

#include "Light.h"

class DirectionalLight :public Light 
{
public:
	DirectionalLight() {};
	~DirectionalLight() {};

public:
	// 方向
	glm::vec3 m_direction{ -1.0 };
};