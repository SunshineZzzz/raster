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
	// 多边形面/线偏移相关
	bool m_polygonOffset{ false };
	unsigned int m_polygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	// Offset = factor * 最大深度斜率 + units * 深度精度的最小细分值
	// 最大深度斜率，深度值再屏幕空间变化的速度(x,y方向的最大值)
	float m_factor{ 0.0f };
	// 深度精度的最小细分值，即当前深度缓冲中，使用的数据格式所表达的最小值是多少
	float m_unit{ 0.0f };
};