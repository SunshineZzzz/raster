#pragma once

#include "Light.h"
#include "Object.h"

// 为啥要继承Object，球体作为点光源
class PointLight :public Light, public Object {
public:
	PointLight() {};
	~PointLight() {};

public:
	// 点光源光能衰减参数
	float m_k2 = 1.0f;
	float m_k1 = 1.0f;
	float m_kc = 1.0f;
};