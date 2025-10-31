#pragma once

#include "Material.h"
#include "Texture.h"

class OpacityMaskMaterial : public Material 
{
public:
	OpacityMaskMaterial()
	{
		m_type = MaterialType::OpacityMaskMaterial;
	}
	~OpacityMaskMaterial() {}

public:
	Texture* m_diffuse{ nullptr };
	Texture* m_opacityMask{ nullptr };
};