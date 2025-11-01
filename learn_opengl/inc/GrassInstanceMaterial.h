#pragma once

#include "Material.h"
#include "Texture.h"

class GrassInstanceMaterial : public Material
{
public:
	GrassInstanceMaterial()
	{
		m_type = MaterialType::GrassInstanceMaterial;
	}
	~GrassInstanceMaterial() {}

public:
	// 漫反射纹理
	Texture* m_diffuse{ nullptr };
	// 草地透明蒙版纹理
	Texture* m_opacityMask{ nullptr };
	// 光斑大小，值越大，光斑越小
	float m_shiness{ 1.0f };

	// 草地贴图特性
	// 草地贴图，整体贴图，除法参数，用来调整fs中每个顶点映射到整体贴图的uv坐标
	float m_uvScale{ 1.0f };
	// 亮度与
	float m_brightness{ 1.0f };

	// 风力相关
	// 风力大小，风速
	float m_windScale{ 0.1f };
	// 风的方向
	glm::vec3 m_windDirection{ 1.0, 1.0,1.0 };
	// 相位调节
	float m_phaseScale{ 1.0f };

	// 云朵相关
	// 有云没云的蒙版，黑色表示有云，白色表示没云
	Texture* m_cloudMask{ nullptr };
	// 没有云的天空颜色
	glm::vec3 m_cloudWhiteColor{ 0.576, 1.0, 0.393 };
	// 有云的天空颜色
	glm::vec3 m_cloudBlackColor{ 0.994, 0.3, 0.426 };
	// 云朵贴图，整体贴图，除法参数，用来调整fs中每个顶点映射到整体贴图的uv坐标
	float m_cloudUVScale{ 1.0f };
	// 云移动的速度
	float m_cloudSpeed;
	// 云朵与草的混合比例
	float m_cloudLerp;
};