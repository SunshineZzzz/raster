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
	DepthMaterial,
	// 蒙版透明度材质
	OpacityMaskMaterial,
	// 屏幕材质
	ScreenMaterial,
	// 天空盒子材质
	CubeMaterial,
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
	// GL_LESS 在天空盒绘制时会出Z-FIGHTING情况，这是因为天空盒恒定为远平面，深度值一直是1，
	// 清除深度缓冲区的是填写的也是1，因为float类型，这两个值太近了，就出现了Z-FIGHTING情况
	// GLenum m_depthFunc{ GL_LESS };
	GLenum m_depthFunc{ GL_LEQUAL };
	bool m_depthWrite{ true };
	
	// 多边形面/线偏移相关
	bool m_polygonOffset{ false };
	unsigned int m_polygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	// Offset = factor * 最大深度斜率 + units * 深度精度的最小细分值
	// 最大深度斜率，深度值再屏幕空间变化的速度(x,y方向的最大值)
	float m_factor{ 0.0f };
	// 深度精度的最小细分值，即当前深度缓冲中，使用的数据格式所表达的最小值是多少
	float m_unit{ 0.0f };

	// 模板测试相关
	// 是否开启模板测试，glEnable(GL_STENCIL_TEST)
	bool m_stencilTest{ false };
	// glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)
	// 模板测试失败了怎么办
	unsigned int m_sFail{ GL_KEEP };
	// 模板测试通过但是深度检测没通过怎么办
	unsigned int m_zFail{ GL_KEEP };
	// 模板+深度测试都通过了怎么办
	unsigned int m_zPass{ GL_KEEP };
	// glStencilMask(0xFF)
	// 用于控制模板写入
	unsigned int m_stencilMask{ 0xFF };
	// glStencilFunc(GL_ALWAYS, 1, 0xFF)
	// 模板测试通过的条件
	unsigned int m_stencilFunc{ GL_ALWAYS };
	// 参考数值
	unsigned int m_stencilRef{ 0 };
	// 缓冲数值与参考数值的比较前，先与掩码进行与操作
	unsigned int m_stencilFuncMask{ 0xFF };

	// 颜色混合相关
	bool m_blend{ false };
	// 颜色混合方程
	GLenum m_sFactor{ GL_SRC_ALPHA };
	GLenum m_dFactor{ GL_ONE_MINUS_SRC_ALPHA };
	// 0.0f~1.0f，0.0f完全透明，1.0f完全不透明
	// 作用于物体本身透明度
	float m_opacity{ 1.0f };

	// 面剔除相关
	// 是否开启面剔除
	bool m_faceCulling{ false };
	// 面剔除方式
	GLenum m_frontFace{ GL_CCW };
	// 剔除正面还是背面
	GLenum m_cullFace{ GL_BACK };
};