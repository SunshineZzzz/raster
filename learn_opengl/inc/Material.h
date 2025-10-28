#pragma once

// 材质类型
enum class MaterialType 
{
	// 冯氏光照模型，漫反射+镜面反射+环境光
	PhongMaterial,
	// 点光源
	WhiteMaterial,
};

class Material 
{
public:
	Material();
	virtual ~Material();

public:
	MaterialType m_type;
};