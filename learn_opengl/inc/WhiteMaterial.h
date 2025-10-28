#pragma once

#include "Material.h"

class WhiteMaterial :public Material 
{
public:
	WhiteMaterial()
	{
		m_type = MaterialType::WhiteMaterial;
	}
	~WhiteMaterial() {}
};