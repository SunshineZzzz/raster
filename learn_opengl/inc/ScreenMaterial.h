#pragma once

#include "Material.h"
#include "Texture.h"

class ScreenMaterial :public Material 
{
public:
	ScreenMaterial() 
	{
		m_type = MaterialType::ScreenMaterial;
	};
	~ScreenMaterial() {};

public:
	Texture* m_screenTexture{ nullptr };
};