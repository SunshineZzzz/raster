#pragma once

#include "Material.h"
#include "Texture.h"

class PhongInstanceMaterial : public Material 
{
public:
	PhongInstanceMaterial()
	{
		m_type = MaterialType::PhongInstanceMaterial;
	}
	~PhongInstanceMaterial() {}

public:
	// 漫反射纹理
	Texture* m_diffuse{ nullptr };
	// 高光贴图蒙版，目的是控制物体表面不同区域的“闪亮”程度
	Texture* m_specularMask{ nullptr };
	// 光斑大小，值越大，光斑越小
	float m_shiness{ 1.0f };
};