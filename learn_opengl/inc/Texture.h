#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <string>
#include <map>

class Texture 
{
public:
	static std::map<std::string, Texture*> sTextureCache;
	static Texture* CreateTexture(const std::string& path, unsigned int unit);
	static Texture* CreateTextureFromMemory(
		const std::string& path,
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	// 创建颜色附件
	static Texture* CreateColorAttachment(
		unsigned int width,
		unsigned int height,
		unsigned int unit
	);
	// 创建深度模板附件
	static Texture* CreateDepthStencilAttachment(
		unsigned int width,
		unsigned int height,
		unsigned int unit
	);

	Texture();
	Texture(const std::string& path, unsigned int unit);
	Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
	// 产生一个空白纹理
	Texture(unsigned int width, unsigned int height, unsigned int unit);
	~Texture();

	// 纹理单元与纹理对象绑定
	// 存在多个纹理对象绑定一个纹理单元的情况，该函数做切换纹理单元与纹理对象绑定
	void Bind();
	// 是否已经初始化过
	bool IsInitialized() const;
	// 获取纹理单元号
	unsigned int GetUnit() const
	{
		return m_unit;
	}
	// 设置纹理单元号
	void SetUnit(unsigned int unit) 
	{ 
		m_unit = unit; 
	}
	// 获取纹理宽高
	int GetWidth()const { return m_width; }
	int GetHeight()const { return m_height; }
	GLuint GetTexture()const { return m_texture; }

private:
	// 纹理对象
	GLuint m_texture{ 0 };
	// 纹理单元号
	unsigned int m_unit{ 0 };
	// 是否已经初始化过
	bool m_initialized = false;

public:
	// 纹理宽高
	int m_width{ 0 };
	int m_height{ 0 };
};