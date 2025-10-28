#pragma once

#include "light.h"
#include "Object.h"

class SpotLight :public Light, public Object 
{
public:
	SpotLight() {};
	~SpotLight() {};

public:
	// 聚光灯朝向的方向
	glm::vec3 m_targetDirection{ -1.0f };
	// 光锥体张开角度，绝对可视区
	float m_innerAngle{ 0.0f };
	// 光锥体张开角度，边缘可视区
	float m_outerAngle{ 0.0f };
};