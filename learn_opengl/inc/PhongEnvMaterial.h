#pragma once

#include "Material.h"
#include "Texture.h"

class PhongEnvMaterial : public Material 
{
public:
	PhongEnvMaterial()
	{
		m_type = MaterialType::PhongEnvMaterial;
	}
	~PhongEnvMaterial() {};

public:
	// 漫反射颜色，纹理
	Texture* m_diffuse{ nullptr };
	// 高光贴图蒙版，目的是控制物体表面不同区域的“闪亮”程度
	Texture* m_specularMask{ nullptr };
	// 环境光照贴图
	Texture* m_env{ nullptr };
	// 光斑大小，值越大，光斑越小
	float m_shiness{ 1.0f };
};