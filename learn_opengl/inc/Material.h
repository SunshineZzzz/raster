#pragma once

#include <glad/glad.h>

// 材质类型
enum class MaterialType 
{
	// 冯氏光照模型，漫反射+镜面反射+环境光
	PhongMaterial,
	// 点光源材质
	WhiteMaterial,
	// 深度材质
	DepthMaterial
};

class Material 
{
public:
	Material();
	virtual ~Material();

public:
	MaterialType m_type;
	// 深度检测相关
	bool m_depthTest{ true };
	GLenum m_depthFunc{ GL_LESS };
	bool m_depthWrite{ true };
};