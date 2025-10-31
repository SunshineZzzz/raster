#pragma once

#include "Material.h"
#include "Texture.h"

class CubeMaterial : public Material 
{
public:
	CubeMaterial()
	{
		m_type = MaterialType::CubeMaterial;
	}
	~CubeMaterial() {};

public:
	Texture* m_diffuse{ nullptr };
};