#pragma once

#include "Material.h"

class DepthMaterial :public Material 
{
public:
	DepthMaterial()
	{
		m_type = MaterialType::DepthMaterial;
	};
	~DepthMaterial() {};
};